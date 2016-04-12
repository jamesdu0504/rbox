#pragma once

#include <mapbox/util/earcut.hpp>
#include <coord.hpp>
#include <geometry.hpp>

#include <memory>
#include <vector>

// earcut geometry type adapter
namespace mapbox {
namespace util {

template <> struct nth<0, rbox::coord2d> {
    inline static double get(const rbox::coord2d &t) {
        return t.x;
    };
};

template <> struct nth<1, rbox::coord2d> {
    inline static double get(const rbox::coord2d &t) {
        return t.y;
    };
};


template <> struct nth<0, rbox::coord2i> {
    inline static double get(const rbox::coord2i &t) {
        return t.x;
    };
};
template <> struct nth<1, rbox::coord2i> {
    inline static double get(const rbox::coord2i &t) {
        return t.y;
    };
};

template <> struct nth<0, rbox::point<double>> {
    inline static double get(const rbox::point<double> &t) {
        return t.x;
    };
};

template <> struct nth<1, rbox::point<double>> {
    inline static double get(const rbox::point<double> &t) {
        return t.y;
    };
};

template <> struct nth<0, rbox::point<int>> {
    inline static double get(const rbox::point<double> &t) {
        return t.x;
    };
};

template <> struct nth<1, rbox::point<int>> {
    inline static double get(const rbox::point<double> &t) {
        return t.y;
    };
};

}
}


namespace rbox {

template <typename Vertex, typename Polygon = std::vector<std::vector<Vertex>>, typename IndexType = uint32_t>
class triangulate
{
public:
    using Vertices = std::vector<Vertex>;
    using Indices = std::vector<IndexType>;

    triangulate(const Polygon &polygon_)
        : polygon(polygon_)
    {
        for (const auto& ring : polygon_) 
        {
            for (const auto& vertex : ring)
            {
                vertices_.emplace_back(Vertex(vertex));
            }
        }
    }

    void run() {
        indices_ = mapbox::earcut<IndexType>(polygon);
    }

    Indices indices() const
    {
        return indices_;
    }

    Vertices vertices() const
    {
        return vertices_;
    }

    std::size_t num_triangles() const
    {
        return indices_.size() / 3;
    }

private:
    const Polygon &polygon;
    Vertices vertices_;
    Indices indices_;
};

}