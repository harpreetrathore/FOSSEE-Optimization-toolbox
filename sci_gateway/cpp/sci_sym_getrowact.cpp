/*
 * Symphony Toolbox
 * Function to get the row activity after solving
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

int sci_sym_getRowActivity(char *fname){
	
	//error management variable
	SciErr sciErr;
	int iRet;
	
	//data declarations
	int numConstr;
	double *rowAct;
	
	//ensure that environment is active
	if(global_sym_env==NULL){
		sciprint("Error: Symphony environment not initialized. Please run 'sym_open()' first.\n");
		return 1;
	}
	
	//code to check arguments and get them
	CheckInputArgument(pvApiCtx,0,0) ;
	CheckOutputArgument(pvApiCtx,1,1) ;
	
	//code to process input
	iRet=sym_get_num_rows(global_sym_env,&numConstr);
	if(iRet==FUNCTION_TERMINATED_ABNORMALLY){
		Scierror(999, "An error occured. Has the problem been solved? Is the problem feasible?\n");
		return 1;
	}
	rowAct=new double[numConstr];
	iRet=sym_get_row_activity(global_sym_env,rowAct);
	if(iRet==FUNCTION_TERMINATED_ABNORMALLY){
		Scierror(999, "An error occured. Has the problem been solved? Is the problem feasible?\n");
		delete[] rowAct;
		return 1;
	}
	
	//code to give output
	sciErr=createMatrixOfDouble(pvApiCtx,nbInputArgument(pvApiCtx)+1,numConstr,1,rowAct);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		delete[] rowAct;
		return 1;
	}
	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1;
	//ReturnArguments(pvApiCtx);
	
	delete[] rowAct;
	
	return 0;
}

}
