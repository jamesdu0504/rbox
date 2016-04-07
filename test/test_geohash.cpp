#include <algorithm/geohash.hpp>

#include <iostream>
#include <vector>
#include <cassert>

using namespace std;
using namespace rbox;
using namespace geohash;

void test_bbox1() {
    bounding_box b1{ 100, 20, 50, 40 };
    bounding_box b2{ 50, 20, 100, 40 };
    assert(b1 == b2);
}

// Merge
void test_bbox2() {
    bounding_box b1{ 12, 56, 34, 78 };
    bounding_box b2{ 34, 78, 56, 90 };
    bounding_box b3{ 12, 56, 56, 90 };
    bounding_box b4{ 13, 57, 35, 79 };
    assert(b3 == merge(b1, b2));
    assert(b3 != merge(b1, b4));
    assert(b1.merge(b2) == b3);
    assert(b3 == merge(merge(b1, b2), b3));
}

void test_morton_code_bits() {
    morton_code m = morton_code("11100110011110000011101110100110001");
    assert(m.test(1));
    assert(m.test(2));
    assert(m.test(3));
    assert(!m.test(4));
    assert(!m.test(5));
}

void test_common_encode() {
    coord2d c{ 121.473, 31.23 };
    assert(common_encode(c, 1) == morton_code("1"));
    assert(common_encode(c, 2) == morton_code("11"));
    assert(common_encode(c, 3) == morton_code("111"));
    assert(common_encode(c, 4) == morton_code("1110"));
    assert(common_encode(c, 5) == morton_code("11100"));
    assert(common_encode(c, 6) == morton_code("111001"));
    assert(common_encode(c, 7) == morton_code("1110011"));
    assert(common_encode(c, 8) == morton_code("11100110"));
    assert(common_encode(c, 9) == morton_code("111001100"));
    assert(common_encode(c, 10) == morton_code("1110011001"));
    assert(common_encode(c, 11) == morton_code("11100110011"));
    assert(common_encode(c, 12) == morton_code("111001100111"));
    assert(common_encode(c, 13) == morton_code("1110011001111"));
    assert(common_encode(c, 14) == morton_code("11100110011110"));
    assert(common_encode(c, 15) == morton_code("111001100111100"));
    assert(common_encode(c, 16) == morton_code("1110011001111000"));
    assert(common_encode(c, 17) == morton_code("11100110011110000"));
    assert(common_encode(c, 18) == morton_code("111001100111100000"));
    assert(common_encode(c, 19) == morton_code("1110011001111000001"));
    assert(common_encode(c, 20) == morton_code("11100110011110000011"));
    assert(common_encode(c, 21) == morton_code("111001100111100000111"));
    assert(common_encode(c, 22) == morton_code("1110011001111000001111"));
    assert(common_encode(c, 23) == morton_code("11100110011110000011110"));
    assert(common_encode(c, 24) == morton_code("111001100111100000111100"));
    assert(common_encode(c, 25) == morton_code("1110011001111000001111000"));
    assert(common_encode(c, 26) == morton_code("11100110011110000011110001"));
    assert(common_encode(c, 27) == morton_code("111001100111100000111100010"));
    assert(common_encode(c, 28) == morton_code("1110011001111000001111000100"));
    assert(common_encode(c, 29) == morton_code("11100110011110000011110001000"));
    assert(common_encode(c, 30) == morton_code("111001100111100000111100010001"));
    assert(common_encode(c, 31) == morton_code("1110011001111000001111000100011"));
    assert(common_encode(c, 32) == morton_code("11100110011110000011110001000110"));
    assert(common_encode(c, 33) == morton_code("111001100111100000111100010001100"));
    assert(common_encode(c, 34) == morton_code("1110011001111000001111000100011000"));
    assert(common_encode(c, 35) == morton_code("11100110011110000011110001000110001"));
    assert(common_encode(c, 36) == morton_code("111001100111100000111100010001100011"));
    assert(common_encode(c, 37) == morton_code("1110011001111000001111000100011000111"));
    assert(common_encode(c, 38) == morton_code("11100110011110000011110001000110001111"));
    assert(common_encode(c, 39) == morton_code("111001100111100000111100010001100011111"));
    assert(common_encode(c, 40) == morton_code("1110011001111000001111000100011000111111"));
    assert(common_encode(c, 41) == morton_code("11100110011110000011110001000110001111111"));
    assert(common_encode(c, 42) == morton_code("111001100111100000111100010001100011111111"));
    assert(common_encode(c, 43) == morton_code("1110011001111000001111000100011000111111111"));
    assert(common_encode(c, 44) == morton_code("11100110011110000011110001000110001111111111"));
    assert(common_encode(c, 45) == morton_code("111001100111100000111100010001100011111111110"));
    assert(common_encode(c, 46) == morton_code("1110011001111000001111000100011000111111111101"));
    assert(common_encode(c, 47) == morton_code("11100110011110000011110001000110001111111111010"));
    assert(common_encode(c, 48) == morton_code("111001100111100000111100010001100011111111110100"));
    assert(common_encode(c, 49) == morton_code("1110011001111000001111000100011000111111111101000"));
    assert(common_encode(c, 50) == morton_code("11100110011110000011110001000110001111111111010001"));
    assert(common_encode(c, 51) == morton_code("111001100111100000111100010001100011111111110100010"));
    assert(common_encode(c, 52) == morton_code("1110011001111000001111000100011000111111111101000101"));
    assert(common_encode(c, 53) == morton_code("11100110011110000011110001000110001111111111010001010"));
    assert(common_encode(c, 54) == morton_code("111001100111100000111100010001100011111111110100010101"));
    assert(common_encode(c, 55) == morton_code("1110011001111000001111000100011000111111111101000101011"));
    assert(common_encode(c, 56) == morton_code("11100110011110000011110001000110001111111111010001010111"));
    assert(common_encode(c, 57) == morton_code("111001100111100000111100010001100011111111110100010101111"));
    assert(common_encode(c, 58) == morton_code("1110011001111000001111000100011000111111111101000101011111"));
    assert(common_encode(c, 59) == morton_code("11100110011110000011110001000110001111111111010001010111110"));
    assert(common_encode(c, 60) == morton_code("111001100111100000111100010001100011111111110100010101111100"));
    assert(common_encode(c, 61) == morton_code("1110011001111000001111000100011000111111111101000101011111001"));
    assert(common_encode(c, 62) == morton_code("11100110011110000011110001000110001111111111010001010111110010"));
    assert(common_encode(c, 63) == morton_code("111001100111100000111100010001100011111111110100010101111100101"));
    assert(common_encode(c, 64) == morton_code("1110011001111000001111000100011000111111111101000101011111001011"));
}

void test_fast_encode() {
    coord2d c{ 121.473, 31.23 };
    assert(fast_encode(c, 1) == morton_code("1"));
    assert(fast_encode(c, 2) == morton_code("11"));
    assert(fast_encode(c, 3) == morton_code("111"));
    assert(fast_encode(c, 4) == morton_code("1110"));
    assert(fast_encode(c, 5) == morton_code("11100"));
    assert(fast_encode(c, 6) == morton_code("111001"));
    assert(fast_encode(c, 7) == morton_code("1110011"));
    assert(fast_encode(c, 8) == morton_code("11100110"));
    assert(fast_encode(c, 9) == morton_code("111001100"));
    assert(fast_encode(c, 10) == morton_code("1110011001"));
    assert(fast_encode(c, 11) == morton_code("11100110011"));
    assert(fast_encode(c, 12) == morton_code("111001100111"));
    assert(fast_encode(c, 13) == morton_code("1110011001111"));
    assert(fast_encode(c, 14) == morton_code("11100110011110"));
    assert(fast_encode(c, 15) == morton_code("111001100111100"));
    assert(fast_encode(c, 16) == morton_code("1110011001111000"));
    assert(fast_encode(c, 17) == morton_code("11100110011110000"));
    assert(fast_encode(c, 18) == morton_code("111001100111100000"));
    assert(fast_encode(c, 19) == morton_code("1110011001111000001"));
    assert(fast_encode(c, 20) == morton_code("11100110011110000011"));
    assert(fast_encode(c, 21) == morton_code("111001100111100000111"));
    assert(fast_encode(c, 22) == morton_code("1110011001111000001111"));
    assert(fast_encode(c, 23) == morton_code("11100110011110000011110"));
    assert(fast_encode(c, 24) == morton_code("111001100111100000111100"));
    assert(fast_encode(c, 25) == morton_code("1110011001111000001111000"));
    assert(fast_encode(c, 26) == morton_code("11100110011110000011110001"));
    assert(fast_encode(c, 27) == morton_code("111001100111100000111100010"));
    assert(fast_encode(c, 28) == morton_code("1110011001111000001111000100"));
    assert(fast_encode(c, 29) == morton_code("11100110011110000011110001000"));
    assert(fast_encode(c, 30) == morton_code("111001100111100000111100010001"));
    assert(fast_encode(c, 31) == morton_code("1110011001111000001111000100011"));
    assert(fast_encode(c, 32) == morton_code("11100110011110000011110001000110"));
    assert(fast_encode(c, 33) == morton_code("111001100111100000111100010001100"));
    assert(fast_encode(c, 34) == morton_code("1110011001111000001111000100011000"));
    assert(fast_encode(c, 35) == morton_code("11100110011110000011110001000110001"));
    assert(fast_encode(c, 36) == morton_code("111001100111100000111100010001100011"));
    assert(fast_encode(c, 37) == morton_code("1110011001111000001111000100011000111"));
    assert(fast_encode(c, 38) == morton_code("11100110011110000011110001000110001111"));
    assert(fast_encode(c, 39) == morton_code("111001100111100000111100010001100011111"));
    assert(fast_encode(c, 40) == morton_code("1110011001111000001111000100011000111111"));
    assert(fast_encode(c, 41) == morton_code("11100110011110000011110001000110001111111"));
    assert(fast_encode(c, 42) == morton_code("111001100111100000111100010001100011111111"));
    assert(fast_encode(c, 43) == morton_code("1110011001111000001111000100011000111111111"));
    assert(fast_encode(c, 44) == morton_code("11100110011110000011110001000110001111111111"));
    assert(fast_encode(c, 45) == morton_code("111001100111100000111100010001100011111111110"));
    assert(fast_encode(c, 46) == morton_code("1110011001111000001111000100011000111111111101"));
    assert(fast_encode(c, 47) == morton_code("11100110011110000011110001000110001111111111010"));
    assert(fast_encode(c, 48) == morton_code("111001100111100000111100010001100011111111110100"));
    assert(fast_encode(c, 49) == morton_code("1110011001111000001111000100011000111111111101000"));
    assert(fast_encode(c, 50) == morton_code("11100110011110000011110001000110001111111111010001"));
    assert(fast_encode(c, 51) == morton_code("111001100111100000111100010001100011111111110100010"));
    assert(fast_encode(c, 52) == morton_code("1110011001111000001111000100011000111111111101000101"));
    assert(fast_encode(c, 53) == morton_code("11100110011110000011110001000110001111111111010001010"));
    assert(fast_encode(c, 54) == morton_code("111001100111100000111100010001100011111111110100010101"));
    assert(fast_encode(c, 55) == morton_code("1110011001111000001111000100011000111111111101000101011"));
    assert(fast_encode(c, 56) == morton_code("11100110011110000011110001000110001111111111010001010111"));
    assert(fast_encode(c, 57) == morton_code("111001100111100000111100010001100011111111110100010101111"));
    assert(fast_encode(c, 58) == morton_code("1110011001111000001111000100011000111111111101000101011111"));
    assert(fast_encode(c, 59) == morton_code("11100110011110000011110001000110001111111111010001010111110"));
    assert(fast_encode(c, 60) == morton_code("111001100111100000111100010001100011111111110100010101111100"));
    //assert(fast_encode(c, 61) == morton_code("1110011001111000001111000100011000111111111101000101011111001"));
    //assert(fast_encode(c, 62) == morton_code("11100110011110000011110001000110001111111111010001010111110010"));
    //assert(fast_encode(c, 63) == morton_code("111001100111100000111100010001100011111111110100010101111100101"));
    //assert(fast_encode(c, 64) == morton_code("1110011001111000001111000100011000111111111101000101011111001011"));
}

void test_common_decode() {
    assert(common_decode(morton_code::from_geohash("w")).contains(coord2d{ 113, 21}));
    assert(common_decode(morton_code("11100")).contains(coord2d{ 113, 21 }));
    assert(common_decode(morton_code::from_geohash("wt")).contains(coord2d{ 118, 30.9 }));
    assert(common_decode(morton_code("1110011001")).contains(coord2d{ 118, 30.9 }));
    assert(common_decode(morton_code::from_geohash("wtw")).contains(coord2d{ 121.6, 31.6 }));
    assert(common_decode(morton_code("111001100111100")).contains(coord2d{ 121.6, 31.6 }));
    assert(common_decode(morton_code::from_geohash("wtw3")).contains(coord2d{ 121.46, 31.2 }));
    assert(common_decode(morton_code("11100110011110000011")).contains(coord2d{ 121.46, 31.2 }));
    assert(common_decode(morton_code::from_geohash("wtw3r")).contains(coord2d{ 121.619, 31.179 }));
    assert(common_decode(morton_code("1110011001111000001110111")).contains(coord2d{ 121.619, 31.179 }));
    assert(common_decode(morton_code::from_geohash("wtw3r9")).contains(coord2d{ 121.624, 31.1655 }));
    assert(common_decode(morton_code("111001100111100000111011101001")).contains(coord2d{ 121.624, 31.1655 }));
    assert(common_decode(morton_code::from_geohash("wtw3r9j")).contains(coord2d{ 121.6262, 31.1634 }));
    assert(common_decode(morton_code("11100110011110000011101110100110001")).contains(coord2d{ 121.6262, 31.1634 }));
    assert(common_decode(morton_code::from_geohash("wtw3r9jj")).contains(coord2d{ 121.62569, 31.16366 }));
    assert(common_decode(morton_code("1110011001111000001110111010011000110001")).contains(coord2d{ 121.62569, 31.16366 }));
    assert(common_decode(morton_code::from_geohash("wtw3r9jjz")).contains(coord2d{ 121.625841, 31.163728 }));
    assert(common_decode(morton_code("111001100111100000111011101001100011000111111")).contains(coord2d{ 121.625841, 31.163728 }));
    assert(common_decode(morton_code::from_geohash("wtw3r9jjzy")).contains(coord2d{ 121.625857, 31.1637416 }));
    assert(common_decode(morton_code("11100110011110000011101110100110001100011111111110")).contains(coord2d{ 121.625857, 31.1637416 }));
    assert(common_decode(morton_code::from_geohash("wtw3r9jjzyj")).contains(coord2d{ 121.6258588, 31.1637396 }));
    assert(common_decode(morton_code("1110011001111000001110111010011000110001111111111010001")).contains(coord2d{ 121.6258588, 31.1637396 }));
    assert(common_decode(morton_code::from_geohash("wtw3r9jjzyjc")).contains(coord2d{ 121.62585927, 31.16373922 }));
    assert(common_decode(morton_code("111001100111100000111011101001100011000111111111101000101011")).contains(coord2d{ 121.62585927, 31.16373922 }));
    assert(!common_decode(morton_code::from_geohash("wtw3r9jjzyjc")).contains(coord2d{ 121.62585927, 31.16374922 }));
}

void test_fast_decode() {
    assert(fast_decode(morton_code::from_geohash("w")).contains(coord2d{ 113, 21 }));
    assert(fast_decode(morton_code("11100")).contains(coord2d{ 113, 21 }));
    assert(fast_decode(morton_code::from_geohash("wt")).contains(coord2d{ 118, 30.9 }));
    assert(fast_decode(morton_code("1110011001")).contains(coord2d{ 118, 30.9 }));
    assert(fast_decode(morton_code::from_geohash("wtw")).contains(coord2d{ 121.6, 31.6 }));
    assert(fast_decode(morton_code("111001100111100")).contains(coord2d{ 121.6, 31.6 }));
    assert(fast_decode(morton_code::from_geohash("wtw3")).contains(coord2d{ 121.46, 31.2 }));
    assert(fast_decode(morton_code("11100110011110000011")).contains(coord2d{ 121.46, 31.2 }));
    assert(fast_decode(morton_code::from_geohash("wtw3r")).contains(coord2d{ 121.619, 31.179 }));
    assert(fast_decode(morton_code("1110011001111000001110111")).contains(coord2d{ 121.619, 31.179 }));
    assert(fast_decode(morton_code::from_geohash("wtw3r9")).contains(coord2d{ 121.624, 31.1655 }));
    assert(fast_decode(morton_code("111001100111100000111011101001")).contains(coord2d{ 121.624, 31.1655 }));
    assert(fast_decode(morton_code::from_geohash("wtw3r9j")).contains(coord2d{ 121.6262, 31.1634 }));
    assert(fast_decode(morton_code("11100110011110000011101110100110001")).contains(coord2d{ 121.6262, 31.1634 }));
    assert(fast_decode(morton_code::from_geohash("wtw3r9jj")).contains(coord2d{ 121.62569, 31.16366 }));
    assert(fast_decode(morton_code("1110011001111000001110111010011000110001")).contains(coord2d{ 121.62569, 31.16366 }));
    assert(fast_decode(morton_code::from_geohash("wtw3r9jjz")).contains(coord2d{ 121.625841, 31.163728 }));
    assert(fast_decode(morton_code("111001100111100000111011101001100011000111111")).contains(coord2d{ 121.625841, 31.163728 }));
    assert(fast_decode(morton_code::from_geohash("wtw3r9jjzy")).contains(coord2d{ 121.625857, 31.1637416 }));
    assert(fast_decode(morton_code("11100110011110000011101110100110001100011111111110")).contains(coord2d{ 121.625857, 31.1637416 }));
    assert(fast_decode(morton_code::from_geohash("wtw3r9jjzyj")).contains(coord2d{ 121.6258588, 31.1637396 }));
    assert(fast_decode(morton_code("1110011001111000001110111010011000110001111111111010001")).contains(coord2d{ 121.6258588, 31.1637396 }));
    assert(fast_decode(morton_code::from_geohash("wtw3r9jjzyjc")).contains(coord2d{ 121.62585927, 31.16373922 }));
    assert(fast_decode(morton_code("111001100111100000111011101001100011000111111111101000101011")).contains(coord2d{ 121.62585927, 31.16373922 }));
    assert(!fast_decode(morton_code::from_geohash("wtw3r9jjzyjc")).contains(coord2d{ 121.62585927, 31.16374922 }));
}

void test_morton_code_neighbor() {
    morton_code m("11100110");
    assert(neighbor(m, { -1, -1 }) == morton_code("11100001"));
    assert(neighbor(m, { -1,  0 }) == morton_code("11100100"));
    assert(neighbor(m, { -1,  1 }) == morton_code("11100101"));
    assert(neighbor(m, { 0, -1 }) == morton_code("11100011"));
    assert(neighbor(m, { 0,  1 }) == morton_code("11100111"));
    assert(neighbor(m, { 1, -1 }) == morton_code("11101001"));
    assert(neighbor(m, { 1,  0 }) == morton_code("11101100"));
    assert(neighbor(m, { 1,  1 }) == morton_code("11101101"));
}

void test_encode() {
    coord2d c{ 121.62585927, 31.16373922 };
    assert(encode(c, 1) == "w");
    assert(encode(c, 2) == "wt");
    assert(encode(c, 3) == "wtw");
    assert(encode(c, 4) == "wtw3");
    assert(encode(c, 5) == "wtw3r");
    assert(encode(c, 6) == "wtw3r9");
    assert(encode(c, 7) == "wtw3r9j");
    assert(encode(c, 8) == "wtw3r9jj");
    assert(encode(c, 9) == "wtw3r9jjz");
    assert(encode(c, 10) == "wtw3r9jjzy");
    assert(encode(c, 11) == "wtw3r9jjzyj");
    assert(encode(c, 12) == "wtw3r9jjzyjc");
}

void test_decode() {
    assert(decode("w").contains(coord2d{ 113, 21 }));
    assert(decode("wt").contains(coord2d{ 118, 30.9 }));
    assert(decode("wtw").contains(coord2d{ 121.6, 31.6 }));
    assert(decode("wtw3").contains(coord2d{ 121.46, 31.2 }));
    assert(decode("wtw3r").contains(coord2d{ 121.619, 31.179 }));
    assert(decode("wtw3r9").contains(coord2d{ 121.624, 31.1655 }));
    assert(decode("wtw3r9j").contains(coord2d{ 121.6262, 31.1634 }));
    assert(decode("wtw3r9jj").contains(coord2d{ 121.62569, 31.16366 }));
    assert(decode("wtw3r9jjz").contains(coord2d{ 121.625841, 31.163728 }));
    assert(decode("wtw3r9jjzy").contains(coord2d{ 121.625857, 31.1637416 }));
    assert(decode("wtw3r9jjzyj").contains(coord2d{ 121.6258588, 31.1637396 }));
    assert(decode("wtw3r9jjzyjc").contains(coord2d{ 121.62585927, 31.16373922 }));
    assert(!decode("wtw3r9jjzyjc").contains(coord2d{ 121.62585927, 31.16374922 }));
}

void test_neighbor() {
    assert(neighbor("wtw3s", { -1, -1 }) == "wtw37");
    assert(neighbor("wtw3s", { -1, 0 }) == "wtw3e");
    assert(neighbor("wtw3s", { -1, 1 }) == "wtw3g");
    assert(neighbor("wtw3s", { 0, -1 }) == "wtw3k");
    assert(neighbor("wtw3s", { 0, 1 }) == "wtw3u");
    assert(neighbor("wtw3s", { 1, -1 }) == "wtw3m");
    assert(neighbor("wtw3s", { 1, 0 }) == "wtw3t");
    assert(neighbor("wtw3s", { 1, 1 }) == "wtw3v");
    assert(neighbor("wtw3sjj", { -1, -1 }) == "wtw3shu");
    assert(neighbor("wtw3sjj", { -1, 0 }) == "wtw3sjh");
    assert(neighbor("wtw3sjj", { -1, 1 }) == "wtw3sjk");
    assert(neighbor("wtw3sjj", { 0, -1 }) == "wtw3shv");
    assert(neighbor("wtw3sjj", { 0, 1 }) == "wtw3sjm");
    assert(neighbor("wtw3sjj", { 1, -1 }) == "wtw3shy");
    assert(neighbor("wtw3sjj", { 1, 0 }) == "wtw3sjn");
    assert(neighbor("wtw3sjj", { 1, 1 }) == "wtw3sjq");
}

int main() {
    test_bbox1();
    test_bbox2();
    test_morton_code_bits();
    test_common_encode();
    test_fast_encode();
    test_common_decode();
    test_fast_decode();
    test_morton_code_neighbor();
    test_encode();
    test_decode();
    test_neighbor();
    return 0;
}