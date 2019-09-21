#define fun_h__
//extern "C" int fun (double* answ, double* in1, int in1_row, std::string name, std::string opt);

extern "C"
{

	typedef enum
	{
		TYPE_DOUBLE,
		TYPE_STRING,
	}FUNCTYPE;

	typedef struct
	{
		FUNCTYPE type;
		void* in_data;
		void* out_data;
		int n_in_rows;
		int n_in_cols;
		int n_out_rows;
		int n_out_cols;
	}FUNCARGS;
	
	typedef struct {
	char *name; // Name
	int n_in_arguments; // number of input arguments
	int n_out_arguments; // number of output arguments
	int n_out_user; // number of output arguments
	FUNCARGS *argument;
} FUNCCALL;

	int fun(FUNCARGS *arr, FUNCCALL *call);
}
