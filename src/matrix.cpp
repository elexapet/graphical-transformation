/* 
* implementation for matrix.h 
* 
* 12/12/16
* Petr Elexa
*
*/


#include "matrix.h"
#include <string>
#include <cmath>

proxy::proxy(double* colArray, const double& cols) 
			: colArray(colArray), cols(cols) {}

double& proxy::operator[](const uint col) const
{
	if (col >= cols || col < 0) throw matrixException("out of range"); 
	
	return colArray[col];
}

void matrix::deleteInternal()
{
	for (uint i = 0; i < rows; ++i)
	{
		delete [] the_matrix[i];
	}
	delete [] the_matrix;
}

void matrix::allocClearInternal()
{
	the_matrix = new double*[rows];
	for (uint i = 0; i < rows; ++i)
	{
		the_matrix[i] = new double[cols];
		//clear
		for (uint j = 0; j < cols; ++j)
		{
			the_matrix[i][j] = 0.;
		}
	}
}

void matrix::allocAndCopyInternal(const matrix& from)
{
	the_matrix = new double*[rows];
	for (uint i = 0; i < rows; ++i)
	{
		the_matrix[i] = new double[cols];
		//copy
		for (uint j = 0; j < cols; ++j)
		{
			the_matrix[i][j] = from.the_matrix[i][j];
		}
	}
}

// Parameterized constructor
matrix::matrix(unsigned int rows, unsigned int cols) : rows(rows),cols(cols) 
{  
	if (rows < 1 || cols < 1)
	{
		throw matrixException("p-constructor bad arguments");
	}

	allocClearInternal();
}

// Copy constructor
matrix::matrix(const matrix& from) : rows(from.rows), cols(from.cols)
{
	allocAndCopyInternal(from);
}

// Destructor
matrix::~matrix()
{
	deleteInternal();
}

// Assignment operator
matrix& matrix::operator=(const matrix& rhs)
{
	if (this == &rhs) return *this;

	if (rows != rhs.rows  || cols != rhs.cols)
	{
		deleteInternal();

		rows = rhs.rows;
		cols = rhs.cols;

		allocAndCopyInternal(rhs);
	}
	else
	{
		for (uint i = 0; i < rows; ++i)
			for (uint j = 0; j < cols; ++j)
			{
				the_matrix[i][j] = rhs.the_matrix[i][j];
			}
	}

	return *this;
}

// Named constructor (static)
matrix matrix::identity(unsigned int size)
{
	if (size < 1) throw matrixException("out of range");
	
	// use p-constructor
	matrix product = matrix(size,size);
	// make it identity matrix
	for (uint i = 0; i < size; ++i)
	{
		product.the_matrix[i][i] = 1.;
	}
	
	return product;
}


// Binary operations
matrix matrix::operator+(const matrix& rhs) const
{
	if (rows != rhs.rows  || cols != rhs.cols) 
		throw matrixException("size does not match |+|");

	matrix product(rows, cols);

	for (uint i = 0; i < rows; ++i)
		for (uint j = 0; j < cols; ++j)
		{
			product.the_matrix[i][j] = the_matrix[i][j] + rhs.the_matrix[i][j];
		}

	return product;
}

matrix matrix::operator-(const matrix& rhs) const
{
	if (rows != rhs.rows  || cols != rhs.cols) 
		throw matrixException("size does not match |-|");

	matrix product(rows, cols);

	for (uint i = 0; i < rows; ++i)
		for (uint j = 0; j < cols; ++j)
		{
			product.the_matrix[i][j] = the_matrix[i][j] - rhs.the_matrix[i][j];
		}

	return product;
}


matrix matrix::operator*(const matrix& rhs) const
{
	if (cols != rhs.rows)
		throw matrixException("size does not not match |*|");

	matrix product(rows, rhs.cols);
	double tmp;

	for (uint i = 0; i < rows; ++i)
		for (uint j = 0; j < rhs.cols; ++j)
		{
			tmp = 0;
			for (uint k = 0; k < cols; ++k)
			{
				tmp += the_matrix[i][k] * rhs.the_matrix[k][j];
			}
			product.the_matrix[i][j] = tmp;
		}
	return product;
}

matrix matrix::operator*(const double scale) const
{
	matrix product(rows, cols);

	for (uint i = 0; i < rows; ++i)
		for (uint j = 0; j < cols; ++j)
		{
			product.the_matrix[i][j] = the_matrix[i][j] * scale;
		}

	return product;
}


// Unary operations
matrix matrix::operator~() const
{
	matrix product(cols, rows);

	for (uint i = 0; i < cols; ++i)
		for (uint j = 0; j < rows; ++j)
		{
			product.the_matrix[i][j] = the_matrix[j][i];
		}

	return product;
}
	

void matrix::clear()
{
	for (uint i = 0; i < rows; ++i)
		for (uint j = 0; j < cols; ++j)
		{
			the_matrix[i][j] = 0.0;
		}
	return;
}

proxy matrix::operator[](uint row)
{
	if (row >= rows || row < 0) 
		throw matrixException("out of range");
	
	return proxy(the_matrix[row], cols);
}

proxy matrix::operator[](uint row) const
{
	if (row >= rows || row < 0) 
		throw matrixException("out of range");

	return proxy(the_matrix[row], cols);
}


std::ostream& matrix::out(std::ostream& os) const
{
	os << rows << ' ' << cols << std::endl;
	os << "{\n";
	for (uint i = 0; i < rows; ++i)
	{
		os << "{";
		for (uint j = 0; j < cols; ++j)
		{
			os << the_matrix[i][j];
			os << (j+1 < cols ? "," : "");
		}
		os << (i+1 < rows ? "},\n" : "}\n");
	}	
	os << "}";
	return os;
}

std::istream & operator>> (std::istream & is, matrix & out)
{
	uint unpaired = 0, r = 0, c, refc = 0;
	double num;
	char token;

	//freeMatrix(out.the_matrix, out.rows);
	out.deleteInternal();

	//read number of rows and columns
	is >> out.rows >> out.cols;

	//allocate new matrix
	out.the_matrix = new double* [out.rows];
	for (uint i = 0; i < out.rows; ++i){
		out.the_matrix[i] = new double [out.cols];
	}
	
	is >> token;
	if (token != '{') {
		is.setstate (std::ios::failbit);
		return is;
	}
	do
	{
		c = 0;
		is >> token;
		if (token == '{')
		{
			unpaired++;
			do
			{
				is >> num;
				if (!is) return is;
				out.the_matrix[r][c] = num;
				c++;
				is >> token;
			} while ( token == ',' );
			if ( token == '}' )
			{
				if (!refc) refc = c;
				else if (refc != c) break;
				unpaired--;
			} 
			else break;
		}
		else break;
		r++;
	} while (is.get() == ',');

	if (unpaired || r != out.rows || refc != out.cols) {
		is.setstate (std::ios::failbit);
		return is;
	}
	is >> token;
	if (token != '}'){
		is.setstate (std::ios::failbit);
		return is;
	}

	return is;
}

// Global insertion and operator
std::ostream& operator<<(std::ostream& os, const matrix& rhs)
{
	rhs.out(os);
	return os;
}

// Global scalar multiplication
matrix operator*(const double scale, const matrix& rhs)
{
	return rhs*scale;
}


