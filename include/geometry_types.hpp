#pragma once

#include <cstdint>

namespace rbox {

// OGC compatible types
enum geometry_types : std::uint8_t
{
    Unknown = 0,
    Point =   1,
    LineString = 2,
    Polygon = 3,
    MultiPoint = 4,
    MultiLineString = 5,
    MultiPolygon = 6,
    GeometryCollection = 7,
};

}
