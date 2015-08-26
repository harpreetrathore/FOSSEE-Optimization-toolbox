/*
 * Symphony Toolbox
 * Function to set the solution to a known one
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

int sci_sym_setColSoln(char *fname){
	
	//error management variable
	SciErr sciErr;
	int iRet;
	
	//data declarations
	int numVars;
	double *solution;
	
	//ensure that environment is active
	if(global_sym_env==NULL){
		sciprint("Error: Symphony environment not initialized. Please run 'sym_open()' first.\n");
		return 1;
	}
	
	//code to check arguments and get them
	CheckInputArgument(pvApiCtx,1,1) ;
	CheckOutputArgument(pvApiCtx,1,1) ;
	
	//code to process input
	iRet=sym_get_num_cols(global_sym_env,&numVars);
	if(iRet==FUNCTION_TERMINATED_ABNORMALLY){
		Scierror(999, "An error occured. Has a problem been loaded?\n");
		return 1;
	}
	
	if(getFixedSizeDoubleMatrixFromScilab(1,1,numVars,&solution))
		return 1;

	iRet=sym_set_col_solution(global_sym_env,solution);
	if(iRet==FUNCTION_TERMINATED_ABNORMALLY){
		Scierror(999, "An error occured. The given solution may be infeasible\nor worse than the current solution.\n");
		return 1;
	}
	
	//code to give output
	if(return0toScilab())
		return 1;
	
	return 0;
}

}
