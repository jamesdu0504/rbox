//
// test.cpp
//

#include <string>
#include <iostream>
#include <memory>
#include <fstream>

#include <rtree.hpp>
#include <cluster.hpp>

#include <string_utils/string_utils.hpp>

using namespace std;
using namespace rbox;
using namespace string_utils;

typedef int ValueType;

struct Rect
{
    Rect()  {}

    Rect(int a_minX, int a_minY, int a_maxX, int a_maxY)
    {
        min[0] = a_minX;
        min[1] = a_minY;

        max[0] = a_maxX;
        max[1] = a_maxY;
    }


    int min[2];
    int max[2];
};

struct Rect rects[] =
{
    Rect(0, 0, 2, 2), // xmin, ymin, xmax, ymax (for 2 dimensional RTree)
    Rect(5, 5, 7, 7),
    Rect(8, 5, 9, 6),
    Rect(7, 1, 9, 2),
};


int nrects = sizeof(rects) / sizeof(rects[0]);

Rect search_rect(6, 4, 10, 6); // search will find above rects that this one overlaps


bool MySearchCallback(ValueType id, void* arg)
{
    cout << "Hit data rect " << id << "\n";
    return true; // keep going
}


int test_rtree()
{
    //typedef RTree<ValueType, int, 2, float> MyTree;
    RTree2i tree;

    int i, nhits;
    cout << "nrects = " << nrects << "\n";

    for(i=0; i<nrects; i++)
    {
        tree.Insert(rects[i].min, rects[i].max, i); // Note, all values including zero are fine in this version
    }

    nhits = tree.Search(search_rect.min, search_rect.max, MySearchCallback, NULL);

    cout << "Search resulted in " << nhits << " hits\n";

    // Iterator test
    int itIndex = 0;
    RTree2i::Iterator it;
    for( tree.GetFirst(it);
        !tree.IsNull(it);
        tree.GetNext(it) )
    {
        int value = tree.GetAt(it);

        int boundsMin[2] = {0,0};
        int boundsMax[2] = {0,0};
        it.GetBounds(boundsMin, boundsMax);
        cout << "it[" << itIndex++ << "] " << value << " = (" << boundsMin[0] << "," << boundsMin[1] << "," << boundsMax[0] << "," << boundsMax[1] << ")\n";
    }

    // Iterator test, alternate syntax
    itIndex = 0;
    tree.GetFirst(it);
    while( !it.IsNull() )
    {
        int value = *it;
        ++it;
        cout << "it[" << itIndex++ << "] " << value << "\n";
    }

    cout << "new test 1: " << endl;

    vector<int> search_out;
    tree.Search(search_rect.min, search_rect.max, search_out);

    for (auto it = search_out.begin(); it != search_out.end(); ++it)
    {
        cout << *it << endl;
    }

    vector<int> search_out2;

    int coord[2] = {15, 15};
    tree.KNN(coord, 5, search_out2);
    cout << "knn: " << coord[0] << ", " << coord[1] << endl; 
    for (auto it = search_out2.begin(); it != search_out2.end(); ++it)
    {
        cout << *it << endl;
    }

    search_out2.clear();
    coord[0] = 5;
    coord[1] = 5;
    tree.KNN(coord, 5, search_out2);
    cout << "knn: " << coord[0] << ", " << coord[1] << endl; 
    for (auto it = search_out2.begin(); it != search_out2.end(); ++it)
    {
        cout << *it << endl;
    }

    search_out2.clear();
    coord[0] = 0;
    coord[1] = 0;
    tree.KNN(coord, 5, search_out2);
    cout << "knn: " << coord[0] << ", " << coord[1] << endl; 
    for (auto it = search_out2.begin(); it != search_out2.end(); ++it)
    {
        cout << *it << endl;
    }

    search_out2.clear();
    coord[0] = -5;
    coord[1] = 5;
    tree.KNN(coord, 5, search_out2);
    cout << "knn: " << coord[0] << ", " << coord[1] << endl; 
    for (auto it = search_out2.begin(); it != search_out2.end(); ++it)
    {
        cout << *it << endl;
    }

    tree.Save("test.rtree");

    return 0;

    // Output:
    //
    // nrects = 4
    // Hit data rect 1
    // Hit data rect 2
    // Search resulted in 2 hits
    // it[0] 0 = (0,0,2,2)
    // it[1] 1 = (5,5,7,7)
    // it[2] 2 = (8,5,9,6)
    // it[3] 3 = (7,1,9,2)
    // it[0] 0
    // it[1] 1
    // it[2] 2
    // it[3] 3
}

struct Point
{
    double lon;
    double lat;
    string name;
    Point(double lon = .0, double lat = .0, string name = ""):lon(lon),lat(lat),name(name){}
};

int test_cluster()
{
    typedef Cluster<shared_ptr<Point>> MyCluster;

    MyCluster cluster(0, 5);
    vector<shared_ptr<Point>> points;
    vector<MyCluster::ClusterPointPtr> cpoints;
    int scale = 2;

    for(int i = -180*scale; i < 180*scale; ++i)
    {
        for(int j = -85*scale; j < 85*scale; ++j)
        {
            auto point = make_shared<Point>(i*1./scale, j*1./scale, "");
            //Point point(i*1./scale, j*1./scale);
            //point->name = to_string<string>(point->lon) + "_" + to_string<string>(point->lat);
            //point.name = to_string<string>(point.lon) + "_" + to_string<string>(point.lat);
            points.push_back(point);
        }
    }

    cout << points.size() << endl;

    //ofstream out0("src0.csv");
    //ofstream out1("src1.csv");
    for(auto it = points.begin(); it != points.end(); ++it)
    {
        auto point = *it;
        auto cpoint = cluster.create_cluster_point(point->lon, point->lat, true);
        cpoint->data = point;
        cpoints.push_back(cpoint);
        //out0 << point->lon << "," << point->lat << endl;
        //coord2d co = cluster.get_cluster_point_coord(cpoint, true);
        //out1 << co.x << "," << co.y << endl;
    }
    //out0.close();
    //out1.close();

    points.clear();

    cluster.load(cpoints);
    cpoints.clear();

    for(int i = cluster.min_level; i <= cluster.max_level+1; ++i)
    {
        cout << "level " << i << ": " << cluster.get_clusters_num(i) << endl;

        ofstream tree_point_out("vector_center_" + to_string<string>(i) + ".csv");
        ofstream tree_point_out2("vector_centroid_" + to_string<string>(i) + ".csv");
        auto tree = cluster._rtrees[i];
        MyCluster::ClusterRTree::Iterator it;
        int count = 0;
        for(tree->GetFirst(it); !tree->IsNull(it); tree->GetNext(it))
        {
            auto p = tree->GetAt(it);
            coord2d co = cluster.get_cluster_center_coord(p);
            coord2d co2 = cluster.get_cluster_centroid_coord(p);
            tree_point_out << co.x << "," << co.y << "," << p->count << endl;
            tree_point_out2 << co2.x << "," << co2.y << "," << p->count << endl;
            count += p->count;
        }
        cout << "point count: " << count << endl;
        tree_point_out.close();
        tree_point_out2.close();
    }
    return 0;
    auto tree = cluster._rtrees[1];

    cout << tree->Count() << endl;

    coord3i tile(0,0,0);
    vector<MyCluster::ClusterPointPtr> result;
    //cluster.get_clusters(tile, result);
    double min[2] = {0.5, 0.5};
    double max[2] = {1., 1.};
    tree->Search(min, max, result);

    cout << result.size() << endl;
    for(auto it = result.begin(); it != result.end(); ++it)
    {
        auto cpoint = *it;
        coord2d co = cluster.get_cluster_centroid_coord(cpoint, true);
        cout << "centroid: " << co.x << "," << co.y << endl;
        co = cluster.get_cluster_center_coord(cpoint, true);
        cout << "center: " << co.x << "," << co.y << endl;
        cout << "cluster num: " << cpoint->count << endl;
    }



    return 0;
}


int main()
{
    //test_rtree();
    test_cluster();
    return 0;
}