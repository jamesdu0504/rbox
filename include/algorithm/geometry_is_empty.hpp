#pragma once 

#include <geometry.hpp>

namespace rbox { 

namespace detail {

struct geometry_is_empty
{
    bool operator() (rbox::geometry<double> const& geom) const
    {
        return mapbox::util::apply_visitor(*this, geom);
    }

    bool operator() (rbox::point<double> const&) const
    {
        return false;
    }

    bool operator() (rbox::line_string<double> const& geom) const
    {
        return geom.empty();
    }

    bool operator() (rbox::polygon<double> const& geom) const
    {
        return geom.empty();
    }

    bool operator() (rbox::multi_point<double> const& geom) const
    {
        return geom.empty();
    }

    bool operator() (rbox::multi_line_string<double> const& geom) const
    {
        return geom.empty();
    }

    bool operator() (rbox::multi_polygon<double> const& geom) const
    {
        return geom.empty();
    }

    bool operator() (rbox::geometry_collection<double> const& geom) const
    {
        return geom.empty();
    }

    template <typename T>
    bool operator() (T const&) const
    {
        return true;
    }

};

struct geometry_has_empty
{
    bool operator() (rbox::geometry<double> const& geom) const
    {
        return mapbox::util::apply_visitor(*this, geom);
    }

    bool operator() (rbox::geometry_empty const&) const
    {
        return false;
    }

    bool operator() (rbox::point<double> const&) const
    {
        return false;
    }

    bool operator() (rbox::line_string<double> const&) const
    {
        return false;
    }

    bool operator() (rbox::polygon<double> const&) const
    {
        return false;
    }

    bool operator() (rbox::multi_point<double> const&) const
    {
        return false;
    }

    bool operator() (rbox::multi_line_string<double> const& geom) const
    {
        return test_multigeometry(geom);
    }

    bool operator() (rbox::multi_polygon<double> const& geom) const
    {
        return test_multigeometry(geom);
    }

    bool operator() (rbox::geometry_collection<double> const& geom) const
    {
        for (auto const & item : geom)
        {
            if (geometry_is_empty()(item) || (*this)(item))
            {
                return true;
            }
        }
        return false;
    }

    template <typename T>
    bool operator() (T const&) const
    {
        return true;
    }

private:
    template <typename T>
    bool test_multigeometry(T const & geom) const
    {
        for (auto const & item : geom)
        {
            if (item.empty())
            {
                return true;
            }
        }
        return false;
    }
};

}

template <typename GeomType>
inline bool is_empty(GeomType const& geom)
{
    return detail::geometry_is_empty()(geom);
}

template <typename GeomType>
inline bool has_empty(GeomType const& geom)
{
    return detail::geometry_has_empty()(geom);
}

}

