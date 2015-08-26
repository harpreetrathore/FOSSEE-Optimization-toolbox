/*
 * Symphony Toolbox
 * Functions to add a new row or column
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

int sci_sym_addConstr(char *fname){
	
	//error management variable
	SciErr sciErr;
	int iRet;
	
	//data declarations
	int *varAddress,numVars,nonZeros,*itemsPerRow,*colIndex,inputRows,inputCols,colIter;
	double inputDouble,*matrix,conRHS,conRHS2,conRange;
	char conType,*conTypeInput;
	bool isRangedConstr=false;
	
	//ensure that environment is active
	if(global_sym_env==NULL){
		sciprint("Error: Symphony environment not initialized. Please run 'sym_open()' first.\n");
		return 1;
	}
	
	//code to check arguments and get them
	CheckInputArgument(pvApiCtx,3,4) ;
	CheckOutputArgument(pvApiCtx,1,1) ;
	
	//get number of columns
	iRet=sym_get_num_cols(global_sym_env,&numVars);
	if(iRet==FUNCTION_TERMINATED_ABNORMALLY){
		Scierror(999, "An error occured. Has a problem been loaded?\n");
		return 1;
	}
	
	//get input 1: sparse matrix of variable coefficients in new constraint
	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &varAddress);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	if ( !isSparseType(pvApiCtx,varAddress) ||  isVarComplex(pvApiCtx,varAddress) )
	{
		Scierror(999, "Wrong type for input argument #1: A sparse matrix of doubles is expected.\n");
		return 1;
	}
	sciErr = getSparseMatrix(pvApiCtx,varAddress,&inputRows,&inputCols,&nonZeros,&itemsPerRow,&colIndex,&matrix);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	if(inputRows!=1 || inputCols!=numVars)
	{
		Scierror(999, "Wrong type for input argument #1: Incorrectly sized matrix.\n");
		return 1;
	}
	//scilab has 1-based column indices, convert to 0-based for Symphony
	for(colIter=0;colIter<nonZeros;colIter++)
		colIndex[colIter]--;
	
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
	
	iRet=sym_add_row(global_sym_env,nonZeros,colIndex,matrix,conType,conRHS,conRange);
	if(iRet==FUNCTION_TERMINATED_ABNORMALLY){
		Scierror(999, "An error occured.\n");
		return 1;
	}else{
		sciprint("Constraint successfully added.\n");
	}
	
	//code to give output
	if(return0toScilab())
		return 1;
	
	return 0;
}

int sci_sym_addVar(char *fname){
	
	//error management variable
	SciErr sciErr;
	int iRet;
	
	//data declarations
	int *varAddress,numConstr,nonZeros,*itemsPerRow,*colIndex,*rowIndex,inputRows,inputCols,rowIter,arrayIter,isInt;
	double inputDouble,*matrix,uBound,lBound,objCoeff;
	char *varName,isIntChar;
	
	//ensure that environment is active
	if(global_sym_env==NULL){
		sciprint("Error: Symphony environment not initialized. Please run 'sym_open()' first.\n");
		return 1;
	}
	
	//code to check arguments and get them
	CheckInputArgument(pvApiCtx,6,6) ;
	CheckOutputArgument(pvApiCtx,1,1) ;
	
	//get number of rows
	iRet=sym_get_num_rows(global_sym_env,&numConstr);
	if(iRet==FUNCTION_TERMINATED_ABNORMALLY){
		Scierror(999, "An error occured. Has a problem been loaded?\n");
		return 1;
	}
	
	//get input 1: sparse matrix of new variable coefficients constraints
	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &varAddress);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	if ( !isSparseType(pvApiCtx,varAddress) ||  isVarComplex(pvApiCtx,varAddress) )
	{
		Scierror(999, "Wrong type for input argument #1: A sparse matrix of doubles is expected.\n");
		return 1;
	}
	sciErr = getSparseMatrix(pvApiCtx,varAddress,&inputRows,&inputCols,&nonZeros,&itemsPerRow,&colIndex,&matrix);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	if(inputRows!=numConstr || inputCols!=1)
	{
		Scierror(999, "Wrong type for input argument #1: Incorrectly sized matrix.\n");
		return 1;
	}
	
	//get argument 2: lower bound of new variable
	if(getDoubleFromScilab(2,&lBound))
		return 1;
	
	//get argument 3: upper bound of new variable
	if(getDoubleFromScilab(3,&uBound))
		return 1;
	
	//get argument 4: coefficient of new variable in objective
	if(getDoubleFromScilab(4,&objCoeff))
		return 1;
	
	//get argument 5: wether the variable is constrained to be an integer
	sciErr = getVarAddressFromPosition(pvApiCtx, 5, &varAddress);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	if ( !isBooleanType(pvApiCtx, varAddress) )
	{
		Scierror(999, "Wrong type for input argument #5: A boolean is expected.\n");
		return 1;
	}
	iRet = getScalarBoolean(pvApiCtx, varAddress, &isInt);
	if(iRet)
	{
		Scierror(999, "Wrong type for input argument #5: A boolean is expected.\n");
		return 1;
	}
	isIntChar=isInt?TRUE:FALSE;
	
	//get argument 6: name of new variable
	sciErr = getVarAddressFromPosition(pvApiCtx, 6, &varAddress);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	if ( !isStringType(pvApiCtx,varAddress) )
	{
		Scierror(999, "Wrong type for input argument #6: A string is expected.\n");
		return 1;
	}
	iRet = getAllocatedSingleString(pvApiCtx, varAddress, &varName);
	if(iRet)
	{
		Scierror(999, "Wrong type for input argument #6: A string is expected.\n");
		return 1;
	}
	
	//convert to form required by Symphony
	rowIndex=new int[nonZeros];
	for(rowIter=0,arrayIter=0;rowIter<numConstr;rowIter++)
		if(itemsPerRow[rowIter])
			rowIndex[arrayIter++]=rowIter;
	
	iRet=sym_add_col(global_sym_env,nonZeros,rowIndex,matrix,lBound,uBound,objCoeff,isIntChar,varName);
	delete[] rowIndex;
	if(iRet==FUNCTION_TERMINATED_ABNORMALLY){
		Scierror(999, "An error occured.\n");
		return 1;
	}else{
		sciprint("Variable successfully added.\n");
	}
	
	//code to give output
	if(return0toScilab())
		return 1;
	
	return 0;
}

}
