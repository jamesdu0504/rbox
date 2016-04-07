
#pragma once

#ifndef WITH_MAPNIK

namespace rbox {

template <typename T, int dim>
struct coord {
    using type = T;
};

template <typename T>
struct coord<T,2>
{
    using type = T;
    T x;
    T y;
public:
    coord()
        : x(),y() {}
    coord(T x_,T y_)
        : x(x_),y(y_) {}

    coord(coord<T,2> const& rhs)
        : x(rhs.x),
        y(rhs.y) {}

    template <typename T2>
    coord (coord<T2,2> const& rhs)
        : x(type(rhs.x)),
        y(type(rhs.y)) {}

    coord(coord<T,2> && rhs) noexcept
        : x(std::move(rhs.x)),
          y(std::move(rhs.y)) {}

    coord<T,2>& operator=(coord<T,2> rhs)
    {
        swap(rhs);
        return *this;
    }

    template <typename T2>
    coord<T,2>& operator=(const coord<T2,2>& rhs)
    {
        coord<T,2> tmp(rhs);
        swap(rhs);
        return *this;
    }

    template <typename T2>
    bool operator==(coord<T2,2> const& rhs)
    {
        return x == rhs.x && y == rhs.y;
    }

    template <typename T2>
    bool operator!=(coord<T2, 2> const& rhs)
    {
        return x != rhs.x || y != rhs.y;
    }

    coord<T,2>& operator+=(coord<T,2> const& rhs)
    {
        x+=rhs.x;
        y+=rhs.y;
        return *this;
    }

    coord<T,2>& operator+=(T rhs)
    {
        x+=rhs;
        y+=rhs;
        return *this;
    }

    coord<T,2>& operator-=(coord<T,2> const& rhs)
    {
        x-=rhs.x;
        y-=rhs.y;
        return *this;
    }

    coord<T,2>& operator-=(T rhs)
    {
        x-=rhs;
        y-=rhs;
        return *this;
    }

    coord<T,2>& operator*=(T t)
    {
        x*=t;
        y*=t;
        return *this;
    }
    coord<T,2>& operator/=(T t)
    {
        x/=t;
        y/=t;
        return *this;
    }
    coord<T, 2> operator+(coord<T, 2> rhs)
    {
        coord<T, 2> tmp(*this);
        tmp.x += rhs.x;
        tmp.y += rhs.y;
        return tmp;
    }
    coord<T, 2> operator+(T t)
    {
        coord<T, 2> tmp(*this);
        tmp.x += t;
        tmp.y += t;
        return tmp;
    }
    coord<T, 2> operator-(coord<T, 2> rhs)
    {
        coord<T, 2> tmp(*this);
        tmp.x -= rhs.x;
        tmp.y -= rhs.y;
        return tmp;
    }
    coord<T, 2> operator-(T t)
    {
        coord<T, 2> tmp(*this);
        tmp.x -= t;
        tmp.y -= t;
        return tmp;
    }
    coord<T, 2> operator*(T t)
    {
        coord<T, 2> tmp(*this);
        tmp.x *= t;
        tmp.y *= t;
        return tmp;
    }
    coord<T, 2> operator/(T t)
    {
        coord<T, 2> tmp(*this);
        tmp.x /= t;
        tmp.y /= t;
        return tmp;
    }
private:
    void swap(coord<T,2> & rhs)
    {
        std::swap(this->x, rhs.x);
        std::swap(this->y, rhs.y);
    }
};

template <typename T>
struct coord<T,3>
{
    using type = T;
    T x;
    T y;
    T z;
public:
    coord()
        : x(),y(),z() {}
    coord(T x_,T y_,T z_)
        : x(x_),y(y_),z(z_) {}

    template <typename T2>
    coord (coord<T2,3> const& rhs)
        : x(type(rhs.x)),
        y(type(rhs.y)),
        z(type(rhs.z)) {}

    coord(coord<T,3> && rhs) noexcept
        : x(std::move(rhs.x)),
          y(std::move(rhs.y)),
          z(std::move(rhs.z)) {}

    coord<T,3> operator=(coord<T,3> rhs)
    {
        swap(rhs);
        return *this;
    }

    template <typename T2>
    coord<T,3>& operator=(coord<T2,3> const& rhs)
    {
        coord<T,3> tmp(rhs);
        swap(tmp);
        return *this;
    }
private:
    void swap(coord<T,3> & rhs)
    {
        std::swap(this->x, rhs.x);
        std::swap(this->y, rhs.y);
        std::swap(this->z, rhs.z);
    }
};
}
#else
#include <mapnik/coord>
namespace rbox {
    template <typename T, int dim>
    using coord = mapnik::coord<T, dim>;

    template <typename T>
    using coord2 = mapnik::coord2<T, 2>;

    template <typename T>
    using coord3 = mapnik::coord3<T, 3>;
}
#endif

namespace rbox {
using coord2d = coord<double, 2>;
using coord2i = coord<int, 2>;
using coord3d = coord<double, 3>;
using coord3i = coord<int, 3>;
}

