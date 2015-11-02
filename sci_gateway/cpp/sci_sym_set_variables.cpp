/*
 * Implementation Symphony Tool Box for Scilab
 * set_variables.cpp
 * contains function for setting environment variables to their default and userdefined values in symphony
 * By Iswarya
 */
#include <symphony.h>

extern sym_environment* global_sym_env;//defined in globals.cpp

extern "C" {
#include <api_scilab.h>
#include <Scierror.h>
#include <BOOL.h>
#include <localization.h>
#include <sciprint.h>
//This function is for loading a mps file to symphony
int sci_sym_set_defaults(char *fname, unsigned long fname_len){
	
	double status=1.0;//assume error status
	int *piAddressVarOne = NULL;//pointer used to access argument of the function
	int output=0;//out parameter for the setting of default values  function
	CheckInputArgument(pvApiCtx, 0, 0);//Check we no argument as input or not
	CheckOutputArgument(pvApiCtx, 1, 1);//Check we have exactly one argument on output side or not

	//ensure that environment is active
	if(global_sym_env==NULL){
		sciprint("Error: Symphony environment not initialized. Please run 'sym_open()' first.\n");
		}
	else {
		output=sym_set_defaults(global_sym_env);//setting all environment variables and parameters in this symphony environment passed to their default values
		if(output==FUNCTION_TERMINATED_ABNORMALLY)
		{
			status=1.0;//function did not invoke successfully
			sciprint("Function terminated abnormally, didnot execute");
		}
		else if(output==FUNCTION_TERMINATED_NORMALLY)
		{
			status=0.0;//no error in executing the function
			sciprint("Function executed successfully");
		}
		
		
		}
	
	int e=createScalarDouble(pvApiCtx,nbInputArgument(pvApiCtx)+1,status);
	if (e){
		AssignOutputVariable(pvApiCtx, 1) = 0;
		return 1;
		}

	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
	//ReturnArguments(pvApiCtx);

	return 0;
	}



int sci_sym_set_int_param(char *fname, unsigned long fname_len){
	
	// Error management variable
	SciErr sciErr1,sciErr2;
	double status=1.0;//assume error status
	double num;//to store and check the value obtained is pure unsigned integer
	int output;//output variable to store the return value of symphony set integer function
	int value;//to store the value of integer to be set
	int *piAddressVarOne = NULL;//pointer used to access first argument of the function
	int *piAddressVarTwo=NULL;//pointer used to access second argument of the function
	char variable_name[100];//string to hold the name of variable's value to be set
	char *ptr=variable_name;//pointer to point to address of the variable name
	CheckInputArgument(pvApiCtx, 2, 2);//Check we have exactly two argument as input or not
	CheckOutputArgument(pvApiCtx, 1, 1);//Check we have exactly no argument on output side or not

	//load address of 1st argument into piAddressVarOne
	sciErr1 = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
	sciErr2 = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
	//check whether there is an error or not.
	if (sciErr1.iErr){
        printError(&sciErr1, 0);
        return 0;
	}
	if (sciErr2.iErr){
        printError(&sciErr2, 0);
        return 0;
	}
	
	
	//read the value in that pointer pointing to variable name
	int err1=getAllocatedSingleString(pvApiCtx, piAddressVarOne, &ptr);
	//read the value of the variable to be set as a double value
	int err2=getScalarDouble(pvApiCtx, piAddressVarTwo, &num);

	//check for the integrity of integer value obtained
	if((double)(unsigned int)num!=(double)num)
		return 0;
	else
		value=(unsigned int)num;//if the value passed is an integer ,store it as an unsigned integer in value variable 

	//ensure that environment is active
	if(global_sym_env==NULL){
		sciprint("Error: Symphony environment not initialized. Please run 'sym_open()' first.\n");
		}
	else {
		output=sym_set_int_param(global_sym_env,ptr,value);//symphony function to set the variable name pointed by the ptr pointer to the integer value stored in value variable.
		if(output==FUNCTION_TERMINATED_NORMALLY){
		sciprint("setting of integer parameter function executed successfully\n");
		status=0.0;	
		}
		else if(output==FUNCTION_TERMINATED_ABNORMALLY){
			sciprint("setting of integer parameter was unsuccessful.....check your parameter and value\n");
			status=1.0;
		}
		else
			sciprint("\nerror while executing the setting integer function...check your parameter and value!!\n");
		
		}
	
	int e=createScalarDouble(pvApiCtx,nbInputArgument(pvApiCtx)+1,status);
	if (e){
		AssignOutputVariable(pvApiCtx, 1) = 0;
		return 1;
		}

	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
	//ReturnArguments(pvApiCtx);

	return 0;
	}


int sci_sym_get_int_param(char *fname, unsigned long fname_len){
	
	// Error management variable
	SciErr sciErr1;
	double status=1.0;//assume error status
	int *piAddressVarOne = NULL;//pointer used to access first argument of the function
	char variable_name[100];//string to hold the name of variable's value to be retrieved
	char *ptr=variable_name;//pointer to point to address of the variable name
	int output;//output parameter for the symphony get_int_param function
	CheckInputArgument(pvApiCtx, 1, 1);//Check we have exactly one argument as input or not
	CheckOutputArgument(pvApiCtx, 1, 1);//Check we have exactly one argument on output side or not

	//load address of 1st argument into piAddressVarOne
	sciErr1 = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);

	
	//check whether there is an error or not.
	if (sciErr1.iErr){
        printError(&sciErr1, 0);
        return 0;
	}
		
	
	//read the variable name in that pointer pointing to variable name
	int err1=getAllocatedSingleString(pvApiCtx, piAddressVarOne, &ptr);
	

	//ensure that environment is active
	if(global_sym_env==NULL){
		sciprint("Error: Symphony environment not initialized. Please run 'sym_open()' first.\n");
		}
	else {
		int a;//local variable to store the value of variable name we want to retrieve
		output=sym_get_int_param(global_sym_env,ptr,&a);//symphony function to get the value of integer parameter pointed by ptr pointer and store it in 'a' variable
		if(output==FUNCTION_TERMINATED_NORMALLY){			
			sciprint("value of integer parameter %s is :: %d\n",ptr,a);
			status=0.0;
		}
		else{
			sciprint("Unable to get the value of the parameter...check the input values!!\n");
			status=1.0;
		} 
				
		}
	
	int e=createScalarDouble(pvApiCtx,nbInputArgument(pvApiCtx)+1,status);
	if (e){
		AssignOutputVariable(pvApiCtx, 1) = 0;
		return 1;
		}

	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
	//ReturnArguments(pvApiCtx);

	return 0;
	}



int sci_sym_set_dbl_param(char *fname, unsigned long fname_len){
	
	// Error management variable
	SciErr sciErr1,sciErr2;
	double status=1.0;//assume error status
	double num;//to store the value of the double parameter to be set
	int output;//output parameter for the symphomy setting double parameter function
	int *piAddressVarOne = NULL;//pointer used to access first argument of the function
	int *piAddressVarTwo=NULL;//pointer used to access second argument of the function
	char variable_name[100];//string to hold the name of variable's value to be set
	char *ptr=variable_name;//pointer to point to address of the variable name
	CheckInputArgument(pvApiCtx, 2, 2);//Check we have exactly two argument as input or not
	CheckOutputArgument(pvApiCtx, 1, 1);//Check we have exactly no argument on output side or not

	//load address of 1st argument into piAddressVarOne
	sciErr1 = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
	sciErr2 = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
	//check whether there is an error or not.
	if (sciErr1.iErr){
        printError(&sciErr1, 0);
        return 0;
	}
	if (sciErr2.iErr){
        printError(&sciErr2, 0);
        return 0;
	}
	
	
	//read the value in that pointer pointing to variable name
	int err1=getAllocatedSingleString(pvApiCtx, piAddressVarOne, &ptr);
	//read the value of the variable to be set as a double value
	int err2=getScalarDouble(pvApiCtx, piAddressVarTwo, &num);

	//ensure that environment is active
	if(global_sym_env==NULL){
		sciprint("Error: Symphony environment not initialized. Please run 'sym_open()' first.\n");
		}
	else {
		output=sym_set_dbl_param(global_sym_env,ptr,num);//symphony function to set the variable name pointed by the ptr pointer to the double value stored in 'value' variable.
		if(output==FUNCTION_TERMINATED_NORMALLY){		
			sciprint("setting of double parameter function executed successfully\n");
			status=0.0;
		}
		else
			sciprint("Function did not execute successfully...check your inputs!!!\n");
		
		}
	
	int e=createScalarDouble(pvApiCtx,nbInputArgument(pvApiCtx)+1,status);
	if (e){
		AssignOutputVariable(pvApiCtx, 1) = 0;
		return 1;
		}

	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
	//ReturnArguments(pvApiCtx);

	return 0;
	}


int sci_sym_get_dbl_param(char *fname, unsigned long fname_len){
	
	// Error management variable
	SciErr sciErr1;
	double status=1.0;//assume error status
	int *piAddressVarOne = NULL;//pointer used to access first argument of the function
	char variable_name[100];//string to hold the name of variable's value to be retrieved
	char *ptr=variable_name;//pointer to point to address of the variable name
	int output;//output parameter for the symphony get_dbl_param function
	CheckInputArgument(pvApiCtx, 1, 1);//Check we have exactly one argument as input or not
	CheckOutputArgument(pvApiCtx, 1, 1);//Check we have exactly one argument on output side or not

	//load address of 1st argument into piAddressVarOne
	sciErr1 = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);

	
	//check whether there is an error or not.
	if (sciErr1.iErr){
        printError(&sciErr1, 0);
        return 0;
	}
		
	
	//read the variable name in that pointer pointing to variable name
	int err1=getAllocatedSingleString(pvApiCtx, piAddressVarOne, &ptr);
	

	//ensure that environment is active
	if(global_sym_env==NULL){
		sciprint("Error: Symphony environment not initialized. Please run 'sym_open()' first.\n");
		}
	else {
		double a;//local variable to store the value of variable name we want to retrieve
		output=sym_get_dbl_param(global_sym_env,ptr,&a);//symphony function to get the value of double parameter pointed by ptr pointer and store it in 'a' variable
		if(output==FUNCTION_TERMINATED_NORMALLY){
			sciprint("value of double parameter %s is :: %lf\n",ptr,a);
			status=1.0;
		}
		else{
			sciprint("Unable to get the value of the parameter...check the input values!!\n");
			status=1.0;
		} 


		}
	
	int e=createScalarDouble(pvApiCtx,nbInputArgument(pvApiCtx)+1,status);
	if (e){
		AssignOutputVariable(pvApiCtx, 1) = 0;
		return 1;
		}

	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
	//ReturnArguments(pvApiCtx);

	return 0;
	}


int sci_sym_set_str_param(char *fname, unsigned long fname_len){
	
	// Error management variable
	SciErr sciErr1,sciErr2;
	double status=1.0;//assume error status
	double num;//to store the value of the double parameter to be set
	int output;//output return value of the setting of symphony string parameter function
	int *piAddressVarOne = NULL;//pointer used to access first argument of the function
	int *piAddressVarTwo=NULL;//pointer used to access second argument of the function
	char variable_name[100],value[100];//string to hold the name of variable's value to be set and the value to be set is stored in 'value' string
	char *ptr=variable_name,*valptr=value;//pointer-'ptr' to point to address of the variable name and 'valptr' points to the address of the value to be set to the string parameter
	CheckInputArgument(pvApiCtx, 2, 2);//Check we have exactly two argument as input or not
	CheckOutputArgument(pvApiCtx, 1, 1);//Check we have exactly no argument on output side or not

	//load address of 1st argument into piAddressVarOne
	sciErr1 = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
	sciErr2 = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
	//check whether there is an error or not.
	if (sciErr1.iErr){
        printError(&sciErr1, 0);
        return 0;
	}
	if (sciErr2.iErr){
        printError(&sciErr2, 0);
        return 0;
	}
	
	
	//read the value in that pointer pointing to variable name
	int err1=getAllocatedSingleString(pvApiCtx, piAddressVarOne, &ptr);
	//read the value of the string variable to be set
	int err2=getAllocatedSingleString(pvApiCtx, piAddressVarTwo, &valptr);

	//ensure that environment is active
	if(global_sym_env==NULL){
		sciprint("Error: Symphony environment not initialized. Please run 'sym_open()' first.\n");
		}
	else {
		output=sym_set_str_param(global_sym_env,ptr,valptr);//symphony function to set the variable name pointed by the ptr pointer to the double value stored in 'value' variable.
		if(output==FUNCTION_TERMINATED_NORMALLY){
			sciprint("setting of string parameter function executed successfully\n");
			status=0.0;
		}
		else
			sciprint("Setting of the string parameter was unsuccessfull...check the input values!!\n");
		
		}
	
	int e=createScalarDouble(pvApiCtx,nbInputArgument(pvApiCtx)+1,status);
	if (e){
		AssignOutputVariable(pvApiCtx, 1) = 0;
		return 1;
		}

	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
	//ReturnArguments(pvApiCtx);

	return 0;
	}


int sci_sym_get_str_param(char *fname, unsigned long fname_len){
	
	// Error management variable
	SciErr sciErr1;
	double status=1.0;//assume error status
	int *piAddressVarOne = NULL;//pointer used to access first argument of the function
	char variable_name[100];//string to hold the name of variable's value to be retrieved
	char *ptr=variable_name;//pointer to point to address of the variable name
	int output;//output parameter for the symphony get_dbl_param function
	CheckInputArgument(pvApiCtx, 1, 1);//Check we have exactly one argument as input or not
	CheckOutputArgument(pvApiCtx, 1, 1);//Check we have exactly one argument on output side or not

	//load address of 1st argument into piAddressVarOne
	sciErr1 = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);

	
	//check whether there is an error or not.
	if (sciErr1.iErr){
        printError(&sciErr1, 0);
        return 0;
	}
		
	
	//read the variable name in that pointer pointing to variable name
	int err1=getAllocatedSingleString(pvApiCtx, piAddressVarOne, &ptr);
	

	//ensure that environment is active
	if(global_sym_env==NULL){
		sciprint("Error: Symphony environment not initialized. Please run 'sym_open()' first.\n");
		}
	else {
		char value[100];//local variable to store the value of variable name we want to retrieve
		char *p=value;//pointer to store the address of the character array that contains the value of the string parameter to be retrieved
		output=sym_get_str_param(global_sym_env,ptr,&p);//symphony function to get the value of string parameter pointed by ptr pointer and store it in 'p' pointer variable
		if(output==FUNCTION_TERMINATED_NORMALLY){
			sciprint("value of string parameter %s is :: %s\n",ptr,p);
			status=0.0;
		}
		else
			sciprint("The string parameter value could not be retrieved successfully...check the input values!!\n");

				
		}
	
	int e=createScalarDouble(pvApiCtx,nbInputArgument(pvApiCtx)+1,status);
	if (e){
		AssignOutputVariable(pvApiCtx, 1) = 0;
		return 1;
		}

	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
	//ReturnArguments(pvApiCtx);

	return 0;
	}


}
