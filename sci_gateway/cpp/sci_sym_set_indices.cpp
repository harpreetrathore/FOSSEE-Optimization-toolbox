/*
 * Implementation Symphony Tool Box for Scilab
 * sci_sym_set_indices.cpp
 * contains functions for setting index variables as continuous and integer values
 * By Iswarya
 */
#include <symphony.h>
#include "sci_iofunc.hpp"

extern sym_environment* global_sym_env;//defined in globals.cpp

extern "C" {
#include <api_scilab.h>
#include <Scierror.h>
#include <BOOL.h>
#include <localization.h>
#include <sciprint.h>

//This function is for setting a index variable to be continuous
int sci_sym_set_continuous(char *fname, unsigned long fname_len){

	// Error management variable
	SciErr sciErr;
	double status=1.0;//assume error status
	int index;//to indicate the index of the variable to be set continuous
	int output=0;//out parameter for the load mps function
	
	CheckInputArgument(pvApiCtx, 1, 1);//Check we have exactly one argument as input or not
	CheckOutputArgument(pvApiCtx, 1, 1);//Check we have exactly one argument on output side or not

	getUIntFromScilab(1,&index);

	//ensure that environment is active
	if(global_sym_env==NULL){
		sciprint("Error: Symphony environment not initialized. Please run 'sym_open()' first.\n");
	}
	else
	{
		output=sym_set_continuous(global_sym_env,index);//setting the variable continuous
		if(output==FUNCTION_TERMINATED_ABNORMALLY)
		{
			status=1.0;//function did not invoke successfully
			sciprint("An error occured.\n");
		}
		else if(output==FUNCTION_TERMINATED_NORMALLY)
		{
			status=0.0;//no error in executing the function
			sciprint("Variable with index %d is now continuous.\n",index);
		}
	}

	if(returnDoubleToScilab(status))
		return 1;

	return 0;
}


//This function is for setting a index variable to be integer
int sci_sym_set_integer(char *fname, unsigned long fname_len){

	// Error management variable
	SciErr sciErr;
	double status=1.0;//assume error status
	int index;//to indicate the index of the variable to be set continuous
	int output=0;//out parameter for the load mps function
	
	CheckInputArgument(pvApiCtx, 1, 1);//Check we have exactly one argument as input or not
	CheckOutputArgument(pvApiCtx, 1, 1);//Check we have exactly one argument on output side or not

	getUIntFromScilab(1,&index);

	//ensure that environment is active
	if(global_sym_env==NULL){
		sciprint("Error: Symphony environment not initialized. Please run 'sym_open()' first.\n");
	}
	else
	{
		output=sym_set_integer(global_sym_env,index);//setting the variable continuous
		if(output==FUNCTION_TERMINATED_ABNORMALLY)
		{
			status=1.0;//function did not invoke successfully
			sciprint("An error occured.\n");
		}
		else if(output==FUNCTION_TERMINATED_NORMALLY)
		{
			status=0.0;//no error in executing the function
			sciprint("Variable with index %d is now constrained to be an integer.\n",index);
		}
	}

	if(returnDoubleToScilab(status))
		return 1;

	return 0;
}

}

