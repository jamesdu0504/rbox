//
// test.cpp
//

#include <string>
#include <iostream>
#include <memory>
#include <fstream>
#include <cassert>

#include <coord.hpp>

using namespace std;
using namespace rbox;

int main()
{
    coord2i c1{ 1, 2 };
    coord2i c2{ 3, 4 };
    c1 += c2;
    coord2i c3 = c1 + c2;
    c3 = c1 - c2;
    c3 = c1 * 2;
    c3 = c3 / 2;
    return 0;
}