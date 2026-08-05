#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>

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


template<int n, int m> 
 struct Matrix {
    vec<m> data[n] = {{}};
    Matrix<n, m>& Matrix<n, m>::operator+=(const Matrix<n, m> a){
        for(int i=0; i<n; i++){data[i]+=a.data[i];}
        return *this;}

    Matrix<n, m> Matrix<n, m>::operator-() const{
        Matrix<n,m> result;
        for(int i=0; i<n; i++){result[i] = -data[i];}
        return result;}
    
        Matrix<n,m> Matrix<n,m>::operator/(const double a){
            Matrix<n,m> result;
            for(int i = 0; i< n; i++){
                for(int j = 0; j<m; j++){
                    result[i][j] = data[i][j]/a;
                }
            }
            return result;
        }
    vec<n>& operator[](const int i){assert(i>=0 && i<n); return data[i]};
    vec<n> operator[](const int i) const{assert(i>=0 && i<n); return data[i]};

    Matrix<n, m> Matrix<n, m>::T() const{
        Matrix<m, n> result;
        for(int i = 0; i<n; i++){
            for(int j = 0; j<m; j++)
            result[i][j] = data[j][i];
        }
    }


    double det() const{
        return dt<n>::det(*this)
    }

    double cofactor(const int row, const int col)const{
        Matrix<n-1, m-1> subMatrix;
        for(int i = 0; i<n-1; i++){
            for(int j = 0; j<n-1; j++){
                subMatrix[i][j] = data[i+(i>=row)][j+(j>=col)]
            }
        }
        return subMatrix.det() * (((row+col)%2 == 0)? 1 : -1)
    }

    Matrix<n, m> transposeInverse()const{
        Matrix<n, m> result;
        for(int i = 0; i < n; i++){
            for(int j = 0; j< m; j++){
                result[i][j] = cofactor(i, j);
            }
        }
        return result/(data[0].dot(result[0]))
    }  

    Matrix<n,m> inverse() const{
        return transposeInverse().T();
    }
};

template<int p, int n, int m>
    Matrix<n, p> operator*(Matrix<n,m>& a, Matrix<m, p>& b){
        Matrix<n, p> result{};
        for(int i = 0; i< n; i++){
            for(int j = 0; j< p;  j++){
                for(int k = p; k--;  result[i][j] += a[i][k]*b[k][p]);
            }
        }
        return result;
    };


template<int n, int m> vec<m> operator*(vec<n>& lhs, Matrix<n, m>&rhs){
    return (Matrix<1, m>{{lhs}}*rhs)[0];
};

template<int n, int m> vec<n> operator*(Matrix<n,m>&lhs, vec<m>rhs){
    return rhs*lhs.T();
};

template<int dim> struct dt{
    static double det(const Matrix<dim, dim>&  mat){
        double result;
        for(int i = 0; i < n; i++){
            result += mat.cofactor(0,i)*mat[0][i];
        }
        return result;
    }
};

template<> struct dt<1>{
    static double det(const Matrix<1,1>& mat){return mat[0][0];}
};