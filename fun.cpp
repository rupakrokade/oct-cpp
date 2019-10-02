#include <iostream>
#include <stdlib.h>
#include <octave/oct.h>
#include <octave/octave.h>
#include <octave/parse.h>
#include <octave/interpreter.h>
#include <math.h>
#include <string>
#include "fun.h"

extern "C"
{
	int fun(FUNCARGS *inp, FUNCCALL *funcall)
	{

		static octave::interpreter interpreter;
		bool status = interpreter.initialized();
		
		if(status==false)
		{
		interpreter.initialize ();
		int status_exec = interpreter.execute ();

			if (status_exec != 0)
			{
				std::cerr << "creating embedded Octave interpreter failed!"
						<< std::endl;
			}
		}
		
		try
		{	
			octave_value_list in;
			unsigned int k;
			int l;
			int str_count = 0;
			char str_fun[20];
			char str_pkg[20];
			int pkg=0;
			int nouts;
			for(l=0;l<funcall->n_in_arguments;l++)
			{
				if(inp[l].type==TYPE_DOUBLE)
				{
					//std::cout << "In fun double. l is : " << l << '\n';
					Matrix inMatrix_x(inp[l].n_in_rows,inp[l].n_in_cols);
					double* id = (double *)inp[l].in_data;
					k=0;
					for( unsigned int i = 0; i < inp[l].n_in_rows; i++ )
					{
						for( unsigned int j = 0; j < inp[l].n_in_cols; j++ )
						{
								inMatrix_x(i, j) = id[k];
								k++;
						}
					}
					in(l-str_count) = inMatrix_x;
				}
				else if(inp[l].type==TYPE_STRING)
				{
					//std::cout << "In fun string. l is : " << l << '\n';
					
					char* c = (char *)inp[l].in_data;
					//std::cout << "String is: " << c << '\n';
					if(l==0)
						strcpy(str_fun,c);
					else if(l==1)
					{
						strcpy(str_pkg,c);
						pkg=1;
					}					
					else
						in(l-str_count) = c;

					str_count++;
					//std::cout << "String is: " << c << '\n';
				}
			}

			if(pkg==1)
			{
				//std::cout << "loading package " << str_pkg << '\n';
				octave::feval ("pkg", ovl ("load", str_pkg), 0);
			}

			octave_value_list out = octave::feval (str_fun, in, 1);



			nouts = out.length();
			funcall->n_out_arguments = nouts;
//std::cout << "funcall->n_out_arguments is: " << funcall->n_out_arguments << '\n';

			for( unsigned int ii = 0; ii < nouts; ii++ )
			{
				Matrix mOut(out(ii).matrix_value());
				int row = mOut.rows();
				int col = mOut.columns();
				inp[ii].n_out_rows = row;
				inp[ii].n_out_cols = col;
				k=0;
				inp[ii].out_data = malloc(sizeof(double)*(row*col));
				double* dd = (double *)inp[ii].out_data;
				for(unsigned int i=0;i<row;i++)
					{
						for(unsigned int j=0;j<col;j++)
						{
							dd[k]=mOut(k);
							k++;
						}
					}
				}
		}
		catch (const octave::exit_exception& ex)
		{
			std::cerr << "Octave interpreter exited with status = "
					<< ex.exit_status () << std::endl;
			return 1;
		}
		catch (const octave::execution_exception&)
		{
			std::cerr << "error encountered in Octave evaluator!" << std::endl;
			return 1;
		}
		return 0;
	}
}
