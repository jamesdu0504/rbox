#pragma once

#include <geometry.hpp>
#include <geometry_types.hpp>

namespace rbox {
namespace detail {

struct geometry_type
{
    template <typename T>
    geometry_types operator () (T const& geom) const
    {
        return mapbox::util::apply_visitor(*this, geom);
    }

    geometry_types operator() (geometry_empty const& ) const
    {
        return geometry_types::Unknown;
    }

    template <typename T>
    geometry_types operator () (point<T> const&) const
    {
        return geometry_types::Point;
    }

    template <typename T>
    geometry_types operator () (line_string<T> const&) const
    {
        return geometry_types::LineString;
    }

    template <typename T>
    geometry_types operator () (polygon<T> const&) const
    {
        return geometry_types::Polygon;
    }

    template <typename T>
    geometry_types operator () (multi_point<T> const&) const
    {
        return geometry_types::MultiPoint;
    }

    template <typename T>
    geometry_types operator () (multi_line_string<T> const&) const
    {
        return geometry_types::MultiLineString;
    }

    template <typename T>
    geometry_types operator () (multi_polygon<T> const&) const
    {
        return geometry_types::MultiPolygon;
    }

    template <typename T>
    geometry_types operator () (geometry_collection<T> const&) const
    {
        return geometry_types::GeometryCollection;
    }
};
} // detail

template <typename T>
static inline geometry_types geometry_type(T const& geom)
{
    return detail::geometry_type()(geom);
}

}

