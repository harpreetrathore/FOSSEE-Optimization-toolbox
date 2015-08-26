/*
 * Symphony Toolbox
 * Functions for modifying constraints
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

int sci_sym_setConstrBound(char *fname){
	
	//error management variable
	SciErr sciErr;
	int iRet;
	
	//data declarations
	int *varAddress,conIndex,numConstr;
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
	
	//get argument 1: index of constraint whose bound is to be changed
	if(getUIntFromScilab(1,&conIndex))
		return 1;
	iRet=sym_get_num_rows(global_sym_env,&numConstr);
	if(iRet==FUNCTION_TERMINATED_ABNORMALLY){
		Scierror(999, "An error occured. Has a problem been loaded?\n");
		return 1;
	}else if(conIndex>=numConstr){
		Scierror(999, "An error occured. Constraint index must be a number between 0 and %d.\n",numConstr-1);
		return 1;
	}
	
	//get argument 2: new bound
	if(getDoubleFromScilab(2,&newBound))
		return 1;
	
	//decide which function to execute
	isLower=(strcmp(fname,"sym_setConstrLower")==0);
	if(isLower)
		iRet=sym_set_row_lower(global_sym_env,conIndex,newBound);
	else
		iRet=sym_set_row_upper(global_sym_env,conIndex,newBound);
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

int sci_sym_setConstrType(char *fname){
	
	//error management variable
	SciErr sciErr;
	int iRet;
	
	//data declarations
	int *varAddress,conIndex,numConstr;
	double inputDouble,conRHS,conRHS2,conRange;
	bool isRangedConstr=false;
	char conType,*conTypeInput;
	
	//ensure that environment is active
	if(global_sym_env==NULL){
		sciprint("Error: Symphony environment not initialized. Please run 'sym_open()' first.\n");
		return 1;
	}
	
	//code to check arguments and get them
	CheckInputArgument(pvApiCtx,3,4) ;
	CheckOutputArgument(pvApiCtx,1,1) ;
	
	//get argument 1: index of constraint whose type is to be changed
	if(getUIntFromScilab(1,&conIndex))
		return 1;
	iRet=sym_get_num_rows(global_sym_env,&numConstr);
	if(iRet==FUNCTION_TERMINATED_ABNORMALLY){
		Scierror(999, "An error occured. Has a problem been loaded?\n");
		return 1;
	}else if(conIndex>=numConstr){
		Scierror(999, "An error occured. Constraint index must be a number between 0 and %d.\n",numConstr-1);
		return 1;
	}
	
	//get argument 2: type of constraint
	sciErr = getVarAddressFromPosition(pvApiCtx, 2, &varAddress);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	if ( !isStringType(pvApiCtx,varAddress) )
	{
		Scierror(999, "Wrong type for input argument #2: Either \"L\", \"E\", \"G\", or \"R\" is expected.\n");
		return 1;
	}
	iRet = getAllocatedSingleString(pvApiCtx, varAddress, &conTypeInput);
	if(iRet)
	{
		Scierror(999, "Wrong type for input argument #2: Either \"L\", \"E\", \"G\", or \"R\" is expected.\n");
		return 1;
	}
	switch(conTypeInput[0]){
		case 'l':case 'L':
			conType='L';
			break;
		case 'e':case 'E':
			conType='E';
			break;
		case 'g':case 'G':
			conType='G';
			break;
		case 'r':case 'R':
			conType='R';
			isRangedConstr=true;
			break;
		default:
			Scierror(999, "Wrong type for input argument #2: Either \"L\", \"E\", \"G\", or \"R\" is expected.\n");
			return 1;
	}
	//check number of arguments for specific cases
	if(isRangedConstr){
		if(nbInputArgument(pvApiCtx)!=4){
			Scierror(999, "4 Arguments are expected for ranged constraint.\n");
			return 1;
		}
	}else if(nbInputArgument(pvApiCtx)!=3){
		Scierror(999, "3 Arguments are expected for non-ranged constraint.\n");
		return 1;
	}
	
	//get argument 3: constraint RHS
	if(getDoubleFromScilab(3,&conRHS))
		return 1;
	
	//get argument 4: constraint range
	if(isRangedConstr){
		if(getDoubleFromScilab(4,&conRHS2))
			return 1;
		//conRHS should be the upper bound, and conRange should be positive
		if(conRHS>=conRHS2){
			conRange=conRHS-conRHS2;
		}else{
			conRange=conRHS2-conRHS;
			conRHS=conRHS2;
		}
	}else{
		//if not ranged constraint, just set it to 0, the value probably does not matter anyway
		conRange=0;
	}
	
	iRet=sym_set_row_type(global_sym_env,conIndex,conType,conRHS,conRange);
	if(iRet==FUNCTION_TERMINATED_ABNORMALLY){
		Scierror(999, "An error occured. Has a problem been loaded?\n");
		return 1;
	}else{
		sciprint("Constraint successfully changed.\n");
	}
	
	//code to give output
	if(return0toScilab())
		return 1;
	
	return 0;
}

}
