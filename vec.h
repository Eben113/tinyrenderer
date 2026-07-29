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
    double& operator[](const int i) const;
};

template<int n> std::ostream& operator<<(std::ostream& out, vec<n> v);

template<>
struct vec<3>{
    double x = 0, y = 0, z = 0;
    vec<3>& operator+=(const vec<3> a){
    x += a.x; y += a.y; z += a.z;
    return *this;
    }
    vec<3> operator-(){
        return vec<3>{-x, -y, -z};
    }
    vec<3> operator*(double a) const{
        return vec<3>{a*x, a*y, a*z};
    }
    vec<3> operator*(vec<3>& a){
        return vec<3>{x*a.x, y*a.y, z*a.z};
    }
    vec<3>& operator*=(vec<3>& a){
        x*=a.x; y*=a.y; z*=a.z;
        return *this;
    }
    int dot(vec<3> a){
        return x*a.x + y*a.y +z*a.z;
    }
    double& operator[](const int i){assert(i>=0 && i<3); return i? (i==1? y : z): x;};
    double operator[](const int i)const{assert(i>=0 && i<3); return i? (i==1? y : z): x;};
};

vec<3> operator+(vec<3> a, vec<3> b){
    return a += b;
}
vec<3> operator-(vec<3> a, vec<3> b){
    return a += (-b);
}
vec<3> operator*(double a, vec<3>& v){
    return v*a;
}
vec<3> operator*(vec<3>& a, vec<3>& v){
    return vec<3>{a.x*v.x, a.y*v.y, a.z*v.z};
}


template<>
struct vec<2>{
    double x = 0, y = 0;
    vec<2>& operator +=(const vec<2> a);
    vec<2>& operator -();
    vec<2> operator*(double a) const;
    vec<2>& dot(vec<2> a);
    double& operator[](const int i);
    double operator[](const int i)const;
};

template<>
struct vec<4>{
    double x = 0, y = 0, z = 0;
    vec<4>& operator +=(const vec<4> a);
    vec<4>& operator -();
    vec<4> operator*(double a) const;
    vec<4>& dot(vec<4> a);
    double& operator[](const int i);
    double operator[](const int i)const;
};


template<int n> 
 struct squareMatrix {
    vec<n> data[n] = {vec<n>{}};
    squareMatrix<n>& squareMatrix<n>::operator+=(const squareMatrix<n> a){
        for(int i=0; i<n; i++){data[i]+=a.data[i];}
        return *this;}

    squareMatrix<n> squareMatrix<n>::operator-() const{
        squareMatrix result;
        for(int i=0; i<n; i++){result[i] = -data[i];}
        return result;}
    
    vec<n>& operator[](const int i){assert(i>=0 && i<n); return data[i]};
    vec<n> operator[](const int i) const{assert(i>=0 && i<n); return data[i]};

    squareMatrix<n> squareMatrix<n>::T(0 const{
        squareMatrix result;
        for(int i = 0; i<n; i++){
            for(int j = 0; j<n; j++)
            result[i][j] = data[j][i];
        }
    })
    squareMatrix<n> squareMatrix<n>::operator*(squareMatrix<n> b){
        squareMatrix result{};
        squarematrix T = b.T()
        for(int i = 0; i< n; i++){
            for(int j = 0; j< n;  j++){
                result[i][j] = this->data[i].dot(T[j]);
            }
        }
        return result;
    }
};

template<>
struct squareMatrix<3>{
    squareMatrix<3>(vec<3> a, vec<3> b, vec<3> c){
        data[0] = a;
        data[1] = b;
        data[2] = c;
    };

    int squareMatrix<3>::coFactor(int x, int y){
        int xa = (x+1)%3, xb = (x+2)%3, ya = (y+1)%3, yb = (y+2)%3;
    }
}


