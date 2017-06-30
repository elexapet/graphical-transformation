/**
 * matrix.h - declaration of matrix class.  This class will be
 * capable of represting a Matrix as a first-class type.
 * 
 * Do not change any public methods in this file unless otherwise
 * instructed.
 *
 * For CS321.  (c) 2015 Dr. Darrin Rothe
 */
 
 
// compile guard 
#ifndef MATRIX_H
#define MATRIX_H
 
#include <iostream> // for std::ostream
#include <stdexcept>	// for std::runtime_error
#include <string>	// used in exception
 
#define uint unsigned int

// a helper class to bundle a message with any thrown exceptions.
// To use, simply 'throw matrixException("A descriptive message about
// the problem")'.  This will throw the exception object by value.
// Recommendation is to catch by reference (to prevent slicing).
class matrixException:public std::runtime_error
{
	public:
		matrixException(std::string message)
		: std::runtime_error((std::string("Matrix Exception: ")+message).c_str()){}
};

//helper class for accessing individual values (someclass[][])
class proxy 
{
	public:
	    proxy(double* colArray, const double& cols);
	    double& operator[](const uint col) const;
	private:
	    double* colArray;
	    const double cols;
};

class matrix
{
	public:
		// No default (no argument) constructor.  It doesn't really make
		// sense to have one as we cannot rely on a size.  This may trip
		// us up later, but it will lead to a better implementation.
		// matrix();
 
		// Constructor - create Matrix and clear cells.  If rows or
		// cols is < 1, throw a matrixException.  Note, we will not use
		// "exception specifications" as multiple sources report that 
		// the mechanism is not properly supported by most compilers.
		//
		// throw (matrixException)
		//
		matrix(unsigned int rows, unsigned int cols);
 
		// Copy constructor - make a new Matrix just like rhs
		matrix(const matrix& from);
 
		// Destructor.  Free allocated memory
		~matrix();
 
		// Assignment operator - make this just like rhs.  Must function
		// correctly even if rhs is a different size than this.
		matrix& operator=(const matrix& rhs);
 
		// "Named" constructor(s).  This is not a language mechanism, rather
		// a common programming idiom.  The underlying issue is that with
		// overloaded operators, you can loose sight of what various
		// combinations of arguments means.  For our design, constructor
		// arguments set the size of the matrix.  If we want to generate
		// "special" matrices, we are better off with statuc methods that 
		// will generate these and then we can give the metods meaningful
		// names.
		
		// Named Constructor - produce a square identity matrix of the
		// requested size.  Since we do not know how the object produced will
		// be used, we pretty much have to return by value.  A size of 0 
		// would not make sense and should throw an exception.
		//
		// throw (matrixException)
		//
		static matrix identity(unsigned int size);
		
 
 
		// Matrix addition - lhs and rhs must be same size otherwise
		// an exception shall be thrown
		//
		// throw (matrixException)
		//
		matrix operator+(const matrix& rhs) const;
		matrix operator-(const matrix& rhs) const;
 
		// Matrix multiplication - lhs and rhs must be compatible 
		// otherwise an exception shall be thrown
		//
		// throw (matrixException)
		//
		matrix operator*(const matrix& rhs) const;
 
		// Scalar multiplication.  Note, this function will support
		// someMatrixObject * 5.0, but not 5.0 * someMatrixObject.
		matrix operator*(const double scale) const;

		// Transpose of a Matrix - should always work, hence no exception
		matrix operator~() const;
 
		// Clear Matrix to all members 0.0
		void clear();
  
		// Access Operators - throw an exception if index out of range
		//
		// throw (matrixException)
		//
		proxy operator[](uint row);
 
		// const version of above - throws an exception if indices are out of
		// range
		//
		// throw (matrixException)
		//
		proxy operator[](uint row) const;
 
		// I/O - for convenience - this is intended to be called by the global
		// << operator declared below.
		std::ostream& out(std::ostream& os) const;

		//input operator
		friend std::istream & operator>> (std::istream & is, matrix & out);
		
	private:
		// The data - note, per discussion on arrays, you can store these data
		// as a 1-D dynamic array, thus the double* below.  Alternatively, and
		// perhaps preferred, you could store the data as an array of arrays
		// which would require the_Matrix to be changed to a double**.
		double** the_matrix;
		unsigned int rows;
		unsigned int cols;

		/** routines **/

		//free memory allocated for the_matrix
		void deleteInternal();
		
		//allocate the_matrix
		void allocClearInternal();

		//allocate the_matrix copy data
		void allocAndCopyInternal(const matrix& from);

};

/** Some Related Global Functions **/
 
// Overloaded global << with std::ostream as lhs, Matrix as rhs.  This method
// should generate output compatible with an ostream which is commonly used
// with console (cout) and files.
//
// Since this is a global function, it does not have access to the private
// data of a Matrix object.  So, it will need to use the public interface of
// Matrix to do its job.  The method Matrix::out was added to Matrix
// specifically for this purpose.  The other option would have been to make
// it a "friend"
 
std::ostream& operator<<(std::ostream& os, const matrix& rhs);


//friend operator>>
std::istream& operator>> (std::istream& is, matrix& out);
 
// We would normally have a corresponding >> operator, but
// will defer that exercise that until a later assignment.


// Scalar multiplication with a global function.  Note, this function will
// support 5.0 * someMatrixObject, but not someMatrixObject * 5.0
matrix operator*(const double scale, const matrix& rhs);

 
#endif //MATRIX_H
