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
			char c[20];
			char fun_name[20];
			char pkg_name[20];
			char str_list[funcall->n_in_arguments][20];
			Matrix mIn;
			int string_count = 0;
			
			for(l=0;l<funcall->n_in_arguments;l++)
			{
				if(ins[l].type == TYPE_DOUBLE)
				{
					std::cout << "here in double l =  " << l << "\n";
					if(ins[l].n_in_rows!=0 && ins[l].n_in_cols!=0)
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
					in(l-string_count) = inMatrix_x;
					mIn(in(l-string_count).matrix_value());
					std::cout << "printing l in matrix: " << l << "\n";
					std::cout << "printing input" << in(l-string_count).matrix_value() << "\n";
					}
				}
				else if(ins[l].type == TYPE_STRING)
				{
					if(l<2)
					{
						string_count++;
					}
					std::cout << "here in string l =  " << l << "\n";
					int size_str = (ins[l].n_in_rows)*(ins[l].n_in_cols);
					char* str1 = (char *)ins[l].in_data;
					std::cout << "str size: " << strlen(str1) <<'\n';
					int ci;
					
					//strcpy(c,str1);
					if(l==0)
					{
						std::cout << "here l =  " << l << "\n";
						strcpy(fun_name,str1);
						std::cout << "feval is =  " << fun_name << "\n";
					}
					else if(l==1)
					{
						std::cout << "here l =  " << l << "\n";
						strcpy(pkg_name,str1);
						std::cout << "Loading package: " << pkg_name << "\n";
						octave::feval ("pkg", ovl("load", pkg_name), 0);
					}
					else if(l>1)
					{
						/*for(ci=0;ci<size_str;ci++)
						{
							c[ci] = str1[ci];
							std::cout << "c>: " << c[ci] <<'\n';
						}
						c[size_str]='\0';*/

							//in(l) = c;
							strcpy(str_list[0],str1);
							in(l-string_count) = str_list[0];
							std::cout << "c: " << in(l-string_count).string_value() <<'\n';
					}
				}
			}




		
//			if(inp->name2)
//			{
//				in(1) = inp->name2;
//			}
			std::cout << "feval before call is =  " << fun_name << "\n";
std::cout << "input before call is = " << in(0).matrix_value() << "\n";
			//std::cout << "optional string input before call is =  " << in(2).string_value() << "\n";
			octave_value_list out = octave::feval (fun_name, in, 1);
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
