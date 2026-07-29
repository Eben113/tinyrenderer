#include <iostream>
#include <cmath>
#include <cassert>
#include "vec.h"



//non-member functions
vec<3> operator+(vec<3> a, vec<3> b){
    return a += b;
}
vec<3> operator-(vec<3> a, vec<3> b){
    return a += (-b);
}
vec<3> operator*(double a, vec<3>& v){
    return vec<3>{a*v.x, a*v.y, a*v.z};
}



