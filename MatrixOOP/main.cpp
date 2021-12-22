#include <iostream>
#include <cstdlib>

using std::cout;
using std::cin;
using std::cerr;

class Matrix;
Matrix operator+(Matrix left, Matrix right);	
Matrix operator-(Matrix left, Matrix right);	 
Matrix operator*(Matrix left, Matrix right);	 
Matrix operator/(Matrix left, Matrix right);

class Matrix
{
private:
	size_t rows;
	size_t cols;
	double** matrix;

public:
	const size_t getRows()const
	{
		return rows;
	}
	const size_t getCols()const
	{
		return cols;
	}
	double** getMatrix()const
	{
		return matrix;

	}
	Matrix(size_t rows = 4, size_t cols = 4) : rows(rows), cols(cols)
	{
		matrix = new double* [rows] {};
		for (size_t i = 0; i < rows; ++i)
			matrix[i] = new double[cols] {};
	}

	~Matrix()
	{
		for (size_t i = 0; i < rows; ++i)
			delete matrix[i];
		delete[] matrix;
	}

	Matrix(const Matrix& other) : Matrix(other.getRows(), other.getCols())
	{
		for (size_t i = 0; i < rows; ++i)
			for (size_t j = 0; j < cols; ++j)
				matrix[i][j] = other.matrix[i][j];
	}
	Matrix& operator=(const Matrix& other) //copy assignment
	{
		if (this == &other) return *this;

		for (size_t i = 0; i < rows; ++i)
			delete matrix[i];
		delete[] matrix;

		matrix = new double* [other.getRows()]{};
		for (size_t i = 0; i < rows; ++i)
			matrix[i] = new double[other.getCols()]{}; //подчеркивает зеленым "переполнение буфера при записи в matrix"

		for (size_t i = 0; i < rows; ++i)
			for (size_t j = 0; j < cols; ++j)
				matrix[i][j] = other.matrix[i][j]; //подчеркивает зеленым "переполнение буфера при записи в matrix[i]"

		return *this;
	}

	Matrix& operator+=(const Matrix& other)
	{
		return *this = *this + other;
	}
	Matrix& operator-=(const Matrix& other)
	{
		return *this = *this - other;
	}
	Matrix& operator*=(const Matrix& other)
	{
		return *this = *this * other;
	}
	Matrix& operator/=(const Matrix& other)
	{
		return *this = *this / other;
	}
	void print()const
	{
		cout << "\nSize of matrix:\t" << rows << 'x' << cols;
		for (size_t i = 0; i < rows; ++i)
		{
			cout << '\n';
			for (size_t j = 0; j < cols; ++j)
			{
				cout << matrix[i][j] << '\t';
			}
		}
	}

	std::ostream& print(std::ostream& os)const
	{
		os << "\nSize of matrix:\t" << rows << 'x' << cols;
		for (size_t i = 0; i < rows; ++i)
		{
			os << '\n';
			for (size_t j = 0; j < cols; ++j)
			{
				os << matrix[i][j] << '\t';
			}
		}
		return os;
	}

	void fillRandomly()
	{
		for (size_t i = 0; i < rows; ++i)
			for (size_t j = 0; j < cols; ++j)
				matrix[i][j] = rand() % (100 + 100) - 100;
	}

	void getMatrixCut(Matrix& temp, size_t i, size_t j, size_t size)const //не буду врать, мне было слишком лень придумывать велосипед, потому этот код нагло слизан с интернета.
	{
		size_t di;
		di = 0;
		for (size_t ki = 0; ki < size - 1; ki++)
		{
			if (ki == i)
				di = 1;
			size_t dj = 0;
			for (size_t kj = 0; kj < size - 1; kj++)\
			{
				if (kj == j) dj = 1;
				temp.getMatrix()[ki][kj] = this->getMatrix()[ki + di][kj + dj];
			}
		}
	}

	long double Determinant(size_t size) //и это тоже украдено и адаптировано + нам необходим параметр, чтобы создать рекурсию.
	{
		if (rows != cols) //нахождение определителя имеет смысл только для квадратных матриц
		{
			cerr << "It\'s impossible to calculate deteterminant!";
			return -1;
		}
		size_t j, det, k, n;
		Matrix temp(size, size);
		j = 0; det = 0;
		k = 1; //(-1) в степени i
		n = size - 1;
		if (size < 1) cerr << "It\'s impossible to calculate deteterminant!";
		if (size == 1)
		{
			det = this->getMatrix()[0][0];
			return(det);
		}
		if (size == 2)
		{
			det = this->getMatrix()[0][0] * this->getMatrix()[1][1] - (this->getMatrix()[1][0] * this->getMatrix()[0][1]);
			return(det);
		}
		if (size > 2)
		{
			for (size_t i = 0; i < size; i++)
			{
				getMatrixCut(temp, i, 0, size);
				det = det + k * getMatrix()[i][0] * temp.Determinant(n);
				k = k * -1;
			}
		}
		return(det);
	}


	/*Ниже нерабочий метод Гаусса*/
	//double Determinant()
	//{
	//	if (rows != cols) //нахождение определителя имеет смысл только для квадратных матриц
	//	{
	//		cerr << "It\'s impossible to calculate deteterminant!";
	//		return -1;
	//	}
	//	Matrix temp = *this;
	//	//приведение матрицы к верхнетреугольному виду
	//	for (size_t step = 0; step < this->getRows() - 1; step++)
	//		for (size_t row = step + 1; row < this->getRows(); ++row)
	//		{
	//			double coeff = (temp.getMatrix()[row][step] * -1) / temp.getMatrix()[step][step]; //метод Гаусса
	//			for (size_t col = step; col < this->getRows(); ++col)
	//				temp.getMatrix()[row][col] += temp.getMatrix()[step][col] * coeff;
	//		}
	//	//Рассчитать определитель как произведение элементов главной диагонали
	//	double det = 1;
	//	for (int i = 0; i < this->getRows(); ++i)
	//		det *= temp.getMatrix()[i][i];
	//	return det;
	//}

	void Transposition() //пока только для квадратной
	{
		if (rows != cols) //нахождение определителя имеет смысл только для квадратных матриц
		{
			cerr << "It\'s impossible to calculate deteterminant!";
			return;
		}
		double temp;
		for (size_t i = 1; i < rows; i++)
		{
			for (size_t j = 0; j < i; j++)
			{
				temp = this->getMatrix()[i][j];
				this->getMatrix()[i][j] = this->getMatrix()[j][i];
				this->getMatrix()[j][i] = temp;
			}
		}
	}

	Matrix& inverse(Matrix obj)
	{
		if (!((this->getRows() == obj.getRows()) //если у матрицы есть определитель, то значит она квадратная.
			&& (this->getCols() == obj.getCols())))
			return *this;
		long double det = obj.Determinant(obj.getRows());
		if (det)
		{
			for (size_t i = 0; i < this->getRows(); ++i)
			{
				for (size_t j = 0; j < this->getRows(); ++j)
				{
					size_t newSize = this->getRows() - 1;
					Matrix temp(newSize, newSize);
					getMatrixCut(temp, i, j, this->getRows());
					this->getMatrix()[i][j] = pow(-1.0, i + j + 2) * temp.Determinant(temp.getCols()) / det;
				}
			}
		}
		else
		{
			cerr << "\nMatrix is degenerate."; //матрица вырожденная
			return *this;
		}
		Transposition();
		return *this;
	}
};

std::istream& operator>>(std::istream& is, Matrix& obj)
{
	for (size_t i = 0; i < obj.getRows(); ++i)
		for (size_t j = 0; j < obj.getCols(); ++j)
		{
			cout << "\nPlease enter [" << i << "][" << j << "] element: ";
			is >> obj.getMatrix()[i][j];
		}
	return is;
}
std::ostream& operator<<(std::ostream& os, const Matrix& obj)
{
	return obj.print(os);
}

Matrix operator+(Matrix left, Matrix right)
{
	if (!( (left.getRows() == right.getRows())
		&& (left.getCols() == right.getCols()) ))
		return left;
	Matrix result(left.getRows(), left.getCols());
	for (size_t i = 0; i < left.getRows(); ++i)
		for (size_t j = 0; j < left.getCols(); ++j)
			result.getMatrix()[i][j] = left.getMatrix()[i][j] + right.getMatrix()[i][j];
	return result;
}
Matrix operator-(Matrix left, Matrix right)
{
	if (!((left.getRows() == right.getRows())
		&& (left.getCols() == right.getCols())))
		return left;
	Matrix result(left.getRows(), left.getCols());
	for (size_t i = 0; i < left.getRows(); ++i)
		for (size_t j = 0; j < left.getCols(); ++j)
			result.getMatrix()[i][j] = left.getMatrix()[i][j] - right.getMatrix()[i][j];
	return result;
}

Matrix operator*(Matrix left, Matrix right) //известно, что произведение двух матриц имеет смысл только тогда, когда число столбцов матрицы А совпадает с числом строк матрицы В
{
	if (!(left.getCols() == right.getRows())) 
		return left;
	Matrix result(left.getRows(), right.getCols());
	for (size_t i = 0; i < left.getRows(); ++i)
		for (size_t j = 0; j < right.getCols(); ++j)
		{
			for (size_t k = 0; k < right.getRows(); ++k)
				result.getMatrix()[i][j] += (left.getMatrix()[i][k] * right.getMatrix()[k][j]);
		}
	return result;
}
Matrix operator/(Matrix left, Matrix right) //известно, что произведение двух матриц имеет смысл только тогда, когда число столбцов матрицы А совпадает с числом строк матрицы В
{
	if (!((left.getRows() == right.getRows())
		&& (left.getCols() == right.getCols())))
		return left;
	Matrix result(left.getRows(), right.getCols());
	result = left * right.inverse(right);
	return result;
}

//#define OUTPUT_CHECK
//#define OPERATORS_CHECK
int main()
{
	bool choose = false;
	size_t rows;
	size_t cols;
	srand(time(0));
#ifdef OUTPUT_CHECK
	cout << "Please, enter number of rows in your matrix: ";
	cin >> rows;
	cout << "Please, enter number of cols in your matrix: ";
	cin >> cols;
	Matrix A(rows, cols);
	cout << "\nPlease, enter how do you want to fill array?\n1 - Randomly (frosize-100 to 100)\n0 - Manual input \nOther - randomly\nEnter: ";
	cin >> choose;
	if (!choose)
		cin >> A;
	else
		A.fillRandomly();

	cout << A;
#endif //OUTPUT_CHECK
#ifdef OPERATORS_CHECK
	Matrix A(9, 6), B(6, 6);
	A.fillRandomly();
	cout << "\nA" << A;
	B.fillRandomly();
	cout << "\n\nB" << B;
	//Matrix C = A;

	Matrix C = A + B;
	Matrix D = A * B;
	cout << "\n\nC" << C;
	cout << "\n\nD" << D;
#endif OPERATORS_CHECK
	Matrix A(3, 3);
	Matrix B(3, 3);
	//cin >> A;
	A.fillRandomly();
	cout << "\nA" << A;
	cout << "\nB" << B;
	//cout << "\nDeterminant: " << A.Determinant(); //Проверка метода Гаусса
	//cout << "\nDeterminant: " << A.Determinant(A.getRows()); //да, криво, но что поделать. зато работает.
	//Matrix B(A.inverse(A)); //не знаю чего я хотел добиться, когда возвращал значение таким образом, но менять уже лень.
	cout << "\nB" << B;
	cout << "\nA/B" << A / B;


	return 0;
}