#include "Matrix.hpp"
#include <iterator>

MyIterator::MyIterator(Node* some_node)//constructs an iterator
{
    current_node = some_node;
    row_start = some_node;
}

MyIterator& MyIterator::operator++() //prefix
{
    if (current_node->next_row != nullptr) {
        // If there is a next element in the current row, move to it
        current_node = current_node->next_row;
    }
    else {
        // If there is no next element in the current row, move to the first element of the next row
        row_start = row_start->next_col;
        current_node = row_start;
    }
    return *this;
}

MyIterator MyIterator::operator++(int dummy) //postfix
{
    MyIterator temp(*this);
    if (current_node->next_col != nullptr) {
        current_node = current_node->next_col;
    }
    else if (current_node->next_row != nullptr) {
        current_node = current_node->next_row;
        current_node = current_node->next_col;
    }
    return temp;
}

int& MyIterator::operator*()//return the value
{
    return current_node->value;
}

int* MyIterator::operator->()
{
    return &(current_node->value); //& is the address of the pointer
}

bool MyIterator::operator==(const MyIterator& it)//compare iterators
{
    return current_node == it.current_node;
}

bool MyIterator::operator!=(const MyIterator& it)
{
    return current_node != it.current_node;
}


MyIterator& MyIterator::operator=(const MyIterator& it)//iterator assignment operator
{
    if (this != &it)
    {
        current_node = it.current_node;
        row_start = it.row_start;
    }

    return *this;
}

Matrix::Matrix() :head(nullptr), num_rows(0), num_cols(0) {}//default constructor

Matrix::Matrix(int array[][MAX], int row_size, int col_size) :num_rows(row_size), num_cols(col_size)
{                                                   //parametrized constructor 
    if (row_size <= 0 || col_size <= 0 || col_size > MAX)
        throw runtime_error("Rows and columns cannot be negative and cannot exceed ten");

    head = new Node();
    Node* current_col = head;

    for (int i = 0; i < num_rows - 1; i++) //creating the nodes horizontally based on the num of cols
    {
        current_col->value = array[i][0];
        current_col->next_row = new Node();
        current_col = current_col->next_row;
    }

    current_col->value = array[num_rows - 1][0]; // handle the last column separately

    Node* temp = head; //a temporary pointer to head
    for (int i = 0; i < num_rows; i++) //go back to head node and create node vertically
    {
        Node* new_head = temp; //a pointer to head to loop vertically
        for (int j = 1; j < num_cols; j++)
        {
            new_head->next_col = new Node(array[i][j]); //create new nodes along the column
            new_head = new_head->next_col; //shift pointer along the column
        }
        temp = temp->next_row; //shift pointer along the row
    }
}

Matrix::~Matrix() //destructor
{
    clear();
}

// Copy constructor
Matrix::Matrix(const Matrix& copy_m)
{
    copyFrom(copy_m);
}

Matrix::Matrix(Matrix&& obj) noexcept : head(nullptr), num_rows(0), num_cols(0) //move constructor
{

    head = obj.head;
    num_rows = obj.num_rows;
    num_cols = obj.num_cols;


    obj.head = nullptr;
  
}

Matrix& Matrix::operator=(Matrix&& obj) noexcept//move assignment operator
{
    if (this != &obj)
    {
        clear();

        head = obj.head;
        num_rows = obj.num_rows;
        num_cols = obj.num_cols;

        obj.head = nullptr;

    }    

    return (*this);
}

Matrix& Matrix::operator=(const Matrix& obj) //assignment operator
{
    if (this == &obj) // self-assignment check
    {
        return *this;
    }

    clear();
    copyFrom(obj);
    return *this;
}

void Matrix::clear()//clears the matrix
{
    Node* traverse_ptr = head;
    while (traverse_ptr != nullptr)
    {
        Node* delete_row = head; //assign the first row as the now to be delete
        while (delete_row != nullptr) //delete nodes horizontally
        {
            Node* delete_node = delete_row; // hold the first node
            delete_row = delete_row->next_col;
            delete delete_node; //delete that node
        }
        traverse_ptr = traverse_ptr->next_row; // go to the next row and do the same thing
    }
}

int Matrix::getColNum() const//getter for number of columns
{
    return num_cols;
}

int Matrix::getRowNum() const//getter for number of rows
{
    return num_rows;
}

int* Matrix::getRow(int row_num) const { //returns a row
    if (row_num < 0 || row_num >= num_rows) {
        throw runtime_error("Invalid row number.");
    }

    Node* current = head;
    for (int i = 0; i < row_num; i++) {
        current = current->next_row;
    }

    int* row_values = new int[num_cols];
    for (int i = 0; i < num_cols; i++) {
        row_values[i] = current->value;
        current = current->next_col;
    }

    return row_values;
}


int* Matrix::getCol(int col_num) const//returns a column
{
    if (col_num >= num_cols)
    {
        throw runtime_error("Column number cannot exceed the number of columns!");
    }
    int* arr = new int[num_rows];
    Node* temp = head;
    for (int i = 0; i < col_num; i++)
    {
        temp = temp->next_col;
    }
    for (int i = 0; i < num_rows && temp != nullptr; i++, temp = temp->next_row)
    {
        arr[i] = temp->value;
    }
    return arr;
}

Matrix& Matrix::transpose()//gives the transpose of the matrix
{
    const int col = num_rows;
    const int row = num_cols;

    int current_row[MAX][MAX];
    int trans_array[MAX][MAX]; // create a 2D array for the transpose 

    for (int j = 0; j < num_rows; j++) {
        int* hi = getRow(j);

        for (int a = 0; a < num_cols; a++)
        {
            current_row[a][j] = hi[a];
            trans_array[j][a] = current_row[a][j]; // assign the values to the transpose array
        } // get the current row


    }
}

Matrix Matrix::operator+(Matrix obj)//overloaded addition operator
{
    if (num_rows != obj.num_rows || num_cols != obj.num_cols)
        throw runtime_error("cannot add matrices of different dimensions");

    // create new matrix
    Matrix m = obj;

    Node* current_row = m.head;
    Node* copy_row = head;

    Node* r1 = m.head;
    Node* r2 = head;

    do {
        do {

            current_row->value = copy_row->value + current_row->value;
            cout << current_row->value;
            current_row = current_row->next_col;
            copy_row = copy_row->next_col;

        } while (copy_row->next_col != nullptr);
        r1 = r1->next_row;

        r2 = r2->next_row;

        current_row = r1;
        copy_row = r2;

    } while (copy_row->next_row != nullptr);

    return m;

}


Matrix Matrix::operator*(Matrix obj)//matrix multiplication
{
    if (num_cols != obj.num_rows)
        throw runtime_error("cannot multiply these 2 matrices, col num of first needs to match row num of the other");

    int array[MAX][MAX] = { 0 };

    Matrix m(array, num_rows, obj.num_cols);

    // get the transpose of obj, call it R
    // two iterators, one for the calling object and the other for obj
    // as they iterate thru the row, they multiply and add their values
    // once reaches the end of the row, all this is one value
    // third iterator points to head of new matrix, adds that value to the first element of the new matrix
    // this third iterator goes to the next element
    // repeat this process, except R's iterator iterates to the next row, while the calling object's restarts. this whole process
    // is repeated until iterator to new matrix goes to the next row
    // thats when iterator to R restarts, and iterator to calling object goes to next row and (automatically) iterator to new mayrix foes to nextrow

    Matrix R = obj.transpose();

    Node* current_row = m.head;
    Node* copy_row = head;
    Node* copy_row2 = R.head;

    Node* rstart1 = m.head;
    Node* rstart2 = head;
    Node* rstart3 = R.head;


    do {

        do {

            do {

                current_row->value += copy_row->value * copy_row2->value;

                copy_row = copy_row->next_col;
                copy_row2 = copy_row2->next_col;

            } while (copy_row->next_col != nullptr);

            current_row = current_row->next_col;

            rstart3 = rstart3->next_row;
            copy_row2 = rstart3;
            copy_row = rstart2;
        } while (current_row->next_col != nullptr);

        copy_row2 = R.head;
        rstart2 = rstart2->next_row;
        copy_row = rstart2;
        rstart1 = rstart1->next_row;
        current_row = rstart1;
    } while (current_row->next_row && current_row->next_col != nullptr);

    return m;
}

void Matrix::copyFrom(const Matrix& obj)//copies 
{
    num_rows = obj.num_rows;
    num_cols = obj.num_cols;
    head = new Node(obj.head->value); // copy the head
    Node* current_row = head; // pointer to traverse through the rows
    Node* other_current_row = obj.head; // pointer to iterate through the other matrix rows

    // copy the rows
    while (other_current_row->next_row != nullptr)
    {
        current_row->next_row = new Node(other_current_row->next_row->value);
        other_current_row = other_current_row->next_row;
        current_row = current_row->next_row;
    }

    // copy the columns of each row
    current_row = head;
    other_current_row = obj.head;
    while (other_current_row != nullptr)
    {
        Node* current_col = current_row;
        Node* other_current_col = other_current_row;
        while (other_current_col->next_col != nullptr)
        {
            current_col->next_col = new Node(other_current_col->next_col->value);
            other_current_col = other_current_col->next_col;
            current_col = current_col->next_col;
        }
        current_row = current_row->next_row;
        other_current_row = other_current_row->next_row;
    }
}

int Matrix::getElement(int row, int col) const //returns a element based on the given row & column
{
    if (row > getRowNum() || col > getColNum())
    {
        throw runtime_error("ERROR: The value is out of bounds.");
    }

    Node* getEl = head;
    
    for (int i = 0; i < col; i++)
    {
        getEl = getEl->next_col;
    }

    for (int j = 0; j < row; j++)
    {
        getEl = getEl->next_row;
    }

    return getEl->value;
}

ostream& operator<<(ostream& output, const Matrix& mat)//overloaded stream insertion
{
    MyIterator it = mat.begin();
    int count = 0;

    while (it != mat.end())
    {
        output << *it << " ";
        it++;
        count++;
        if (count == mat.getColNum())
        {
            count = 0;
            output << endl; 
        }
    }

    return output;
}
