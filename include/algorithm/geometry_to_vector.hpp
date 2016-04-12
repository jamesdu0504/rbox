#pragma once 

#include <geometry.hpp>

namespace rbox { 

template <typename T>
std::vector<point<T>> geometry_to_vector(line_string<T> const& geom)
{
    std::vector<point<T>> res;
    for (const auto& point : geom)
    {
        res.emplace_back(std::move(point));
    }
    return res;
}

template <typename T>
std::vector<point<T>> geometry_to_vector(linear_ring<T> const& geom)
{
    std::vector<point<T>> res;
    for (const auto& point : geom)
    {
        res.emplace_back(std::move(point));
    }
    return res;
}

template <typename T>
std::vector<std::vector<point<T>>> geometry_to_vector(polygon<T> const& geom)
{
    std::vector<std::vector<point<T>>> res;
    res.emplace_back(std::move(geometry_to_vector(geom.exterior_ring)));
    for (const auto& ring : geom.interior_rings)
    {
        res.emplace_back(std::move(geometry_to_vector(ring)));
    }
    return res;
}

}

