#include <iostream>
#include <fstream>
#include <armadillo>

using namespace std;
using namespace arma;

fstream f("dump.txt");

mat roundToZero( mat in_mat ) {
	mat out_mat = in_mat;
	
	for( uword i=0; i<out_mat.n_rows; i++ )	
		for( uword j=0; j<out_mat.n_cols; j++ )
			if( in_mat(i, j) < .000001 && in_mat(i, j) > -.000001  )		
				out_mat(i,j) = 0;
	return out_mat;
}

uword findPivotCol( mat in_mat, bool& no_negative ) {
	uword col_index;

	float most_negative = 10000000;

	for( uword i=0.0; i<in_mat.n_cols; i++ )
		if( in_mat( in_mat.n_rows-1, i ) < most_negative )	{
			most_negative = in_mat(in_mat.n_rows-1 , i );
			col_index = i;
		}
		
	if(most_negative < 0 )
		return col_index;

	no_negative = true;
}

uword findPivotRow( mat in_mat, uword piv_col ) {
	float min_value = 1000000.0;
		
	uword row_index;
		
	for( uword i=0.0; i<in_mat.n_rows-1; i++ )	{			
		float temp = in_mat.at( i, in_mat.n_cols-1 ) / in_mat.at( i, piv_col );
			
		if( temp <= min_value ){
			min_value = temp;
			row_index = i;
		}
	}	
	return row_index;
}

mat rowOperationMultiply( mat in_mat, uword row, float number ) {
	mat out_mat = in_mat;

	for( uword j=0.0; j<in_mat.n_cols; j++ )
			out_mat( row, j ) = in_mat( row, j ) * number;

	out_mat = roundToZero( out_mat );
	return out_mat;
}

mat rowOperationMultiplyAdd( mat in_mat, uword multiply_row, uword add_row, float multiply_number ) {

	mat out_mat = rowOperationMultiply(in_mat, multiply_row, multiply_number);

	for( uword j=0.0; j<in_mat.n_cols; j++ )
		out_mat( add_row, j ) += out_mat( multiply_row, j );

	for( uword i=0.0; i<in_mat.n_cols; i++ )
		out_mat(multiply_row, i) = in_mat(multiply_row, i);

	out_mat = roundToZero( out_mat );
	return out_mat;
}

mat zeroColumn( mat in_mat, uword piv_row, uword piv_col) {
	mat out_mat = in_mat;

	for( uword i=0; i<out_mat.n_rows; i++ )
		if( i != piv_row )
  			out_mat = rowOperationMultiplyAdd( out_mat, piv_row, i, 0 - out_mat( i, piv_col ) );

	out_mat = roundToZero( out_mat );
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

	uword pivot_col = 1.1;
	uword pivot_row = 1.1;
	bool exit = false;

	while( !exit )	{
 		pivot_col = findPivotCol( A, exit );
		
		if( !exit )	{
			pivot_row = findPivotRow( A, pivot_col );
			f << "Pivot is at position: ( " << pivot_row << " , " << pivot_col << " )" << endl << endl;

			float divide_by = A.at( pivot_row, pivot_col );	
			A = rowOperationMultiply(A,  pivot_row, 1.0/divide_by );
			f << A << endl;
		
			A = zeroColumn( A, pivot_row, pivot_col );
			f << A;
		}
		else {
			 f << "The maximized matrix is: " << endl << A;
		}
	}

	return 0;
}