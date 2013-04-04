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
	f << in_mat << endl;
	f  << out_mat << endl;
	return out_mat;
}


int main()
{
	//cout << "Hello World" << endl;

	mat A;
	mat B;
	A << 2 << 1 << 1 << 1 << 0 << 0 << 14 << endr
	  << 4 << 2 << 3 << 0 << 1 << 0 << 28 << endr
	  << 2 << 5 << 5 << 0 << 0 << 1 << 30 << endr
	  << -1 << -2 << 1 << 0 << 0 << 0 << 0 << endr;

	uword pivot_col = findPivotCol( A );
	uword pivot_row = findPivotRow( A, pivot_col );
	

	if( pivot_col != NULL)
	{
		float divide_by = A.at( pivot_row, pivot_col );	
		B = rowOperationMultiply(A,  pivot_row, 1.0/divide_by );
	}
	//cout << B << endl;


	system("pause");
	return 0;
}