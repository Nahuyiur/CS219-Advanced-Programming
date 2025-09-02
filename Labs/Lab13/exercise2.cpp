#include <iostream>
#include <vector>
#include <memory>
using namespace std;

class Matrix {
private:
    int rows;                
    int cols;             
    shared_ptr<vector<float> > data;

public:
    //row 指有几行，col指有几列
    Matrix(int r, int c) : rows(r), cols(c), data(make_shared<vector<float> >(r * c, 0)) {}
    
    Matrix(const Matrix &m) : rows(m.rows), cols(m.cols), data(make_shared<vector<float>>(m.rows * m.cols)) {
        for (int i = 0; i < rows * cols; ++i) {
            (*data)[i] = (*m.data)[i];
        }
    }
    float &operator()(int r, int c) {
        if (r < 0 || r >= rows || c < 0 || c >= cols) {
            throw out_of_range("Index out of range");
        }
        return (*data)[r * cols + c];
    }

    float operator()(int r, int c) const {
        if (r < 0 || r >= rows || c < 0 || c >= cols) {
            throw out_of_range("Index out of range");
        }
        return (*data)[r * cols + c];
    }

    Matrix operator+(const Matrix &rhs) const {
        if (rows != rhs.rows || cols != rhs.cols) {
            throw invalid_argument("Matrix dimensions must match for addition");
        }
        Matrix result(rows, cols);
        for (size_t i = 0; i < data->size(); ++i) {
            (*result.data)[i] = (*data)[i] + (*rhs.data)[i];
        }
        return result;
    }

    friend ostream& operator<<(ostream& os,const Matrix& max);
};
ostream& operator<<(ostream& os, const Matrix& matrix) {
    for (int r = 0; r < matrix.rows; ++r) {
        for (int c = 0; c < matrix.cols; ++c) {
            os << matrix(r, c) << " ";
        }
        os << endl;
    }
    return os;
}
int main() {
    Matrix a(3, 4); 
    Matrix b(3, 4); 

    a(1, 2) = 3;   
    b(2, 3) = 4;    

    cout << "Matrix a is:" << endl;
    cout<<a<<endl;

    cout << "Matrix b is:" << endl;
    cout<<b<<endl;

    Matrix c = a + b;
    cout << "Matrix c (a + b) is:" << endl;
    cout<<c<<endl;

    Matrix d = a;
    cout << "Matrix d before assignment is:" << endl;
    cout<<d<<endl;

    d = b;
    cout << "Matrix d after assignment is:" << endl;
    cout<<d<<endl;

    return 0;
}