/*
 * Symphony Toolbox
 * Provides information about variables: is it continuous/integer/boolean?
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

//error management variable
static SciErr sciErr;
static int iRet;

//data declarations
static int *varAddress,varIndex,numVars,retVal;
static double inputDouble;

static int checkNumArgs()
{
	CheckInputArgument(pvApiCtx,1,1);
	CheckOutputArgument(pvApiCtx,1,1);
	return 1;
}

static int commonCodePart1(){
	//ensure that environment is active
	if(global_sym_env==NULL){
		sciprint("Error: Symphony environment not initialized. Please run 'sym_open()' first.\n");
		return 1;
	}
	
	//code to check arguments and get them
	if(checkNumArgs()==0)
		return 1;
	
	//code to process input
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
	
	return 0;
}

static int commonCodePart2(){
	if(returnDoubleToScilab(retVal))
		return 1;
	
	return 0;
}

int sci_sym_isContinuous(char *fname){
	
	if(commonCodePart1())
		return 1;
	
	iRet=sym_is_continuous(global_sym_env,varIndex,&retVal);
	if(iRet==FUNCTION_TERMINATED_ABNORMALLY){
		Scierror(999, "An error occured. Has a problem been loaded?\n");
		return 1;
	}else{
		if(retVal)
			sciprint("This variable is continuous.\n");
		else
			sciprint("This variable is not continuous.\n");
	}
	
	if(commonCodePart2())
		return 1;
	
	return 0;
}

int sci_sym_isBinary(char *fname){
	
	if(commonCodePart1())
		return 1;

	iRet=sym_is_binary(global_sym_env,varIndex,&retVal);
	if(iRet==FUNCTION_TERMINATED_ABNORMALLY){
		Scierror(999, "An error occured.\n");
		return 1;
	}else{
		if(retVal)
			sciprint("This variable is constrained to be binary.\n");
		else
			sciprint("This variable is not constrained to be binary.\n");
	}
	
	if(commonCodePart2())
		return 1;
	
	return 0;
}

int sci_sym_isInteger(char *fname){
	
	char retValc; //for some wierd reason this function unlike the above 2 returns a char
	
	if(commonCodePart1())
		return 1;
	
	iRet=sym_is_integer(global_sym_env,varIndex,&retValc);
	if(iRet==FUNCTION_TERMINATED_ABNORMALLY){
		Scierror(999, "An error occured.\n");
		return 1;
	}else{
		if(retValc)
			sciprint("This variable is constrained to be an integer.\n");
		else
			sciprint("This variable is not constrained to be an integer.\n");
	}
	retVal=retValc;
	
	if(commonCodePart2())
		return 1;
	
	return 0;
}

}
