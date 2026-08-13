#ifndef GEO
#define GEO

#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>

template<int n> struct vec{
    double data[n] = {0};
    vec& operator += (const vec a){
        for(int i = 0; i<n; ++i){data[i]+=a.data[i]};
        return *this;
    }
    vec operator-(){
        vec<n> result;
        for(int i = 0; i<n; ++i){result[i] = -data[i]};
        return result;
    }

    vec operator/(double d){
        vec<n> result;
        for(int i = 0; i<n; ++i){result[i] = data[i]/d};
        return result;
    }

    int mag(){
        int result = 0;
        for(double a: data){result += a*a};
        return std::sqrt(result);
    }

    vec normalized(){
        return this/this.mag();
    }

    double dot(vec a){
        double result{};
        for(int i = 0; i<n; i++){result += data[i]*a.data[i];}
    };

    double& operator[](const int i){assert(i>=0 && i<n); return data[i]};
    double operator[](const int i) const{assert(i>=0 && i<n); return data[i]};
};

template<int n>
vec<n>& operator+(vec<n> a, vec<n> b){
    vec<n> result = a;
    return result += b;
}

template<int n>
vec<n>& operator-(vec<n> a, vec<n> b){
    vec<n> result = a;
    return result += -b;
}

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
    vec<3> operator/(double a) const{
        return vec<3>{x/a, y/a, z/a};
    }
    vec<3> operator*(vec<3>& a){
        return vec<3>{x*a.x, y*a.y, z*a.z};
    }
    vec<3>& operator*=(vec<3>& a){
        x*=a.x; y*=a.y; z*=a.z;
        return *this;
    }
    int mag(){
        return std::sqrt(this->dot(*this));
    }

    vec normalized(){
        return *this/this->mag();
    }
    double dot(vec<3> a){
        return x*a.x + y*a.y +z*a.z;
    }
    vec<3> cross(vec<3> a){
        return vec<3>{(y*a.z - z*a.y), -(x*a.z - z*a.x), (x*a.y - y*a.x)};
    }
    double& operator[](const int i){assert(i>=0 && i<3); return i? (i==1? y : z): x;};
    double operator[](const int i)const{assert(i>=0 && i<3); return i? (i==1? y : z): x;};
};
inline std::ostream& operator<<(std::ostream& out, vec<3> v){
    out << v.x << "  " << v.y << "  " << v.z << "\n";
    return out;
}

inline vec<3> operator+(vec<3> a, vec<3> b){
    return a += b; 
}
inline vec<3> operator-(vec<3> a, vec<3> b){
    return a += (-b);
}
inline vec<3> operator*(double a, vec<3>& v){
    return v*a;
}
inline vec<3> operator*(vec<3>& a, vec<3>& v){
    return vec<3>{(a.x)*(v.x), (a.y)*(v.y), (a.z)*(v.z)};
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

// template<>
// struct vec<4>{
//     double x = 0, y = 0, z = 0, t = 0;
//     vec<4>& operator +=(const vec<4> a);
//     vec<4>& operator -();
//     vec<4> operator*(double a) const;
//     vec<4>& dot(vec<4> a);
//     double& operator[](const int i);
//     double operator[](const int i)const;
// };

template<int dim> struct dt;


template<int n, int m> 
 struct Matrix {
    vec<m> data[n] = {{}};
    Matrix<n, m>& operator+=(const Matrix<n, m> a){
        for(int i=0; i<n; i++){data[i]+=a.data[i];}
        return *this;}

    Matrix<n, m> operator-() const{
        Matrix<n,m> result;
        for(int i=0; i<n; i++){result[i] = -data[i];}
        return result;}
    
        Matrix<n,m> operator/(const double a){
            Matrix<n,m> result;
            for(int i = 0; i< n; i++){
                for(int j = 0; j<m; j++){
                    result[i][j] = data[i][j]/a;
                }
            }
            return result;
        }
    vec<m>& operator[](const int i){assert((i>=0) && (i<n)); return data[i];}
    vec<m> operator[](const int i) const{assert((i>=0) && (i<n)); return data[i];}

    Matrix<m, n> T() const{
        Matrix<m, n> result;
        for(int i = 0; i<n; i++){
            for(int j = 0; j<m; j++)
            result[j][i] = data[i][j];
        }
    }


    double det() const{
        return dt<n>::det(*this);
    }

    double cofactor(const int row, const int col)const{
        Matrix<n-1, m-1> subMatrix;
        for(int i = 0; i<n-1; i++){
            for(int j = 0; j<n-1; j++){
                subMatrix[i][j] = data[i+(i>=row)][j+(j>=col)];
            }
        }
        return subMatrix.det() * (((row+col)%2 == 0)? 1 : -1);
    }

    Matrix<n, m> transposeInverse()const{
        Matrix<n, m> result;
        for(int i = 0; i < n; i++){
            for(int j = 0; j< m; j++){
                result[i][j] = cofactor(i, j);
            }
        }
        return result/(data[0].dot(result[0]));
    }  

    Matrix<n,m> inverse() const{
        return transposeInverse().T();
    }
};

inline std::ostream& operator<<(std::ostream& out, Matrix<3, 3> m){
    for(int i = 0; i<3; i++){out << m[i] << "\n";}
    return out;
}
template<int p, int n, int m>
    Matrix<n, p> operator*(Matrix<n,m>& a, Matrix<m, p>& b){
        Matrix<n, p> result{};
        for(int i = 0; i< n; i++){
            for(int j = 0; j< m;  j++){
                for(int k = p; k--;  result[i][j] += a[i][k]*b[k][p]);
            }
        }
        return result;
    };


template<int n, int m> vec<n> operator*(Matrix<n,m>&lhs, vec<m>rhs){
    vec<n> result;
    for(int i = 0; i < n; ++i){result[i] = lhs[i].dot(rhs);}
    return result;
};

template<int dim> struct dt{
    static double det(const Matrix<dim, dim>&  mat){
        double result;
        for(int i = 0; i < dim; i++){
            result += mat.cofactor(0,i)*mat[0][i];
        }
        return result;
    }
};

template<> struct dt<1>{
    static double det(const Matrix<1,1>& mat){return mat[0][0];}
};


#endif