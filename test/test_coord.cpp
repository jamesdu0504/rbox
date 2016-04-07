
#include <coord.hpp>

#include <iostream>
#include <cassert>

using namespace std;
using namespace rbox;

int main()
{
    coord2i c1{ 1, 2 };
    coord2i c2{ 3, 4 };
    coord2i c3 = c1;
    c1 += c2;
    c3 = c3 + c2;
    assert(c1 == c3);
    c1 -= c2;
    c3 = c3 - c2;
    assert(c1 == c3);
    c1 *= 2;
    c3 = c3 * 2;
    assert(c1 == c3);
    c1 /= 2;
    c3 = c3 / 2;
    assert(c1 == c3);

    return 0;
}