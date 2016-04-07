#pragma once 

#include <geometry.hpp>
#include <algorithm/geometry_is_empty.hpp>

namespace rbox {

namespace detail {

struct geometry_remove_empty
{
    rbox::multi_line_string<double> operator() (rbox::multi_line_string<double> const & geom) const
    {
        return remove_empty(geom);
    }

    rbox::multi_polygon<double> operator() (rbox::multi_polygon<double> const & geom) const
    {
        return remove_empty(geom);
    }

    template <typename T>
    T operator() (T const & geom) const
    {
        return geom;
    }

private:
    template <typename T>
    T remove_empty(T const & geom) const
    {
        T new_geom;
        for (auto const & g : geom)
        {
            if (!g.empty())
            {
                new_geom.emplace_back(g);
            }
        }
        return new_geom;
    }
};

}

template <typename GeomType>
inline GeomType remove_empty(GeomType const& geom)
{
    return detail::geometry_remove_empty()(geom);
}

}
