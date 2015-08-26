/*
 * Symphony Toolbox
 * Functions for setting the coefficients of the objective and the sense (minimization/maximization)
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

int sci_sym_setObjCoeff(char *fname){
	
	//error management variable
	SciErr sciErr;
	int iRet;
	
	//data declarations
	int *varAddress,varIndex,numVars;
	double inputDouble,newCoeff;
	
	//ensure that environment is active
	if(global_sym_env==NULL){
		sciprint("Error: Symphony environment not initialized. Please run 'sym_open()' first.\n");
		return 1;
	}
	
	//code to check arguments and get them
	CheckInputArgument(pvApiCtx,2,2) ;
	CheckOutputArgument(pvApiCtx,1,1) ;
	
	//get argument 1: index of variable whose coefficient is to be changed
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
	
	//get argument 2: new coefficient
	if(getDoubleFromScilab(2,&newCoeff))
		return 1;
	
	iRet=sym_set_obj_coeff(global_sym_env,varIndex,newCoeff);
	if(iRet==FUNCTION_TERMINATED_ABNORMALLY){
		Scierror(999, "An error occured. Has a problem been loaded?\n");
		return 1;
	}else{
		sciprint("Coefficient successfully changed.\n");
	}
	
	//code to give output
	if(return0toScilab())
		return 1;
	
	return 0;
}

int sci_sym_setObjSense(char *fname){
	
	//error management variable
	SciErr sciErr;
	int iRet;
	
	//data declarations
	int *varAddress;
	double objSense;
	
	//ensure that environment is active
	if(global_sym_env==NULL){
		sciprint("Error: Symphony environment not initialized. Please run 'sym_open()' first.\n");
		return 1;
	}
	
	//code to check arguments and get them
	CheckInputArgument(pvApiCtx,1,1) ;
	CheckOutputArgument(pvApiCtx,1,1) ;
	
	//code to process input
	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &varAddress);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	if ( !isDoubleType(pvApiCtx,varAddress) ||  isVarComplex(pvApiCtx,varAddress) )
	{
		Scierror(999, "Wrong type for input argument #1:\nEither 1 (sym_minimize) or -1 (sym_maximize) is expected.\n");
		return 1;
	}
	iRet = getScalarDouble(pvApiCtx, varAddress, &objSense);
	if(iRet || (objSense!=-1 && objSense!=1))
	{
		Scierror(999, "Wrong type for input argument #1:\nEither 1 (sym_minimize) or -1 (sym_maximize) is expected.\n");
		return 1;
	}
	iRet=sym_set_obj_sense(global_sym_env,objSense);
	if(iRet==FUNCTION_TERMINATED_ABNORMALLY){
		Scierror(999, "An error occured.\n");
		return 1;
	}else{
		if(objSense==1)
			sciprint("The solver has been set to minimize the objective.\n");
		else
			sciprint("The solver has been set to maximize the objective.\n");
	}
	
	//code to give output
	if(return0toScilab())
		return 1;
	
	return 0;
}

}
