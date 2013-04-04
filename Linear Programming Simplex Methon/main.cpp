#include <iostream>
#include <armadillo>

using namespace std;
using namespace arma;

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

int main()
{
	//cout << "Hello World" << endl;

	mat A;

	A << 2 << 1 << 1 << 1 << 0 << 0 << 14 << endr
	  << 4 << 2 << 3 << 0 << 1 << 0 << 28 << endr
	  << 2 << 5 << 5 << 0 << 0 << 1 << 30 << endr
	  << -1 << -2 << 1 << 0 << 0 << 0 << 0 << endr;

	uword pivot_col = findPivotCol( A );
	uword pivot_row = findPivotRow( A, pivot_col );

	//if( pivot_col != NULL)
	//{
	//	float divide_by = A.at( pivot_row, pivot_col );
	//	A.each_col() = divide_by;
	//	
	//}



	system("pause");
	return 0;
}