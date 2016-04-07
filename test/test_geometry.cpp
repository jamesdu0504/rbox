
#include <algorithm/geometry_is_empty.hpp>
#include <algorithm/geometry_remove_empty.hpp>

#include <geometry.hpp>
#include <geometry_type.hpp>
#include <geometry_types.hpp>

#include <iostream>
#include <cassert>

using namespace rbox;

int main()
{
    geometry_empty empty;
    assert(is_empty(empty));
    assert(geometry_type(empty) == geometry_types::Unknown);

    point<double> p1 = {1., 1.5};
    assert(geometry_type(p1) == geometry_types::Point);

    multi_point<double> mp;
    mp.emplace_back(std::move(p1));
    mp.add_coord(3., 3.);
    mp.emplace_back();
    assert(mp.size() == 3);

    line_string<double> line;
    assert(is_empty(line));
    line.add_coord(0., 0.);
    line.add_coord(1., 1.);
    line.add_coord(1., -1.);
    assert(!is_empty(line));
    assert(!has_empty(line));
    assert(geometry_type(line) == geometry_types::LineString);

    multi_line_string<double> mline;
    mline.emplace_back(std::move(line));
    mline.emplace_back();
    assert(has_empty(mline));
    assert(mline.size() == 2);
    auto mline2 = remove_empty(mline);
    assert(mline2.size() == 1);

    linear_ring<double> outer;
    outer.add_coord(0., 0.);
    outer.add_coord(1., 1.);
    outer.add_coord(1., -1.);
    outer.add_coord(0., 0.);
    
    linear_ring<double> inner;
    inner.add_coord(0., 0.9);
    inner.add_coord(-0.9, 0.);
    inner.add_coord(0.9, 0.);
    inner.add_coord(0., 0.9);

    linear_ring<double> inner2;

    polygon<double> poly;
    poly.set_exterior_ring(std::move(outer));
    poly.add_hole(std::move(inner));
    poly.add_hole(std::move(inner2));
    assert(poly.num_rings() == 3);
    assert(!is_empty(poly));

    geometry<double> geom;
    geom = p1;
    assert(geometry_type(geom) == geometry_types::Point);
    geom = line;
    assert(geometry_type(geom) == geometry_types::LineString);
    geom = mline;
    assert(geometry_type(geom) == geometry_types::MultiLineString);
    geom = poly;
    assert(geometry_type(geom) == geometry_types::Polygon);


    geometry_collection<double> col;
    assert(is_empty(col));
    col.emplace_back(std::move(empty));
    col.emplace_back(std::move(p1));
    col.emplace_back(std::move(mp));
    col.emplace_back(std::move(line));
    col.emplace_back(std::move(mline));
    col.emplace_back(std::move(poly));
    assert(!is_empty(col));
    assert(has_empty(col));

    return 0;
}
