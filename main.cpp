#include<iostream>
#include <stdlib.h>
#include <string.h>
#include"fun.h"
//#include "funcdemo.h"

int main(void)
{
	FUNCCALL funcall;
	FUNCCALL *funptr = &funcall;
	funcall.n_in_arguments = 2;
	funcall.n_out_user = 1;;

	FUNCARGS ins[funcall.n_in_arguments];
	FUNCARGS *argptr = ins;

	//funcall.name = "hamming";
	//funcall.package = "";
	//ins.name2 = NULL; //Passing "" will not work


/*
	int dim = 1;
	ins.input1 = new double[dim];
	ins.size_input1[2]=dim;

	double val = 5.0;

	for(int i=0; i<dim; i++)
		ins.input1[i]=i+val;
*/

	int in_type[4] = {10,1,10};
	int i,j;
	double* d;
	int size;
	//char in_str[20] = ["scs","sdd"];
	char str[20] = "det";
	char str1[20] = "pkg";
char str2[20] = "periodic";
	char* c;
	for(i=0;i<funcall.n_in_arguments;i++)
	{
		if(in_type[i]==1)
		{	
			std::cout << "here in main double" << in_type[i] <<'\n';
			ins[i].type = TYPE_DOUBLE;
			ins[i].n_in_rows = 2;
			ins[i].n_in_cols = 2;
			size = (ins[i].n_in_rows)*(ins[i].n_in_cols);
			ins[i].in_data = malloc(sizeof(double)*size);
			d = (double *)ins[i].in_data;

			for(j=0;j<size;j++)
			{
				d[j] = 2*i+5*j+5;
				//printf("%f\n",d[j]);
			}
		}
		else if(in_type[i]==10)
		{
			if(i>=0)
			{
				std::cout << "here in String" <<'\n';
				ins[i].type = TYPE_STRING;
				ins[i].n_in_rows = 1;
				ins[i].n_in_cols = 20;
				size = (ins[i].n_in_rows)*(ins[i].n_in_cols);
				ins[i].in_data = malloc(sizeof(char)*size);
				c = (char *)ins[i].in_data;
				int ci=0;

				if(i==0)
				{
					while(str[ci]!='\0')
					{
						c[ci] = str[ci];
						ci++;
					}
				}

				if(i==1)
				{
					while(str1[ci]!='\0')
					{
						c[ci] = str1[ci];
						ci++;
					}
				}

				if(i>1)
				{
					while(str2[ci]!='\0')
					{
						c[ci] = str2[ci];
						ci++;
					}
				}

				ins[i].n_in_cols = strlen(c);
			}
		}
		std::cout << "here in main>>: " << c <<'\n';
	}
		for(i=0;i<funcall.n_in_arguments;i++)
		{
			//printf("%ld : ",&ins[i].in_data);
			//printf("%f\n",(*(double *)ins[i].in_data));
		
			//free(ins[i].in_data);	
		}


	int status_fun = fun(argptr, funptr);

	if(status_fun==1)
	{
		std::cout<< "Octave function failed" << std::endl;
		return 1;
	}
	else
	{

std::cout << "input  ins[i].n_in_rows is: " << ins[i].n_in_rows << '\n';
std::cout << "input ins[i].n_in_cols is: " << ins[i].n_in_cols << '\n';

std::cout << "input  ins[i].n_out_rows is: " << ins[i].n_out_rows << '\n';
std::cout << "input ins[i].n_out_cols is: " << ins[i].n_out_cols << '\n';

		for(i=0;i<(ins[i].n_in_rows*ins[i].n_in_cols);i++)
		{
			//printf("%ld : ",&ins[i].in_data);
			printf("%f\n",*((double *)ins[0].in_data+i));
		
			free(ins[i].in_data);	
		}

		for(i=0;i<(ins[i].n_out_rows*ins[i].n_out_cols);i++)
		{
			//printf("%ld : ",&ins[i].out_data);
			printf("%f\n",*((double *)ins[0].in_data+i));
		
			free(ins[i].out_data);	
		}

//		for(int i=0; i<ins.size_output1[1]; i++)
//		{
//			std::cout<< i+1 <<"\t" << ins.output1[i] << std::endl;
//		}

//		free(ins.output1);
//		free(ins.input1);
	}
	return 0;
}
