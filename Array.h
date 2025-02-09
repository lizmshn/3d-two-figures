#pragma once
#include <iostream>
#include <vector>
#include "graphics.h"
using namespace std;
template<typename T>
class Array {
private:
 vector<vector<T>> data;
 size_t row_size;
 size_t col_size;
public:
 size_t Amount_rows() { return row_size; }
 size_t Amount_cols() { return col_size; }
 Array(size_t s)
 {
 row_size = s;
 col_size = s;
 this->data.resize(s, vector<T>(s));
 };
 Array(size_t row_size, size_t col_size)
 {
 this->row_size = row_size;
 this->col_size = col_size;
 this->data.resize(row_size, vector<T>(col_size));
 };
 Array(const vector<vector<T>>& data)
 {
 row_size = data.size();
 col_size = data[0].size();
 this->data = data;
 }
 T& operator()(size_t i, size_t j)
 {//change elem
 if (j >= col_size || i >= row_size)
 throw std::runtime_error("Exiting the boundaries of the vectors");
 return data[i][j];
 }
 Array operator*(const Array& b) const
 {
 if (col_size != b.row_size)
 throw std::runtime_error("The mismatch of the sizes of matrices");
 Array result(row_size, b.col_size);
 result.Fill(0);
 for (size_t i = 0; i < result.Amount_rows(); ++i)
 {
 for (size_t k = 0; k < col_size; ++k)
 {
 double temp = operator()(i, k);
 for (size_t j = 0; j < result.Amount_cols(); ++j)
 result.data[i][j] += temp * b(k, j);
 }
 }
 return result;
 }
 Array(size_t s, const vector<vector<T>>& data)
 {
 row_size = s;
 col_size = s;
 if (data.size() != s * s)
 throw std::runtime_error("uncorrect vector size");
 this->data = data;
 }
 Array(size_t row_size, size_t col_size, const vector<vector<T>>& data)
 {
 this->row_size = row_size;
 this->col_size = col_size;
 if (data.size() != row_size * col_size)
 throw std::runtime_error("uncorrect vector size");
 this->data = data;
 }
 Array Fill(T value)
 {
 for (size_t i = 0; i < row_size; ++i)
 for (size_t j = 0; j < col_size; ++j)
 data[i][j] = value;
 return *this;
 }
 T operator()(size_t i, size_t j) const
 {//error if more gran
 if (j >= col_size || i >= row_size)
 throw std::runtime_error("Exiting the boundaries of the vectors");
 return data[i][j];
 }
 friend std::ostream& operator<<(std::ostream& os, const Array& array)
 {
 for (const auto& row : array.data)
 {
 for (const auto& item : row)
 os << item << "\t\t";
 os << endl;
 }
 return os;
 }
};
