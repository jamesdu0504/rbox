//
// test.cpp
//

#include <string>
#include <iostream>
#include <memory>
#include <fstream>
#include <cassert>

#include <rtree.hpp>

//#include <string_utils/string_utils.hpp>
//using namespace string_utils;

using namespace std;
using namespace rbox;

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


int main()
{
    RTree2i tree;

    for(int i=0; i< sizeof(rects) / sizeof(rects[0]); i++)
    {
        tree.Insert(rects[i].min, rects[i].max, i); // Note, all values including zero are fine in this version
    }


    vector<int> search_out;

    int coord[2] = {15, 15};
    tree.KNN(coord, 2, search_out);
    assert(search_out.size() == 2);
    assert(search_out[0] == 2);
    assert(search_out[1] == 1);

    search_out.clear();
    coord[0] = 5;
    coord[1] = 5;
    tree.KNN(coord, 5, search_out);
    assert(search_out.size() == 4);
    assert(search_out[0] == 1);
    assert(search_out[1] == 2);
    assert(search_out[2] == 3);
    assert(search_out[3] == 0);

    search_out.clear();
    coord[0] = 0;
    coord[1] = 0;
    tree.KNN(coord, 5, search_out);
    assert(search_out.size() == 4);
    assert(search_out[0] == 0);
    assert(search_out[1] == 3);
    assert(search_out[2] == 1);
    assert(search_out[3] == 2);

    search_out.clear();
    coord[0] = -5;
    coord[1] = 5;
    tree.KNN(coord, 4, search_out);
    assert(search_out.size() == 4);
    assert(search_out[0] == 0);
    assert(search_out[1] == 1);
    assert(search_out[2] == 2);
    assert(search_out[3] == 3);

    return 0;

}
