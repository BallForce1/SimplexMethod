#include <iostream>
#include <fstream>
#include <armadillo>

using namespace std;
using namespace arma;

fstream f("dump.txt");

//Since rational numbers are not used to represent numerical values,
//numbers come very close to zero.  For neatness this function finds
//those numbers and sets the value to 0 for neatness and clarity.
mat roundToZero( mat in_mat )
{
	mat out_mat = in_mat;
	
	//Loop through each element in the matrix.
	for( uword i=0; i<out_mat.n_rows; i++ )
	{
		for( uword j=0; j<out_mat.n_cols; j++ )
		{
			if( in_mat(i, j) < .000001 && in_mat(i, j) > -.000001  )
			{
				out_mat(i,j) = 0;
			}
		}
	}
	return out_mat;
}

//Returns the index of the pivot column if it exists.
//Returns NULL if no negative exists in the objective row.
uword findPivotCol( mat in_mat, bool& no_negative )
{
	uword col_index;

	//Find the lowest number in the matxrix.
	float most_negative = 10000000;

	//Loop through each column except looking at the objective row
	for( uword i=0.0; i<in_mat.n_cols; i++ )
	{			
		if( in_mat( in_mat.n_rows-1, i ) < most_negative )
		{
			most_negative = in_mat(in_mat.n_rows-1 , i );
			col_index = i;
		}
	}	

	//if the matrix is greater than 0 and exists in the objective row.
	if(most_negative < 0 )
		return col_index;

	no_negative = true;		//Simplex method complete.	
}

//Returns the index of the pivot column if the pivot
//column exists.
uword findPivotRow( mat in_mat, uword piv_col )
{
	float min_value = 1000000.0;
		
	uword row_index;
		
	//Loop through each row except the objective row
	for( uword i=0.0; i<in_mat.n_rows-1; i++ )
	{			
		//Using the pivot column divide the last element in each row
		//by the element in the respective pivot column. 
		float temp = in_mat.at( i, in_mat.n_cols-1 ) / in_mat.at( i, piv_col );
			
		//Store the lowest value and index.
		if( temp <= min_value )
		{
			min_value = temp;
			row_index = i;
		}
	}	
	return row_index;

}

//Creates a new matrix indentical to the matrix given but each
//element in the row given is multiplied by a some number.
mat rowOperationMultiply( mat in_mat, uword row, float number )
{
	mat out_mat = in_mat;

	//For each element in the row multiply it by some number.
	for( uword j=0.0; j<in_mat.n_cols; j++ )
	{
			out_mat( row, j ) = in_mat( row, j ) * number;
	}
	out_mat = roundToZero( out_mat );
	return out_mat;
}

//Similar to rowOperationMultiply however a row is multiplied by a factor
//and added to another row.  Note that the row that was multiplied by is
//returned to its original values.
mat rowOperationMultiplyAdd( mat in_mat, uword multiply_row, uword add_row, float multiply_number )
{
	//Apply multiplication factor.
	mat out_mat = rowOperationMultiply(in_mat, multiply_row, multiply_number);

	//Add each element in the multiply row to the add row
	for( uword j=0.0; j<in_mat.n_cols; j++ )
	{
		out_mat( add_row, j ) += out_mat( multiply_row, j );
	}

	//Replace the origional multiply_row 
	for( uword i=0.0; i<in_mat.n_cols; i++ )
	{
		out_mat(multiply_row, i) = in_mat(multiply_row, i);
	}

	out_mat = roundToZero( out_mat );
	return out_mat;
}

//Given the pivot row and pivot column, loops through each
//row (except the pivot row) and performs some row operation
//to produce zeros in each element in the pivot column (except
//the pivot row).
mat zeroColumn( mat in_mat, uword piv_row, uword piv_col) 
{
	mat out_mat = in_mat;

	//Loop through each row (except the pivot row) and produce
	//a zero in each pivot column.
	for( uword i=0; i<out_mat.n_rows; i++ )
	{
		if( i != piv_row )
		{
  			out_mat = rowOperationMultiplyAdd( out_mat, piv_row, i, 0 - out_mat( i, piv_col ) );
		}
	}
	out_mat = roundToZero( out_mat );
	return out_mat;
}

int main()
{
	mat A;

	//A << 1 << 1 << 1 << 0 << 4 << endr
	//	<< 2 << 1 << 0 << 1 << 5 << endr
	//	<< -3 << -4 << 0 << 0 << 0 << endr;

	//A << 1 << 3 << 2 << 1 << 0 << 10 << endr
	//	<< 1 << 5 << 1 << 0 << 1 << 8 << endr
	//	<< -8 << -10 << -7 << 0 << 0 << 0 << endr;
	
	A << 2 << 1 << 1 << 1 << 0 << 0 << 14 << endr
	  << 4 << 2 << 3 << 0 << 1 << 0 << 28 << endr
	  << 2 << 5 << 5 << 0 << 0 << 1 << 30 << endr
	  << -1 << -2 << 1 << 0 << 0 << 0 << 0 << endr;


	//A << 1 << 1 << 3 << 1 << 0 << 0 << 30 << endr
	//	<< 2 << 2 << 5 << 0 << 1 << 0 << 24 << endr
	//	<< 4 << 1 << 2 << 0 << 0 << 1 << 36 << endr
	//	<< -3 << -1 << -2 << 0 << 0 << 0 << 0 << endr;

	f << A;
	uword pivot_col = 1.1;
	uword pivot_row = 1.1;
	bool exit = false;

	while( !exit )
	{
 		pivot_col = findPivotCol( A, exit );
		
		if( !exit )
		{
			pivot_row = findPivotRow( A, pivot_col );
			f << "Pivot is at position: ( " << pivot_row << " , " << pivot_col << " )" << endl << endl;

			float divide_by = A.at( pivot_row, pivot_col );	
			A = rowOperationMultiply(A,  pivot_row, 1.0/divide_by );
			f << A << endl;
		
			A = zeroColumn( A, pivot_row, pivot_col );
			f << A;
		}
		else
		{
			 f << "The maximized matrix is: " << endl << A;
		}
	}

	//system("pause");
	return 0;
}