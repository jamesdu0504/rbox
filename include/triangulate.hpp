#pragma once
#include <earcut.hpp>
#include <coord.hpp>

#include <memory>
#include <vector>

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

}
}

namespace rbox {

template <typename Vertex, typename IndexType = uint32_t>
class Triangulate {
public:
    using Vertices = std::vector<Vertex>;
    using Polygon = std::vector<std::vector<Vertex>>;

    Triangulate(const Polygon &polygon_)
        : polygon(polygon_)
    {
        for (const auto& ring : polygon_) {
            for (const auto& vertex : ring) {
                vertices_.emplace_back(Vertex(vertex));
            }
        }
    }

    void run() {
        indices_ = mapbox::earcut<IndexType>(polygon);
    }

    std::vector<IndexType> indices() const {
        return indices_;
    }

    Vertices vertices() const {
        return vertices_;
    }

private:
    const Polygon &polygon;
    Vertices vertices_;
    std::vector<IndexType> indices_;
};

}