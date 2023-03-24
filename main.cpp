#include <iostream>
#include <iterator>
#include "Matrix.hpp"
using namespace std;
const int MAX = 10;

int main()
{
    int input_array[3][10] = { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} };
    int row_size = 3;
    int col_size = 4;

    // Create a new Matrix object using the input array
    Matrix matrix(input_array, row_size, col_size);
    //Matrix copy_m = matrix;


}