/*
 * Implementation Symphony Tool Box for Scilab
 * template.cpp
 * contains function for loading .mps file to symphony
 * By Iswarya
 */
#include <symphony.h>
#include "sci_iofunc.hpp"

extern sym_environment* global_sym_env;//defined in globals.cpp

extern "C"{
#include <api_scilab.h>
#include <Scierror.h>
#include <BOOL.h>
#include <localization.h>
#include <sciprint.h>

//This function is for loading a mps file to symphony
int sci_sym_load_mps(char *fname, unsigned long fname_len){

	// Error management variable
	SciErr sciErr;

	//data declarations
	double status=1.0;//assume error status
	int *piAddressVarOne = NULL;//pointer used to access argument of the function
	char file[100];//string to hold the name of .mps file
	char* ptr=file;//pointer to point to address of file name
	int output=0;//out parameter for the load mps function
	CheckInputArgument(pvApiCtx, 1, 1);//Check we have exactly one argument as input or not
	CheckOutputArgument(pvApiCtx, 1, 1);//Check we have exactly one argument on output side or not

	//load address of 1st argument into piAddressVarOne
	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);

	//check whether there is an error or not.
	if (sciErr.iErr){
        printError(&sciErr, 0);
        return 1;
	}
	if ( !isStringType(pvApiCtx,piAddressVarOne) )
	{
		Scierror(999,"Wrong type for input argument 1: A file name is expected.\n");
		return 1;
	}

	//read the value in that pointer pointing to file name
	int err=getAllocatedSingleString(pvApiCtx, piAddressVarOne, &ptr);

	//ensure that environment is active
	if(global_sym_env==NULL){
		sciprint("Error: Symphony environment not initialized. Please run 'sym_open()' first.\n");
	}else{
		output=sym_read_mps(global_sym_env,ptr);//loading .mps file to symphony
		if(output==FUNCTION_TERMINATED_ABNORMALLY || output==ERROR__READING_MPS_FILE)
		{
			status=1.0;//function did not invoke successfully
			sciprint("Error while reading file\n");
		}
		else if(output==FUNCTION_TERMINATED_NORMALLY)
		{
			status=0.0;//no error in executing the function
			sciprint("File read successfully\n");
		}
	}

	if(returnDoubleToScilab(status))
		return 1;

	return 0;
}

}

