//
// test.cpp
//

#include <string>
#include <iostream>
#include <memory>
#include <fstream>
#include <cassert>

#include <rtree.hpp>
#include <cluster.hpp>

//#include <string_utils/string_utils.hpp>

using namespace std;
using namespace rbox;

struct Point
{
    double lon;
    double lat;
    Point(double lon = .0, double lat = .0):lon(lon),lat(lat){}
};
typedef Cluster<shared_ptr<Point>> MyCluster;

void print_cluster(MyCluster& cluster)
{
    //for (int i = cluster.min_level; i <= cluster.max_level + 1; ++i)
    //{
    //    cout << "level " << i << ": " << cluster.get_clusters_num(i) << endl;
    //    ofstream tree_point_out("tree_center_" + string_utils::to_string<string>(i) + ".csv");
    //    ofstream tree_point_out2("tree_centroid_" + string_utils::to_string<string>(i) + ".csv");
    //    auto tree = cluster._rtrees[i];
    //    MyCluster::ClusterRTree::Iterator it;
    //    int count = 0;
    //    for (tree->GetFirst(it); !tree->IsNull(it); tree->GetNext(it))
    //    {
    //        auto p = tree->GetAt(it);
    //        coord2d co = cluster.get_cluster_center_coord(p);
    //        coord2d co2 = cluster.get_cluster_centroid_coord(p);
    //        tree_point_out << co.x << "," << co.y << "," << p->count << endl;
    //        tree_point_out2 << co2.x << "," << co2.y << "," << p->count << endl;
    //        count += p->count;
    //    }
    //    cout << "point count: " << count << endl;
    //    tree_point_out.close();
    //    tree_point_out2.close();
    //}
}

int main()
{
    MyCluster cluster(0, 5);
    vector<shared_ptr<Point>> points;
    vector<MyCluster::ClusterPointPtr> cpoints;
    int scale = 4;

    for(int i = -180*scale; i < 180*scale; ++i)
    {
        for(int j = -85*scale; j < 85*scale; ++j)
        {
            auto point = make_shared<Point>(i*1./scale, j*1./scale);
            points.push_back(point);
        }
    }
    assert(points.size() == 979200);

    for(auto it = points.begin(); it != points.end(); ++it)
    {
        auto point = *it;
        auto cpoint = cluster.create_cluster_point(point->lon, point->lat, true);
        cpoint->data = point;
        cpoints.push_back(cpoint);
    }
    points.clear();
    cluster.load(cpoints);
    cpoints.clear();

    print_cluster(cluster);
    assert(cluster.get_clusters_num(6) == 979200);
    assert(cluster.get_clusters_num(5) == 33012);
    assert(cluster.get_clusters_num(4) == 1596);
    assert(cluster.get_clusters_num(3) == 485);
    assert(cluster.get_clusters_num(2) == 187);
    assert(cluster.get_clusters_num(1) == 58);
    assert(cluster.get_clusters_num(0) == 18);

    vector<MyCluster::ClusterPointPtr> result;
    cluster.get_clusters(coord3i(0, 0, 0), result);
    assert(result.size() == cluster.get_clusters_num(0));
    result.clear();

    int count = 0;
    cluster.get_clusters(coord3i(0, 0, 1), result);
    count += result.size();
    assert(result.size() == 24);
    result.clear();
    cluster.get_clusters(coord3i(0, 1, 1), result);
    count += result.size();
    assert(result.size() == 5);
    result.clear();
    cluster.get_clusters(coord3i(1, 1, 1), result);
    count += result.size();
    assert(result.size() == 20);
    result.clear();
    cluster.get_clusters(coord3i(1, 0, 1), result);
    count += result.size();
    assert(result.size() == 9);

    assert(count == cluster.get_clusters_num(1));
    return 0;
}

