/*
 * Symphony Toolbox
 * Functions for setting the upper and lower bounds of the variables
 * By Keyur Joshi
 */
#include "symphony.h"
#include "sci_iofunc.hpp"

extern sym_environment* global_sym_env; //defined in globals.cpp

extern "C" {
#include "api_scilab.h"
#include "Scierror.h"
#include "sciprint.h"
#include "BOOL.h"
#include <localization.h>
#include <string.h>

int sci_sym_setVarBound(char *fname){
	
	//error management variable
	SciErr sciErr;
	int iRet;
	
	//data declarations
	int *varAddress,varIndex,numVars;
	double inputDouble,newBound;
	bool isLower;
	
	//ensure that environment is active
	if(global_sym_env==NULL){
		sciprint("Error: Symphony environment not initialized. Please run 'sym_open()' first.\n");
		return 1;
	}
	
	//code to check arguments and get them
	CheckInputArgument(pvApiCtx,2,2) ;
	CheckOutputArgument(pvApiCtx,1,1) ;
	
	//get argument 1: index of variable whose bound is to be changed
	if(getUIntFromScilab(1,&varIndex))
		return 1;
	iRet=sym_get_num_cols(global_sym_env,&numVars);
	if(iRet==FUNCTION_TERMINATED_ABNORMALLY){
		Scierror(999, "An error occured. Has a problem been loaded?\n");
		return 1;
	}else if(varIndex>=numVars){
		Scierror(999, "An error occured. Variable index must be a number between 0 and %d.\n",numVars-1);
		return 1;
	}
	
	//get argument 2: new bound
	if(getDoubleFromScilab(2,&newBound))
		return 1;
	
	//decide which function to execute
	isLower=(strcmp(fname,"sym_setVarLower")==0);
	if(isLower)
		iRet=sym_set_col_lower(global_sym_env,varIndex,newBound);
	else
		iRet=sym_set_col_upper(global_sym_env,varIndex,newBound);
	if(iRet==FUNCTION_TERMINATED_ABNORMALLY){
		Scierror(999, "An error occured. Has a problem been loaded?\n");
		return 1;
	}else{
		sciprint("Bound successfully changed.\n");
	}
	
	//code to give output
	if(return0toScilab())
		return 1;
	
	return 0;
}

}
