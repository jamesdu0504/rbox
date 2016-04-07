
#pragma once

#ifndef WITH_MAPNIK
#include <mapbox/util/variant.hpp>
#include <coord.hpp>
#include <vector>
#include <type_traits>
#include <cstddef>

namespace rbox {

template <typename T>
struct point
{
    using value_type = T;
    point() {}
    point(T x_, T y_)
        : x(x_), y(y_)
    {}
    point(coord<value_type, 2> const& c)
        : x(c.x), y(c.y) {}

    point(point const& other) = default;
    point(point && other) noexcept = default;
    point & operator=(point const& other) = default;
    friend inline bool operator== (point<T> const& a, point<T> const& b)
    {
        return a.x == b.x && a.y == b.y;
    }
    friend inline bool operator!= (point<T> const& a, point <T> const& b)
    {
        return a.x != b.x  || a.y != b.y;
    }
    value_type x;
    value_type y;
};


template <typename T>
struct line_string : std::vector<point<T> >
{
    line_string() = default;
    line_string (std::size_t size)
        : std::vector<point<T> >(size) {}
    line_string (line_string && other) = default ;
    line_string& operator=(line_string &&) = default;
    line_string (line_string const& ) = default;
    line_string& operator=(line_string const&) = default;
    inline std::size_t num_points() const { return std::vector<point<T>>::size(); }
    inline void add_coord(T x, T y) { std::vector<point<T>>::template emplace_back(x,y);}
};

template <typename T>
struct linear_ring : line_string<T>
{
    linear_ring() = default;
    linear_ring(std::size_t size)
        : line_string<T>(size) {}
    linear_ring (linear_ring && other) = default ;
    linear_ring& operator=(linear_ring &&) = default;
    linear_ring(line_string<T> && other)
        : line_string<T>(other) {}
    linear_ring (linear_ring const& ) = default;
    linear_ring(line_string<T> const& other)
        : line_string<T>(other) {}
    linear_ring& operator=(linear_ring const&) = default;

};

template <typename T>
using rings_container = std::vector<linear_ring<T>>;

template <typename T, template <typename> class InteriorRings = rings_container>
struct polygon
{
    linear_ring<T> exterior_ring;
    using rings_container = InteriorRings<T>;
    rings_container interior_rings;

    polygon() = default;
    inline void set_exterior_ring(linear_ring<T> && ring)
    {
        exterior_ring = std::move(ring);
    }

    inline void add_hole(linear_ring<T> && ring)
    {
        interior_rings.emplace_back(std::move(ring));
    }

    inline bool empty() const { return exterior_ring.empty(); }

    inline std::size_t num_rings() const
    {
        return 1 + interior_rings.size();
    }
};

template <typename T>
struct multi_point : line_string<T> {};

template <typename T>
struct multi_line_string : std::vector<line_string<T>> {};

template <typename T>
struct multi_polygon : std::vector<polygon<T>> {};

template <typename T>
struct geometry_collection;

struct geometry_empty {};


template <typename T>
using geometry_base = mapbox::util::variant<geometry_empty,
                                            point<T>,
                                            line_string<T>,
                                            polygon<T>,
                                            multi_point<T>,
                                            multi_line_string<T>,
                                            multi_polygon<T>,
                                            mapbox::util::recursive_wrapper<geometry_collection<T> > >;
template <typename T>
struct geometry : geometry_base<T>
{
    using value_type = T;

    geometry()
        : geometry_base<T>() {} // empty

    template <typename G>
    geometry(G && geom)
        : geometry_base<T>(std::forward<G>(geom)) {}

};

template <typename T>
struct geometry_collection : std::vector<geometry<T>> {};

}

#else
#include <mapnik/geometry>
namespace rbox {
    template <typename T>
    using point = mapnik::geometry::point;
    template <typename T>
    using line_string = mapnik::geometry::line_string;
    template <typename T>
    using line_ring = mapnik::geometry::line_ring;
    template <typename T>
    using rings_container = mapnik::geometry::rings_container;
    template <typename T>
    using polygon = mapnik::geometry::polygon;
    template <typename T>
    using multi_point = mapnik::geometry::multi_point;
    template <typename T>
    using multi_line_string = mapnik::geometry::multi_line_string;
    template <typename T>
    using multi_polygon = mapnik::geometry::multi_polygon;
    template <typename T>
    using geometry_collection = mapnik::geometry::geometry_collection;
    template <typename T>
    using geometry_empty = mapnik::geometry::geometry_empty;
    template <typename T>
    using geometry_base = mapnik::geometry::geometry_base;
    template <typename T>
    using geometry = mapnik::geometry::geometry;
}
#endif

