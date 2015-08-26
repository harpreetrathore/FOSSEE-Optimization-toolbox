/*
 * Symphony Toolbox
 * <Description>
 * <Author(s)>
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

int sci_template(char *fname){
	
	//error management variable
	SciErr sciErr;
	int iRet;
	
	//data declarations
	
	//ensure that environment is active
	if(global_sym_env==NULL){
		sciprint("Error: Symphony environment not initialized. Please run 'sym_open()' first.\n");
		return 1;
	}
	
	//code to check arguments and get them
	CheckInputArgument(pvApiCtx, , ) ;
	CheckOutputArgument(pvApiCtx, , ) ;
	
	//code to process input
	
	//code to give output
	
	return 0;
}

}
