//
// test.cpp
//

#include <string>
#include <iostream>
#include <memory>
#include <fstream>
#include <cassert>

#include <rtree.hpp>

using namespace std;
using namespace rbox;

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


int main()
{
    //typedef RTree<ValueType, int, 2, float> MyTree;
    RTree2i tree;

    int i, nhits;
    assert(nrects == 4);

    for(i=0; i<nrects; i++)
    {
        tree.Insert(rects[i].min, rects[i].max, i); // Note, all values including zero are fine in this version
    }

    nhits = tree.Search(search_rect.min, search_rect.max, MySearchCallback, NULL);

    assert(nhits == 2);

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

    vector<int> search_out;
    tree.Search(search_rect.min, search_rect.max, search_out);

    assert(search_out.size() == 2);
    assert(search_out[0] == 1);
    assert(search_out[1] == 2);

    return 0;

}
