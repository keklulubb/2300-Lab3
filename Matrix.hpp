#include <iostream>
#include <iterator>
using namespace std;
#ifndef Matrix_hpp
#define Matrix_hpp

const int MAX = 10;


struct Node
{
    int value;
    Node* next_col;
    Node* next_row;

    Node() :value(0), next_col(nullptr), next_row(nullptr) {};
    Node(int val) : value(val), next_row(nullptr), next_col(nullptr) {};
    Node(Node* ptr, int val, Node* next_rowptr) :next_row(next_rowptr), value(val), next_col(ptr) {};
};

class Matrix
{
private:
    Node* head; //a pointer to the head of the matrix//
    int num_rows;
    int num_cols;
public:
    Matrix();
    Matrix(int array[][MAX], int row_size, int col_size);
    Matrix(const Matrix& copy_m);
    Matrix& operator =(const Matrix& obj);
    Matrix(Matrix&& obj) noexcept;
    Matrix& operator=(Matrix&& obj) noexcept;
    ~Matrix();
    void clear();
    int getColNum() const;
    int getRowNum() const;
    int* getRow(int row_num) const; //throw exception
    int* getCol(int col_num) const;//throw exception
    int getElement(int row, int col) const;
    Matrix& transpose(); //the user shouldnt be aware of the node type // should the 2d array be passed?// instead//
    Matrix operator+(Matrix obj); //throw exception if row not equal row
    Matrix operator*(Matrix obj); //throw exception if not multiplicable
    void copyFrom(const Matrix& obj);

    friend ostream& operator<<(ostream& output, Matrix& obj);

    friend class MyIterator;
    MyIterator begin() const
    {
        return MyIterator(head);
    }
   
};

class MyIterator : public std::iterator<std::forward_iterator_tag, int>
{
private:
    Node* current_node;
    Node* row_start;
public:
    MyIterator(Node* some_node); //should change to one paratmeter
    MyIterator& operator++();
    MyIterator operator++(int dummy); 
    int& operator*(); //dereference
    int* operator->();
    bool operator ==(const MyIterator& it);
    bool operator !=(const MyIterator& it);
    MyIterator& operator=(const MyIterator& it);


    //should consider implementing the allocate function to reuse//
};
#endif /* Matrix_hpp */