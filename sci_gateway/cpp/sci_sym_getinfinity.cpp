/*
 * Symphony Toolbox
 * Provides the Symphony infinity value
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

int sci_sym_getInfinity(char *fname){
	
	//error management variable
	SciErr sciErr;
	int iRet;
	
	//code to check arguments and get them
	CheckInputArgument(pvApiCtx,0,0) ;
	CheckOutputArgument(pvApiCtx,1,1) ;
	
	//code to give output
	if(returnDoubleToScilab(sym_get_infinity()))
		return 1;
	
	return 0;
}

}
