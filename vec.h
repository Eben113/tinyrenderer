#include <iostream>
#include <cmath>
#include <cassert>

template<int n> struct vec{
    double data[n] = {0};
    vec& operator +=(const vec i);
    vec& operator +(const vec i);
    vec operator*(double a) const;
    vec& dot(vec a);
    double& operator[](const int i);
    double operator[](const int i);
};

template<int n> std::ostream& operator<<(std::ostream& out, vec<n> v);

struct vec<3>{
    double x = 0, y = 0, z = 0;
    vec(double a = 0, double b = 0, double c = 0);
    vec<3>& operator +=(const vec<3> a);
    vec<3>& operator -();
    vec<3> operator*(double a) const;
    vec<3>& dot(vec<3> a);
    double& operator[](const int i);
    double operator[](const int i)const;
};

struct vec<2>{
    double x = 0, y = 0;
    vec<2>& operator +=(const vec<2> a);
    vec<2>& operator -();
    vec<2> operator*(double a) const;
    vec<2>& dot(vec<2> a);
    double& operator[](const int i);
    double operator[](const int i)const;
};


struct vec<4>{
    double x = 0, y = 0, z = 0;
    vec<4>& operator +=(const vec<4> a);
    vec<4>& operator -();
    vec<4> operator*(double a) const;
    vec<4>& dot(vec<4> a);
    double& operator[](const int i);
    double operator[](const int i)const;
}


template<int n> struct squareMatrix {
    vec<n> data[n] = {vec<n>{}};
    squareMatrix& operator+=(const squareMatrix a);
    squareMatrix operator-() const;
    squareMatrix operator*(squareMatrix a);
    squareMatrix& T();
    vec<n> operator[](const int i);
    vec<n> operator[](const int i) const;
};