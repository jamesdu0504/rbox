
#ifndef COORD_HPP
#define COORD_HPP

namespace mapdb {

template <typename T,int dim>
struct coord {
    //using type = T;
    typedef T type;
};

template <typename T>
struct coord<T,2>
{
    //using type = T;
    typedef T type;
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

    //coord(coord<T,2> && rhs) noexcept
    //    : x(std::move(rhs.x)),
    //      y(std::move(rhs.y)) {}
    coord(coord<T,2> && rhs)
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
    //using type = T;
    typedef T type;
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

    //coord(coord<T,3> && rhs) noexcept
    //    : x(std::move(rhs.x)),
    //      y(std::move(rhs.y)),
    //      z(std::move(rhs.z)) {}
    coord(coord<T,3> && rhs)
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

typedef coord<double, 2> coord2d;
typedef coord<int, 2> coord2i;
typedef coord<double, 3> coord3d;
typedef coord<int, 3> coord3i;

} // mapdb
#endif // COORD_HPP
