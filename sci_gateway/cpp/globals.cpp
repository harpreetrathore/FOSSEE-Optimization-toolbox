/*
 * Implementation Symphony Tool Box for Scilab
 * globals.cpp
 * contains definitions of global variables and functions
 * By Keyur Joshi and Sai Kiran
 */
#include <symphony.h>
#include <sciprint.h>

sym_environment *global_sym_env=0;

extern "C"{
int process_ret_val(int ret_val){
	int status=0;
	sciprint("\n");
	switch(ret_val){
		case TM_NO_PROBLEM:
			sciprint("No problem has been loaded.");
			break;
		case TM_NO_SOLUTION:
			sciprint("This problem is infeasible.");
			break;
		case TM_FINISHED:
			sciprint("The solver has finished working.");
			break;
		case TM_UNFINISHED:
			sciprint("The solver has NOT finished working.");
			break;
		case TM_FEASIBLE_SOLUTION_FOUND:
			sciprint("A feasible solution has been found. It may not be optimal.");
			break;
		case TM_SIGNAL_CAUGHT:
			sciprint("TM_SIGNAL_CAUGHT");
			break;
		case TM_UNBOUNDED:
			sciprint("This problem or its solution is unbounded.");
			break;
		case PREP_OPTIMAL_SOLUTION_FOUND:
			sciprint("An optimal solution has been found.");
			break;
		case PREP_NO_SOLUTION:
			sciprint("This problem is infeasible.");
			break;
		case PREP_ERROR:
			sciprint("An error occured during preprocessing.");
			status=1;
			break;
		case ERROR__USER:
			sciprint("Error: user error detected in one of "
			"sci_user_send_lp_data(), "
			"sci_user_send_cg_data(), "
			"user_send_cp_data(), "
			"user_receive_feasible_solution(), "
			"user_display_solution(), "
			"user_process_own_messages() functions.");
			status=1;
			break;
		case TM_OPTIMAL_SOLUTION_FOUND:
			sciprint("An optimal solution has been found.");
			break;
		case TM_TIME_LIMIT_EXCEEDED:
			sciprint("The solver stopped after the time limit was reached.");
			break;
		case TM_NODE_LIMIT_EXCEEDED:
			sciprint("The solver stopped after the node limit was reached.");
			break;
		case TM_TARGET_GAP_ACHIEVED:
			sciprint("The solver stopped after achieving the target gap.");
			break;
		case TM_FOUND_FIRST_FEASIBLE:
			sciprint("A feasible solution has been found. It may not be optimal.");
			break;
		case TM_ERROR__NO_BRANCHING_CANDIDATE:
			sciprint("Error: user didn’t select branching candidate in user_select_candidates()");
			status=1;
			break;
		case TM_ERROR__ILLEGAL_RETURN_CODE:
			sciprint("Error: illegal return code.");
			status=1;
			break;
		case TM_ERROR__NUMERICAL_INSTABILITY:
			sciprint("Error: solver stopped due to some numerical difficulties.");
			status=1;
			break;
		case TM_ERROR__COMM_ERROR:
			sciprint("Error: solver stopped due to communication error.");
			status=1;
			break;
		case TM_ERROR__USER:
			sciprint("Error: user error detected in one of user callbacks called during TM processes.");
			break;
		default:
			sciprint("Error: undefined return value.");
			status=1;
			break;
	}
	sciprint("\n");
	return status;
}

}
