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

	int fun(FUNCARGS *ins, FUNCCALL *funcall)
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
			unsigned int k;
			int l;
			octave_value_list in;
			std::cout << "inputs: " << funcall->n_in_arguments << "\n";
			for(l=0;l<funcall->n_in_arguments;l++)
			{
				if(ins[l].n_in_rows!=0 || ins[l].n_in_cols!=0)
				{

					Matrix inMatrix_x(ins[l].n_in_rows,ins[l].n_in_cols);
					double* d = (double *)ins[l].in_data;
					k=0;
					for( unsigned int i = 0; i < ins[l].n_in_rows; i++ )
					{
						for( unsigned int j = 0; j < ins[l].n_in_cols; j++ )
						{
								inMatrix_x(i, j) = d[k]; 
								k++;
						}
					}
				in(l) = inMatrix_x;
				Matrix mIn(in(l).matrix_value());
				std::cout << mIn << "\n";
				}
			}




		
//			if(inp->name2)
//			{
//				in(1) = inp->name2;
//			}

			octave_value_list out = octave::feval (funcall->name, in, 1);
			int nouts = out.length();
			funcall->n_out_arguments = nouts;
//////////////////OUTPUT///////////////////////////
		for(int l=0; l<funcall->n_out_user;l++)
		{
			if(l <= nouts)
			{
				Matrix mOut(out(l).matrix_value());

				//std::cout << mOut << "\n";

				int row = mOut.rows();
				int col = mOut.columns();
				//std::cout << "--output matrix2: " << row2 << "X" << col2 << "\n";

				ins[l].n_out_rows = row;
				ins[l].n_out_cols = col;
				//std::cout << "--output matrix2 inp->: " << inp->size_output2[1] << "X" << inp->size_output2[1] << "\n";
				int len = row*col;
				//inp->output2 = new double[len2];
				ins[l].out_data = malloc(sizeof(double)*len);
				k=0;
				double* out_d = (double *)ins[l].out_data;
				for(unsigned int i=0;i<row;i++)
					{
						for(unsigned int j=0;j<col;j++)
						{
							out_d[k]=mOut(k);
							k++;
						}
					}
					std::cout << mOut << "\n";
				}
			}
///-----------------------------------------------------
/*
				Matrix mOut(out(0).matrix_value());
				//std::cout << mOut << "\n";
				int len = mOut.numel();
				inp->size_output1[1] = len;
				inp->output1 = new double[len];

				for(int i=0; i<len; i++)
				{
					inp->output1[i] = mOut(i);
				}	*/
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
