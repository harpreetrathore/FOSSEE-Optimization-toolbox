/*
 * Implementation of Symphony Tool Box for Scilab
 * solver_status_query_functions.cpp
 * contains Solver Status Query Functions (7 functions)
 * Author: Sai Kiran
 */

#include <symphony.h>
#include <sci_iofunc.hpp>
extern sym_environment* global_sym_env;//defined in globals.cpp

extern "C" {
#include <api_scilab.h>
#include <Scierror.h>
#include <BOOL.h>
#include <localization.h>
#include <sciprint.h>
#include <string.h>

int process_ret_val(int);

/*
 * This function returns the status of problem that has been solved.
 */
int sci_sym_get_status(char *fname, unsigned long fname_len){

	int status=0;
  
	//check whether we have no input and one output argument or not
	CheckInputArgument(pvApiCtx, 0, 0) ;//no input argument
	CheckOutputArgument(pvApiCtx, 1, 1) ;//one output argument

	// Check environment
	if(global_sym_env==NULL)
		sciprint("Error: Symphony environment is not initialized.\n");
	else // There is an environment opened
		status=sym_get_status(global_sym_env);// Call function
	// Return result to scilab
	return returnDoubleToScilab(status);
	}


/* This is a generelized function for 
 * sym_isOptimal,sym_isInfeasible,sym_isAbandoned,
 * sym_isIterLimitReached,sym_isTimeLimitReached,
 * and sym_isTargetGapAchieved.
 * All the above functions have same return value and input argument.
 * 
 * It returns (to scilab) 
 * 1 if the function is proved true.
 * 0 if the function is proved false.
 * -1 if there is an error.
 */
int sci_sym_get_solver_status(char *fname, unsigned long fname_len){
	int result= -1 ;// Result to caller. Set to error.
  
	// Check whether we have no input and one output argument or not
	CheckInputArgument(pvApiCtx, 0, 0) ;// No input argument

	// One output argument (For scilab 1 o/p argument is fixed)	
	CheckOutputArgument(pvApiCtx, 1, 1) ;

	/* Array of possible callers of this function */
	char *arr_caller[]={"sym_isOptimal","sym_isInfeasible","sym_isAbandoned",
						"sym_isIterLimitReached","sym_isTimeLimitReached",
						"sym_isTargetGapAchieved" };
	
	/* Array of functions to be called */
	int (*fun[])(sym_environment *)= { sym_is_proven_optimal,
										sym_is_proven_primal_infeasible,
										sym_is_abandoned,sym_is_iteration_limit_reached,
										sym_is_time_limit_reached,sym_is_target_gap_achieved
											};
	/* Output values if functions return TRUE */
	char *output_true[] = {"The problem is solved to optimality.",
							"The problem is proved to be infeasible.",
							"The problem is abandoned.",
							"Iteration limit is reached.",
							"Time limit is reached.","Target gap is reached."};

	/* Output values if functions return FALSE */
	char *output_false[] = {"The problem is not solved to optimality.",
							"The problem is not proved to be infeasible.",
							"The problem is not abandoned.",
							"Iteration limit is not reached.",
							"Time limit is not reached.","Target gap is not reached."};
	// Check environment
	if(global_sym_env==NULL)
		sciprint("Error: Symphony environment is not initialized.\n");
	else {//there is an environment opened
		int iter = 0, length= sizeof(arr_caller) / sizeof(char *),found_at= -1;

		for (;iter < length ;++iter)
			if (!strcmp(fname,arr_caller[iter])) //Find caller
				found_at=iter;
		if (found_at != -1 ) {
			result = fun[found_at](global_sym_env);
			sciprint("\n");
			switch (result) {
				case TRUE: // TRUE = 1
					sciprint(output_true[found_at]);
					break;
				case FALSE: // FALSE = 0
					sciprint(output_false[found_at]);
					break;
				default:
					sciprint("Undefined return value.");
					result = -1;
				}
			sciprint("\n");
			}
		else // Very rare case
			sciprint("\nError in function mapping in scilab script\n");
		}
	return returnDoubleToScilab(result);
	}
}
