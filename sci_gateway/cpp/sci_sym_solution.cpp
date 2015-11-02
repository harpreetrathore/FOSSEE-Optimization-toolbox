/*
 * Symphony Toolbox
 * Provides the solution after the problem is solved
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

int sci_sym_getVarSoln(char *fname){
	
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
	CheckInputArgument(pvApiCtx,0,0) ;
	CheckOutputArgument(pvApiCtx,1,1) ;
	
	//code to process input
	iRet=sym_get_num_cols(global_sym_env,&numVars);
	if(iRet==FUNCTION_TERMINATED_ABNORMALLY){
		Scierror(999, "An error occured. Has the problem been loaded and solved?\n");
		return 1;
	}
	solution=new double[numVars];
	iRet=sym_get_col_solution(global_sym_env,solution);
	if(iRet==FUNCTION_TERMINATED_ABNORMALLY){
		//Scierror(999, "An error occured. Has the problem been solved? Is the problem feasible?\n");
		delete[] solution;
		return 1;
	}
	
	//code to give output
	sciErr=createMatrixOfDouble(pvApiCtx,nbInputArgument(pvApiCtx)+1,1,numVars,solution);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		delete[] solution;
		return 1;
	}
	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1;
	//ReturnArguments(pvApiCtx);
	
	delete[] solution;
	
	return 0;
}

int sci_sym_getObjVal(char *fname){
	
	//error management variable
	SciErr sciErr;
	int iRet;
	
	//data declarations
	double solution;
	
	//ensure that environment is active
	if(global_sym_env==NULL){
		sciprint("Error: Symphony environment not initialized. Please run 'sym_open()' first.\n");
		return 1;
	}
	
	//code to check arguments and get them
	CheckInputArgument(pvApiCtx,0,0) ;
	CheckOutputArgument(pvApiCtx,1,1) ;
	
	//code to process input
	iRet=sym_get_obj_val(global_sym_env,&solution);
	if(iRet==FUNCTION_TERMINATED_ABNORMALLY){
		Scierror(999, "An error occured. Has the problem been loaded and solved? Is the problem feasible?\n");
		return 1;
	}
	
	//code to give output
	if(returnDoubleToScilab(solution))
		return 1;
	
	return 0;
}

}
