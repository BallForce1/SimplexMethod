#include <iostream>
#include <fstream>
#include <armadillo>

using namespace std;
using namespace arma;

fstream f("dump.txt");

uword findPivotCol( mat in_mat )
{
	uword row_index;
	uword col_index;
	float most_negative = in_mat.min( row_index, col_index );
	if( row_index == in_mat.n_rows-1 && most_negative < 0 )
		return col_index;
	return NULL;
}

uword findPivotRow( mat in_mat, uword pc )
{
	if(pc != NULL)
	{
		float min_value = 1000000.0;
		uword index;
		for( uword i=0.0; i<in_mat.n_rows-1; i++ )
		{
			float temp = in_mat.at( i, in_mat.n_cols-1 ) / in_mat.at( i, pc );
			if( temp < min_value )
			{
				min_value = temp;
				index = i;
			}
		}	
		return index;
	}
	else
	{
		cout << "WARNING:  Pivot column null." << endl;
		return NULL;
	}
}

mat rowOperationMultiply( mat in_mat, uword row, float number )
{
	mat out_mat( in_mat.n_rows, in_mat.n_cols );

	for( uword i=0.0; i<in_mat.n_rows; i++ )
	{
		for( uword j=0.0; j<in_mat.n_cols; j++ )
		{
			if( i == row )
			{
				out_mat( i, j ) = in_mat( i, j ) * number;
			}
			else
			{
				out_mat( i, j ) = in_mat( i, j );
			}
		}
	}
	return out_mat;
}

mat rowOperationMultiplyAdd( mat in_mat, uword multiply_row, uword add_row, float multiply_number )
{
	mat out_mat( in_mat.n_rows, in_mat.n_cols );

	out_mat = rowOperationMultiply(in_mat, multiply_row, multiply_number);

	for( uword i=0.0; i<in_mat.n_rows; i++ )
	{
		for( uword j=0.0; j<in_mat.n_cols; j++ )
		{
			if( i == add_row )
			{
				out_mat( i, j ) += out_mat( multiply_row, j );
			}
			else if( i != multiply_row )
			{
				out_mat( i, j ) = in_mat( i, j );
			}

		}
	}

	//Replace the origional multiply_row
	for( uword i=0.0; i<in_mat.n_cols; i++ )
	{
		out_mat(multiply_row, i) = in_mat(multiply_row, i);
	}

	return out_mat;
}

mat zeroColumn( mat in_mat, uword piv_row, uword piv_col) 
{
	mat out_mat = in_mat;

	for( uword i=0; i<out_mat.n_rows; i++ )
	{
		if( i != piv_row )
		{
 			out_mat = rowOperationMultiplyAdd( out_mat, piv_row, i, 0 - out_mat( i, piv_col ) );
		}
	}
	return out_mat;
}

int main()
{
	mat A;

	A << 2 << 1 << 1 << 1 << 0 << 0 << 14 << endr
	  << 4 << 2 << 3 << 0 << 1 << 0 << 28 << endr
	  << 2 << 5 << 5 << 0 << 0 << 1 << 30 << endr
	  << -1 << -2 << 1 << 0 << 0 << 0 << 0 << endr;

	f << A;

	uword pivot_col = findPivotCol( A );
	uword pivot_row = findPivotRow( A, pivot_col );

	f << "Pivot is at position: ( " << pivot_row << " , " << pivot_col << " )" << endl;

	if( pivot_col != NULL)
	{
		float divide_by = A.at( pivot_row, pivot_col );	
		A = rowOperationMultiply(A,  pivot_row, 1.0/divide_by );
		f << A << endl;
		
		A = zeroColumn( A, pivot_row, pivot_col );
		f << A << endl;
	}

	system("pause");
	return 0;
}