
#pragma once

#include <coord.hpp>

#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>

namespace rbox {

namespace geohash {

    typedef std::string string_type;
    typedef std::vector<string_type> string_vector;

    struct bounding_box
    {
        coord2d min;
        coord2d max;
        bounding_box(double x1 = -180., double y1 = -90., double x2 = 180., double y2 = 90.) :
            min(std::min(x1, x2), std::min(y1, y2)), max(std::max(x1, x2), std::max(y1, y2))
        {}
        bounding_box(coord2d const& coord1, coord2d const& coord2) :
            min(std::min(coord1.x, coord2.x), std::min(coord1.y, coord2.y)),
            max(std::max(coord1.x, coord2.x), std::max(coord1.y, coord2.y))
        {}
        bounding_box(coord2d& coord, double dis)
        {
            // TODO
        }
        bool contains(coord2d const& coord) const
        {
            return (coord.x >= min.x) && (coord.x <= max.x) && (coord.y >= min.y) && (coord.y <= max.y);
        }
        double x_center() const
        {
            return (min.x + max.x) / 2;
        }
        double y_center() const
        {
            return (min.y + max.y) / 2;
        }
        double x_range() const
        {
            return max.x - min.x;
        }
        double y_range() const
        {
            return max.y - min.y;
        }
        coord2d center() const
        {
            return coord2d(x_center(), y_center());
        }
        coord2d bottom_left() const
        {
            return min;
        }
        coord2d bottom_right() const
        {
            return coord2d(max.x, min.y);
        }
        coord2d top_left() const
        {
            return coord2d(min.x, max.y);
        }
        coord2d top_right() const
        {
            return max;
        }
        coord2d offset(coord2d& coord) const
        {
            double x = (coord.x - min.x) / x_range();
            double y = (coord.y - min.y) / y_range();
            return{ x, y };
        }
        bounding_box& merge(const bounding_box &b) {
            min.x = std::min(min.x, b.min.x);
            max.x = std::max(max.x, b.max.x);
            min.y = std::min(min.y, b.min.y);
            max.y = std::max(max.y, b.max.y);
            return *this;
        }
    };

    inline bool operator==(const bounding_box &b1, const bounding_box &b2) {
        return (b1.min.x == b2.min.x) && (b1.max.x == b2.max.x)
            && (b1.min.y == b2.min.y) && (b1.max.y == b2.max.y);
    }

    inline bool operator!=(const bounding_box &b1, const bounding_box &b2) {
        return !(b1 == b2);
    }

    inline bounding_box merge(const bounding_box &b1, const bounding_box &b2) {
        return bounding_box(
            std::min(b1.min.x, b2.min.x),
            std::min(b1.min.y, b2.min.y),
            std::max(b1.max.x, b2.max.x),
            std::max(b1.max.y, b2.max.y));
    }

    static const char base32_codes[] =
    {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'b', 'c', 'd', 'e', 'f', 'g',
        'h', 'j', 'k', 'm', 'n', 'p', 'q', 'r',
        's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    };

    static const int base32_indexes[] = 
    {
        0,  1,  2,  3,  4,  5,  6,  7, // 30-37, '0'..'7'
        8,  9, -1, -1, -1, -1, -1, -1, // 38-2F, '8','9'
        -1, -1, 10, 11, 12, 13, 14, 15, // 40-47, 'B'..'G'
        16, -1, 17, 18, -1, 19, 20, -1, // 48-4F, 'H','J','K','M','N'
        21, 22, 23, 24, 25, 26, 27, 28, // 50-57, 'P'..'W'
        29, 30, 31, -1, -1, -1, -1, -1, // 58-5F, 'X'..'Z'
        -1, -1, 10, 11, 12, 13, 14, 15, // 60-67, 'b'..'g'
        16, -1, 17, 18, -1, 19, 20, -1, // 68-6F, 'h','j','k','m','n'
        21, 22, 23, 24, 25, 26, 27, 28, // 70-77, 'p'..'w'
        29, 30, 31,                     // 78-7A, 'x'..'z'
    };
    
    struct morton_code
    {
        morton_code() = default;

        morton_code(uint64_t b, size_t p) : bits(b), precision(p)
        {
            if (p > 64)
            {
                throw std::invalid_argument("Invalid precision");
            }
        }

        morton_code(const std::string &bit_string): bits(0), precision(bit_string.size())
        {
            if (bit_string.size() > 64)
            {
                throw std::invalid_argument("Invalid bit_string");
            }
            for (auto c : bit_string)
            {
                if (c == '1')
                    bits = (bits << 1) | 1;
                else
                    bits = bits << 1;
            }
        }

        static morton_code from_geohash(const std::string &hash_string)
        {
            if (hash_string.size() > 12)
            {
                throw std::invalid_argument("Invalid hash_string");
            }
            morton_code output;
            for (auto c : hash_string)
            {
                int char_index = base32_indexes[c - 48];
                if (char_index < 0)
                {
                    throw std::invalid_argument("Invalid hash_string");
                }
                for (int bits = 4; bits >= 0; --bits)
                {
                    output.push_back(((char_index >> bits) & 1) != 0);
                }
            }
            return output;
        }

        size_t size() const
        {
            return precision;
        }

        bool empty() const
        {
            return size() == 0;
        }

        bool test(size_t n) const
        {
            return (bits & (1ull << (precision - n))) != 0;
        }

        void push_back(bool b) 
        {
            bits <<= 1; bits |= (b ? 1 : 0); precision++;
        }

        operator std::string() const 
        {
            return to_string();
        }

        std::string to_string() const
        {
            std::string output(precision, ' ');
            uint64_t b = bits << (64 - precision);
            for (size_t i = 0; i < precision; i++)
            {
                output[i] = (b & 0x8000000000000000LL) ? '1' : '0';
                b <<= 1;
            }
            return output;
        }

        std::string to_geohash() const
        {
            int hash_precision = int(precision / 5);
            std::string output(hash_precision, ' ');
            uint64_t b = bits << (64 - precision);
            int num_bits = 0;
            int hash_index = 0;
            int output_length = 0;
            int val = 0;
            for (int i = 0; i < hash_precision*5; i++)
            {
                num_bits++;
                val = (b & 0x8000000000000000LL) ? 1 : 0;
                b <<= 1;
                hash_index = (hash_index << 1) + val;
                if (5 == num_bits) {
                    output[output_length] = base32_codes[hash_index];
                    output_length++;
                    num_bits = 0;
                    hash_index = 0;
                }
            }
            return output;
        }

        static morton_code from_offset(coord2d& coord, size_t precision)
        {
            static const uint64_t B[] = 
            { 
                0x5555555555555555LL, 0x3333333333333333LL, 0x0F0F0F0F0F0F0F0FLL, 0x00FF00FF00FF00FFLL, 0x0000FFFF0000FFFFLL
            };
            static const unsigned int S[] = { 1, 2, 4, 8, 16 };

            //convert it to fixed point based on the step size
            int y_precision = int(precision / 2);
            int x_precision = precision - y_precision;

            uint64_t x = (uint64_t)(coord.x * (1 << x_precision)); // Interleave lower  bits of x and y, so the bits of x
            uint64_t y = (uint64_t)(coord.y * (1 << y_precision)); // are in the even positions and bits from y in the odd; //https://graphics.stanford.edu/~seander/bithacks.html#InterleaveBMN

            x = (x | (x << S[4])) & B[4];
            y = (y | (y << S[4])) & B[4];

            x = (x | (x << S[3])) & B[3];
            y = (y | (y << S[3])) & B[3];

            x = (x | (x << S[2])) & B[2];
            y = (y | (y << S[2])) & B[2];

            x = (x | (x << S[1])) & B[1];
            y = (y | (y << S[1])) & B[1];

            x = (x | (x << S[0])) & B[0];
            y = (y | (y << S[0])) & B[0];

            uint64_t bits = y | (x << 1);
            if (x_precision > y_precision)
            {
                bits = x | (y << 1);
            }
            return morton_code(bits, precision);
        }

        bounding_box to_bbox() const
        {

            static const uint64_t B[] =
            {
                0x5555555555555555LL, 0x3333333333333333LL, 0x0F0F0F0F0F0F0F0FLL,
                0x00FF00FF00FF00FFLL, 0x0000FFFF0000FFFFLL, 0x00000000FFFFFFFFLL
            };

            static const unsigned int S[] = { 0, 1, 2, 4, 8, 16 };

            int y_precision = int(precision / 2);
            int x_precision = precision - y_precision;

            ///reverse the interleave process (http://stackoverflow.com/questions/4909263/how-to-efficiently-de-interleave-bits-inverse-morton)
            uint64_t x = bits >> 1;
            uint64_t y = bits;
            if (x_precision > y_precision)
            {
                x = bits;
                y = bits >> 1;
            }

            x = (x | (x >> S[0])) & B[0];
            y = (y | (y >> S[0])) & B[0];

            x = (x | (x >> S[1])) & B[1];
            y = (y | (y >> S[1])) & B[1];

            x = (x | (x >> S[2])) & B[2];
            y = (y | (y >> S[2])) & B[2];

            x = (x | (x >> S[3])) & B[3];
            y = (y | (y >> S[3])) & B[3];

            x = (x | (x >> S[4])) & B[4];
            y = (y | (y >> S[4])) & B[4];

            x = (x | (x >> S[5])) & B[5];
            y = (y | (y >> S[5])) & B[5];

            uint32_t ixo = (uint32_t)x;
            uint32_t iyo = (uint32_t)y;

            coord2d min, max;
            double xmin = ixo * 1.0 / (1ull << x_precision);
            double xmax = (ixo + 1) * 1.0 / (1ull << x_precision);
            double ymin = iyo * 1.0 / (1ull << y_precision);
            double ymax = (iyo + 1) * 1.0 / (1ull << y_precision);
            return bounding_box(xmin, ymin, xmax, ymax);
        }

        uint64_t bits = 0;
        size_t precision = 0;
    };

    static inline bool operator==(const morton_code &b1, const morton_code &b2)
    {
        return b1.empty() ? b2.empty() : (b1.bits == b2.bits && b1.precision == b2.precision);
    }

    static inline bool operator!=(const morton_code &b1, const morton_code &b2)
    {
        return !(b1 == b2);
    }

    static morton_code common_encode(const coord2d &coord, const int precision, bounding_box bbox = bounding_box())
    {
        if (coord.x > bbox.max.x || coord.x < bbox.min.x || coord.y > bbox.max.y || coord.y < bbox.min.y)
        {
            throw std::invalid_argument("Invalid coord");
        }

        if (precision > 64)
        {
            throw std::invalid_argument("Invalid precision");
        }

        bool is_x = true;
        morton_code output;

        while (output.size() < precision)
        {
            if (is_x)
            {
                if (coord.x > bbox.x_center())
                {
                    output.push_back(true);
                    bbox.min.x = bbox.x_center();
                }
                else
                {
                    output.push_back(false);
                    bbox.max.x = bbox.x_center();
                }
            }
            else
            {
                if (coord.y > bbox.y_center())
                {
                    output.push_back(true);
                    bbox.min.y = bbox.y_center();
                }
                else
                {
                    output.push_back(false);
                    bbox.max.y = bbox.y_center();
                }
            }
            is_x = !is_x;
        }
        return output;
    }

    static string_type encode(const coord2d &coord, const int precision, const bounding_box& bbox=bounding_box())
    {
        morton_code bcode = common_encode(coord, precision * 5, bbox);
        return bcode.to_geohash();
    }

    static bounding_box common_decode(const morton_code& bcode, const bounding_box& bbox = bounding_box())
    {
        bounding_box output = bbox;

        bool is_x = true;

        for (size_t i = 1; i <= bcode.size(); i++)
        {
            bool bit = bcode.test(i);
            if (is_x)
            {
                if (bit)
                {
                    output.min.x = output.x_center();
                }
                else
                {
                    output.max.x = output.x_center();
                }
            }
            else
            {
                if (bit)
                {
                    output.min.y = output.y_center();
                }
                else
                {
                    output.max.y = output.y_center();
                }
            }
            is_x = !is_x;
        }
        return output;
    }

    static bounding_box decode(const string_type& hash_string, const bounding_box& bbox = bounding_box())
    {
        morton_code bcode = morton_code::from_geohash(hash_string);
        return common_decode(bcode, bbox);
    }

    static morton_code neighbor(const morton_code &bcode, const std::pair<int, int> &direction, const bounding_box& bbox = bounding_box())
    {
        bounding_box b = common_decode(bcode, bbox);
        coord2d cp = b.center();
        cp.x += direction.first * b.x_range();
        cp.y += direction.second * b.y_range();
        return common_encode(cp, bcode.size(), bbox);
    }

    static string_type neighbor(const string_type hash_string, const std::pair<int, int> &direction, const bounding_box& bbox = bounding_box())
    {
        bounding_box b = decode(hash_string, bbox);
        coord2d cp = b.center();
        cp.x += direction.first * b.x_range();
        cp.y += direction.second * b.y_range();
        return encode(cp, hash_string.size(), bbox);
    }

    static string_vector neighbors(const string_type hash_string, const bounding_box& bbox = bounding_box())
    {
        morton_code bcode = morton_code::from_geohash(hash_string);
        string_vector result;
        result.push_back(neighbor(bcode, { -1, -1 }).to_geohash());
        result.push_back(neighbor(bcode, { -1,  0 }).to_geohash());
        result.push_back(neighbor(bcode, { -1,  1 }).to_geohash());
        result.push_back(neighbor(bcode, {  0, -1 }).to_geohash());
        result.push_back(neighbor(bcode, {  0,  1 }).to_geohash());
        result.push_back(neighbor(bcode, {  1, -1 }).to_geohash());
        result.push_back(neighbor(bcode, {  1,  0 }).to_geohash());
        result.push_back(neighbor(bcode, {  1,  1 }).to_geohash());
        return result;
    }

    static morton_code fast_encode(coord2d coord, size_t precision, bounding_box& bbox = bounding_box())
    {
        if (coord.x > bbox.max.x || coord.x < bbox.min.x || coord.y > bbox.max.y || coord.y < bbox.min.y)
        {
            throw std::invalid_argument("Invalid coord");
        }

        if (precision > 64)
        {
            throw std::invalid_argument("Invalid precision");
        }
        
        //compute the coordinate in the range 0-1
        coord2d offset = bbox.offset(coord);
        return morton_code::from_offset(offset, precision);
    }

    bounding_box fast_decode(const morton_code& bcode, const bounding_box& bbox = bounding_box())
    {
        bounding_box offset_box = bcode.to_bbox();
        double xmin = bbox.min.x + offset_box.min.x*bbox.x_range();
        double xmax = bbox.min.x + offset_box.max.x*bbox.x_range();
        double ymin = bbox.min.y + offset_box.min.y*bbox.y_range();
        double ymax = bbox.min.y + offset_box.max.y*bbox.y_range();
        return bounding_box(xmin, ymin, xmax, ymax);
    }

}

}

