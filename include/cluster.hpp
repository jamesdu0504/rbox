#ifndef CLUSTER_H
#define CLUSTER_H

#include <rtree.hpp>
#include <coord.hpp>
#include <vector>
#include <memory>
#include <cmath>

#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#endif

namespace rbox {

template <typename DATATYPE>
struct ClusterPointT
{
  coord2d center;
  coord2d centroid;
  int level;
  int count;
  DATATYPE data;

  ClusterPointT(double x = .0, double y = .0): center(x,y), centroid(x,y), level(99), count(1), data(DATATYPE()){}

  ~ClusterPointT(){}

};

template <typename DATATYPE, int NODE_SIZE = 16>
struct Cluster
{
  typedef ClusterPointT<DATATYPE> ClusterPoint;
  typedef std::shared_ptr<ClusterPoint> ClusterPointPtr;
  typedef RTree<ClusterPointPtr, double, 2, double, NODE_SIZE> ClusterRTree;
  typedef std::shared_ptr<ClusterRTree> ClusterRTreePtr;

  int min_level;
  int max_level;
  int radius;
  int extent;
  double proj_len;

  std::vector<ClusterRTreePtr> _rtrees;

  Cluster(int min = 0, int max = 16, int radius = 40, int extent = 256, double proj_len = -1.)
    :min_level(min), max_level(max), radius(radius), extent(extent), proj_len(proj_len)
  {
    for(int i = 0; i <= max_level + 1; ++i)
    {
      _rtrees.push_back(std::make_shared<ClusterRTree>());
    }
  }

  ~Cluster(){}

  ClusterPointPtr create_cluster_point(double lon, double lat, bool bproj = false)
  {
    if(bproj)
    {
      double x,y;
      if(proj_len < .0)
      {
        x = lon / 360. + 0.5;
        double sin = std::sin(lat * M_PI / 180.);
        y = (0.5 - 0.25 * std::log((1 + sin) / (1 - sin)) / M_PI);
        y = y < 0 ? 0 : y;
        y = y > 1 ? 1 : y;
      }
      else
      {
        x = lon / proj_len + 0.5;
        y = lat / proj_len + 0.5;
      }
      return std::make_shared<ClusterPoint>(x, y);
    }
    else
    {
      return std::make_shared<ClusterPoint>(lon, lat);
    }
  }

  coord2d get_cluster_center_coord(ClusterPointPtr point, bool bproj = false)
  {
    if(bproj)
    {
      double x,y;
      if(proj_len < .0)
      {
        x = (point->center.x - 0.5) * 360.;
        double y2 = (180. - point->center.y * 360.) * M_PI / 180.;
        y = 360. * std::atan(std::exp(y2)) / M_PI - 90.;
      }
      else
      {
        x = (point->center.x - 0.5) * proj_len;
        y = (point->center.y - 0.5) * proj_len;
      }
      return coord2d(x, y);
    }
    else
    {
      return coord2d(point->center.x, point->center.y);
    }
  }

  coord2d get_cluster_centroid_coord(ClusterPointPtr point, bool bproj = false)
  {
    if(bproj)
    {
      double x,y;
      if(proj_len < .0)
      {
        x = (point->centroid.x - 0.5) * 360.;
        double y2 = (180. - point->centroid.y * 360.) * M_PI / 180.;
        y = 360. * std::atan(std::exp(y2)) / M_PI - 90.;
      }
      else
      {
        x = (point->centroid.x - 0.5) * proj_len;
        y = (point->centroid.y - 0.5) * proj_len;
      }
      return coord2d(x, y);
    }
    else
    {
      return coord2d(point->centroid.x, point->centroid.y);
    }
  }

  double _distance_between_cluster_points(ClusterPointPtr point1, ClusterPointPtr point2)
  {
    double dx = point1->centroid.x - point2->centroid.x;
    double dy = point1->centroid.y - point2->centroid.y;
    return dx*dx + dy*dy;
  }

  void load(std::vector<ClusterPointPtr>& cluster_points)
  {
    for (auto it = cluster_points.begin(); it != cluster_points.end(); ++it)
    {
      insert_cluster_point(_rtrees[max_level+1], *it);
    }

    for (int z = max_level; z >= min_level; z--) 
    {
      cluster(z, _rtrees[z+1], _rtrees[z]);
    }

    //for (int z = max_level; z >= min_level; z--) 
    //{
    //  std::vector<ClusterPointPtr> new_cluster_points;
    //  cluster(z, cluster_points, new_cluster_points);
    //  cluster_points = new_cluster_points;
    //}

  }

  void cluster(int level, ClusterRTreePtr tree, ClusterRTreePtr new_tree)
  {
    double r = radius/(extent * std::pow(2., level));
    double bbox_min[2] = {.0, .0};
    double bbox_max[2] = {.0, .0};

    ClusterRTree::Iterator it;
    for(tree->GetFirst(it); !tree->IsNull(it); tree->GetNext(it))
    {
      auto p = tree->GetAt(it);
      if (p->level <= level)
      {
        continue;
      }
      p->level = level;

      bbox_min[0] = p->centroid.x - r;
      bbox_min[1] = p->centroid.y - r;
      bbox_max[0] = p->centroid.x + r;
      bbox_max[1] = p->centroid.y + r;
      std::vector<ClusterPointPtr> bboxNeighbors;
      tree->Search(bbox_min, bbox_max, bboxNeighbors);

      bool foundNeighbors = false;
      int point_num = p->count;
      double wx = p->centroid.x * point_num;
      double wy = p->centroid.y * point_num;

      for (size_t j = 0; j < bboxNeighbors.size(); j++)
      {
        auto b = bboxNeighbors[j];
        if (level < b->level && _distance_between_cluster_points(p, b) <= r * r)
        //if (level < b->level)
        {
          foundNeighbors = true;
          b->level = level;
          wx += b->centroid.x * b->count;
          wy += b->centroid.x * b->count;
          point_num += b->count;
        }
      }

      if (!foundNeighbors)
      {
        insert_cluster_point(new_tree, p);
        continue;
      }

      auto cluster_point = create_cluster_point(p->center.x, p->center.y);
      cluster_point->count = point_num;

      cluster_point->centroid.x = wx / point_num;
      cluster_point->centroid.y = wy / point_num;

      insert_cluster_point(new_tree, cluster_point);
    }
  }
  
  void cluster(int level, std::vector<ClusterPointPtr>& old_points, std::vector<ClusterPointPtr>& new_points)
  {
    ClusterRTreePtr tree = _rtrees[level + 1];
    ClusterRTreePtr new_tree = _rtrees[level];

    double r = radius/(extent * std::pow(2., level));
    double bbox_min[2] = {.0, .0};
    double bbox_max[2] = {.0, .0};

    for(int i = 0; i < old_points.size(); ++i)
    {
      auto p = old_points[i];
      if (p->level <= level)
      {
        continue;
      }
      p->level = level;

      bbox_min[0] = p->centroid.x - r;
      bbox_min[1] = p->centroid.y - r;
      bbox_max[0] = p->centroid.x + r;
      bbox_max[1] = p->centroid.y + r;
      std::vector<ClusterPointPtr> bboxNeighbors;
      tree->Search(bbox_min, bbox_max, bboxNeighbors);

      bool foundNeighbors = false;
      int point_num = p->count;
      double wx = p->centroid.x * point_num;
      double wy = p->centroid.y * point_num;

      for (size_t j = 0; j < bboxNeighbors.size(); j++)
      {
        auto b = bboxNeighbors[j];
        if (level < b->level && _distance_between_cluster_points(p, b) <= r * r)
        //if (level < b->level)
        {
          foundNeighbors = true;
          b->level = level;
          wx += b->centroid.x * b->count;
          wy += b->centroid.x * b->count;
          point_num += b->count;
        }
      }

      if (!foundNeighbors)
      {
        insert_cluster_point(new_tree, p);
        new_points.push_back(p);
        continue;
      }

      auto cluster_point = create_cluster_point(p->center.x, p->center.y);
      cluster_point->count = point_num;

      cluster_point->centroid.x = wx / point_num;
      cluster_point->centroid.y = wy / point_num;

      insert_cluster_point(new_tree, cluster_point);
      new_points.push_back(cluster_point);
    }
  }

  void get_clusters(coord2d& min_coord, coord2d& max_coord, ClusterRTreePtr tree, std::vector<ClusterPointPtr>& result)
  {
    double min[2] = {min_coord.x, min_coord.y};
    double max[2] = {max_coord.x, max_coord.y};
    tree->Search(min, max, result);
  }

  void get_clusters(coord2d& min_coord, coord2d& max_coord, int level, std::vector<ClusterPointPtr>& result)
  {
    int z = std::max(min_level, std::min(level, max_level + 1));
    auto tree = _rtrees[z];
    get_clusters(min_coord, max_coord, tree, result);
  }

  void get_clusters(coord3i& tile_id, std::vector<ClusterPointPtr>& result)
  {
    double z2 = std::pow(2., tile_id.z);
    double p = radius / extent;
    coord2d min((tile_id.x - p) / z2, (tile_id.y - p) / z2);
    coord2d max((tile_id.x + 1 + p) / z2, (tile_id.y + 1 + p) / z2);
    get_clusters(min, max, tile_id.z, result);
  }

  int get_clusters_num(int level)
  {
    if(level < min_level)
      return 0;
    if(level > max_level + 1)
      return 0;
    return _rtrees[level]->Count();
  }

  void insert_cluster_point(ClusterRTreePtr tree, ClusterPointPtr cluster_point)
  {
    double min[2] = {cluster_point->centroid.x, cluster_point->centroid.y};
    double max[2] = {cluster_point->centroid.x, cluster_point->centroid.y};
    tree->Insert(min, max, cluster_point);
  }

};

} // rbox

#endif // CLUSTER_H
