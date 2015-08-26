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

int sci_sym_getObjSense(char *fname){
	
	//error management variable
	SciErr sciErr;
	int iRet;
	
	//data declarations
	int objSense;
	
	//ensure that environment is active
	if(global_sym_env==NULL){
		sciprint("Error: Symphony environment not initialized. Please run 'sym_open()' first.\n");
		return 1;
	}
	
	//code to check arguments and get them
	CheckInputArgument(pvApiCtx,0,0) ;
	CheckOutputArgument(pvApiCtx,1,1) ;
	
	//code to give output
	iRet=sym_get_obj_sense(global_sym_env,&objSense);
	if(iRet==FUNCTION_TERMINATED_ABNORMALLY){
		Scierror(999, "An error occured. Has a problem been loaded?\n");
		return 1;
	}
	if(objSense==1)
		sciprint("Symphony has been set to minimize the objective.\n");
	else
		sciprint("Symphony has been set to maximize the objective.\n");

	if(returnDoubleToScilab(objSense))
		return 1;
	
	return 0;
}

}
