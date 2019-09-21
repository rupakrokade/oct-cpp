#include<iostream>
#include <stdlib.h>
#include"fun.h"
//#include "funcdemo.h"

int main(void)
{
	FUNCCALL funcall;
	FUNCCALL *funptr = &funcall;
	funcall.n_in_arguments = 1;
	funcall.n_out_user = 1;;

	FUNCARGS ins[funcall.n_in_arguments];
	FUNCARGS *argptr = ins;

	funcall.name = "hamming";
	//ins.name2 = NULL; //Passing "" will not work


/*
	int dim = 1;
	ins.input1 = new double[dim];
	ins.size_input1[2]=dim;

	double val = 5.0;

	for(int i=0; i<dim; i++)
		ins.input1[i]=i+val;
*/

	int in_type[2] = {1,10};
	int i,j;
	double* d;
	for(i=0;i<funcall.n_in_arguments;i++)
	{
		if(in_type[i]==1)
		{	
			ins[i].n_in_rows = 1;
			ins[i].n_in_cols = 1;
			int size = (ins[i].n_in_rows)*(ins[i].n_in_cols);
			ins[i].in_data = malloc(sizeof(double)*size);
			d = (double *)ins[i].in_data;

			for(j=0;j<size;j++)
			{
				d[j] = 2.5*i+5*j+5;
				//printf("%f\n",d[j]);
			}
		}
		else if(in_type[i]==10)
		{
			ins[i].in_data = malloc(sizeof(char)*size);
			char* c = (char *)ins[i].in_data;
			int ci=0;
			while(str[ci]!='\0')
			{
				c[ci] = str[ci];
				ci++;
			}
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
		for(i=0;i<funcall.n_in_arguments;i++)
		{
			//printf("%ld : ",&ins[i].in_data);
		//	printf("%f\n",(*(double *)ins[i].in_data));
		
			free(ins[i].in_data);	
		}

		for(i=0;i<funcall.n_out_arguments;i++)
		{
			//printf("%ld : ",&ins[i].in_data);
		//	printf("%f\n",(*(double *)ins[i].in_data));
		
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
