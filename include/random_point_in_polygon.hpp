
#ifndef RANDOM_POINTS_IN_POLYGON_HPP
#define RANDOM_POINTS_IN_POLYGON_HPP

#include <triangulate.hpp>

#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cstdint>
#include <algorithm>

namespace rbox {

namespace rpip {

static inline double random(const double xmin = 0., const double xmax = 1.)
{
    static bool first = true;
    static uint64_t count = 0;
    if (first)
    {
        first = false;
        srand((unsigned) time(NULL));
    }
    count++;
    return xmin + (xmax - xmin) * rand() / RAND_MAX;
}

static coord2d random_point_in_triangle(const coord2d& c1, const coord2d& c2, const coord2d& c3)
{
    double x = random();
    double y = random();
    if(x + y > 1.)
    {
        x = 1. - x;
        y = 1. - y;
    }
    coord2d output;
    output.x = (c2.x - c1.x) * x + (c3.x - c1.x) * y + c1.x;
    output.y = (c2.y - c1.y) * x + (c3.y - c1.y) * y + c1.y;
    return output;
}

static double triangle_area(const coord2d& c1, const coord2d& c2, const coord2d& c3)
{
    return double(std::abs((c1.x - c3.x) * (c2.y - c1.y) - (c1.x - c2.x) * (c3.y - c1.y))) / 2;
}

typedef void (*generate_point_callback)(const coord2d& c);
typedef std::vector<std::vector<coord2d>> polygon;
 
static void random_point_in_polygon(polygon const& poly, uint64_t generate_nums, generate_point_callback handler_point)
{
    auto triangluate = Triangulate<coord2d>(poly);
    triangluate.run();
    const auto &vertices = triangluate.vertices();
    const auto &indices = triangluate.indices();
    // calculate area
    std::vector<double> tri_areas;
    double poly_area = 0.;
    double area = 0.;
    for (size_t i = 0; i < indices.size(); i = i + 3)
    {
        area = triangle_area(
            vertices[indices[i]],
            vertices[indices[i + 1]],
            vertices[indices[i + 2]]);
        tri_areas.push_back(area);
        poly_area += area;
    }

    //std::stable_sort(tri_areas.begin(), tri_areas.end());

    uint64_t ready_num = 0;
    for (uint64_t i = 0; i < indices.size(); i = i + 3)
    {
        uint64_t temp = uint64_t(tri_areas[i/3] / poly_area * generate_nums + 0.5);
        if (ready_num + temp > generate_nums)
        {
            temp = generate_nums - ready_num;
        }
        for (uint64_t j = 0; j < temp; j++)
        {
            auto& coord = random_point_in_triangle(
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
        uint64_t temp = generate_nums - ready_num;
        for (uint64_t j = 0; j < temp; j++)
        {
            auto& coord = random_point_in_triangle(
                vertices[indices[0]],
                vertices[indices[1]],
                vertices[indices[2]]);
            if (handler_point)
            {
                handler_point(coord);
            }
        }
    }
}

}
}

#endif // RANDOM_POINTS_IN_POLYGON_HPP
