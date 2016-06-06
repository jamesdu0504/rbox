
#pragma once

#include <algorithm/triangulate.hpp>

#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cstdint>
#include <algorithm>

namespace rbox {

namespace detail {

    inline double random(const double xmin = 0., const double xmax = 1.)
    {
        static bool first = true;
        static size_t count = 0;
        if (first)
        {
            first = false;
            srand((unsigned) time(NULL));
        }
        count++;
        return xmin + (xmax - xmin) * rand() / RAND_MAX;
    }

    template <typename Vertex>
    inline Vertex random_point_in_triangle(const Vertex& c1, const Vertex& c2, const Vertex& c3)
    {
        double x = random();
        double y = random();
        if(x + y > 1.)
        {
            x = 1. - x;
            y = 1. - y;
        }
        Vertex output;
        output.x = (c2.x - c1.x) * x + (c3.x - c1.x) * y + c1.x;
        output.y = (c2.y - c1.y) * x + (c3.y - c1.y) * y + c1.y;
        return output;
    }

    template <typename Vertex>
    inline double triangle_area(const Vertex& c1, const Vertex& c2, const Vertex& c3)
    {
        return double(std::abs((c1.x - c3.x) * (c2.y - c1.y) - (c1.x - c2.x) * (c3.y - c1.y))) / 2;
    }
    
}

template <typename Vertex>
using generate_point_callback = void(*)(const Vertex& c);

template <typename Vertex, typename Polygon = std::vector<std::vector<Vertex>>>
inline void random_point_in_polygon(Polygon const& poly, size_t generate_nums, generate_point_callback<Vertex> handler_point)
{
    auto triangluate = triangulate<Vertex>(poly);
    triangluate.run();
    const auto &vertices = triangluate.vertices();
    const auto &indices = triangluate.indices();

    std::vector<double> tri_areas;
    double poly_area = 0.;
    double area = 0.;
    for (size_t i = 0; i < indices.size(); i += 3)
    {
        area = detail::triangle_area(
            vertices[indices[i]],
            vertices[indices[i + 1]],
            vertices[indices[i + 2]]);
        tri_areas.push_back(area);
        poly_area += area;
    }

    size_t ready_num = 0;
    for (size_t i = 0; i < indices.size(); i += 3)
    {
        size_t temp = size_t(tri_areas[i/3] / poly_area * generate_nums + 0.5);
        if (ready_num + temp > generate_nums)
        {
            temp = generate_nums - ready_num;
        }
        for (size_t j = 0; j < temp; j++)
        {
            auto& coord = detail::random_point_in_triangle(
                vertices[indices[i]],
                vertices[indices[i + 1]],
                vertices[indices[i + 2]]);
            if (handler_point)
            {
                handler_point(coord);
            }
        }
        ready_num += temp;
    }

    if (ready_num < generate_nums)
    {
        size_t temp = generate_nums - ready_num;
        int max_pos = std::distance(tri_areas.begin(), std::max_element(tri_areas.begin(), tri_areas.end()));
        for (size_t j = 0; j < temp; j++)
        {
            auto& coord = detail::random_point_in_triangle(
                vertices[indices[3 * max_pos]],
                vertices[indices[3 * max_pos + 1]],
                vertices[indices[3 * max_pos + 2]]);
            if (handler_point)
            {
                handler_point(coord);
            }
        }
    }
}

}

