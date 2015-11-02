/*
 * Implementation Symphony Tool Box for Scilab
 * sym_data_query_functions.cpp
 * contains Data Query Functions( 13 functions)
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

// Function to print termination status of a function
void show_termination_status(int status) {
	if (status == FUNCTION_TERMINATED_ABNORMALLY)
		sciprint("\nFunction invoked unsuccessfully.\n");
	//else
		//sciprint("\nFunction invoked successfully.\n");	
	}

/*
 * Generelized function for sym_getNumVars,
 * sym_getNumConstrs,sym_get_NumElements
*/
int sci_sym_get_num_int(char *fname, unsigned long fname_len){

	int result=-1;/* Result of the callar */
  
	//check whether we have no input and one output argument or not
	CheckInputArgument(pvApiCtx, 0, 0) ; //no input argument
	CheckOutputArgument(pvApiCtx, 1, 1) ; //one output argument

	/* Array of possible callers of this function */
	char* arr_caller[]={"sym_getNumConstr","sym_getNumVar","sym_getNumElements"};

	/* Array of functions to be called */
	int (*fun[])(sym_environment*,int*)= { sym_get_num_rows,
											sym_get_num_cols,
											sym_get_num_elements
											};
	
	if(global_sym_env==NULL) //There is no environment opened.
		sciprint("Error: Symphony environment is not initialized.\n");
	else {
		//There is an environment opened
		int iter=0,length=sizeof(arr_caller)/sizeof(char*),found_at= -1;
		for (;iter < length ;++iter){
			if (!strcmp(fname,arr_caller[iter]))
				found_at=iter;
			}
		if (found_at != -1) {
			int ret_val=fun[found_at](global_sym_env,&result);
			show_termination_status(ret_val);
			if (ret_val == FUNCTION_TERMINATED_ABNORMALLY)
				result=-1;
			}
		else //very rare case
			sciprint("\nError in function mapping in scilab script\n");
		}
	
	//Copy the result to scilab. Location is position next to input arguments.
	return returnDoubleToScilab(result);
	}

/* This is generelized function for 
 * sym_getVarLower,sym_getVarUpper,sym_getRhs,sym_getConstrRange,sym_getConstrLower,
 * sym_getConstrUpper and sym_getObjCoeff .
 * (Functions taking symphony env and pointer to array of doubles as arguments)
*/
int sci_sym_get_dbl_arr(char *fname, unsigned long fname_len){

	int result_len=0;/* Length of the output double array */
	double *result=NULL;/* Pointer to output double array */
  
	//check whether we have no input and one output argument or not
	CheckInputArgument(pvApiCtx, 0, 0) ; //no input argument
	CheckOutputArgument(pvApiCtx, 1, 1) ; //one output argument
	
	/* Array of possible callers of this function */
	char* arr_caller[]={"sym_getVarLower","sym_getVarUpper",
						"sym_getRhs","sym_getConstrRange",
						"sym_getConstrLower","sym_getConstrUpper",
						"sym_getObjCoeff"};

	/* Array of functions to be called */
	int (*fun[])(sym_environment*,double*)= {sym_get_col_lower,sym_get_col_upper,
											 sym_get_rhs,sym_get_row_range,
											 sym_get_row_lower,sym_get_row_upper,
											sym_get_obj_coeff };
	
	/* Array of functions the above functions depend on */
	int (*fun_depends[])(sym_environment*,int*) = {sym_get_num_cols,sym_get_num_cols,
													sym_get_num_rows,sym_get_num_rows,
													sym_get_num_rows,sym_get_num_rows,
													sym_get_num_cols };

	/* We want to ouput row-matrix if we are dealing with column data .
	 * column matrix if we are dealing with row data .
	 * 0 - output a row matrix.
	 * 1 - output a column matrix.
	 */
	int representation = 0; //output a row matrix

	/* Array of representations of output depending on the above functions.
	 * It's length is same as above arrays.
	*/
	int matrix_representation[] = { 0 ,0 , 1, 1, 1, 1, 0};
	if(global_sym_env==NULL) //There is no environment opened.
		sciprint("Error: Symphony environment is not initialized.\n");
	else {
		//There is an environment opened
		int iter=0,length=sizeof(arr_caller)/sizeof(char*),found_at= -1;
		for (;iter < length ;++iter){
			if (!strcmp(fname,arr_caller[iter]))
				found_at=iter;
			}
		if (found_at != -1){
			int status1=fun_depends[found_at](global_sym_env,&result_len);
			if ( status1 == FUNCTION_TERMINATED_NORMALLY && result_len ) {
				result=(double*)malloc( sizeof(double) * result_len );
				int ret_val=fun[found_at](global_sym_env,result);
				show_termination_status(ret_val);
				if (ret_val == FUNCTION_TERMINATED_ABNORMALLY)
					result_len=0;
				else {
					if (found_at == 6) {//if called function is sym_getObjCoeff
						int iter=0,sense=0,status2 = sym_get_obj_sense(global_sym_env,&sense);
						if (sense == -1) // Multiply with -1 while showing 
							for (;iter < result_len;++iter) result[iter] *= -1; 						
						}
					representation = matrix_representation[found_at];
					}				
				}
			else
				sciprint("\n Is a problem loaded ? \n");
			}
		else //very rare case
			sciprint("\nError in function mapping in scilab script\n");
		}

	//Copy the result to scilab. Location is position next to input arguments.
	SciErr err;
	if (representation) // output a column-matrix
		err=createMatrixOfDouble(pvApiCtx,nbInputArgument(pvApiCtx)+1,result_len,1,result);
	else // output a row-matrix
		err=createMatrixOfDouble(pvApiCtx,nbInputArgument(pvApiCtx)+1,1,result_len,result);
	free(result); //Free the allocated space
	result=NULL; //Set to NULL
	if (err.iErr){ //Process error
		AssignOutputVariable(pvApiCtx, 1) = 0;
        printError(&err, 0);
        return 1;
    	}

	//assign result position to output argument
	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
	//ReturnArguments(pvApiCtx);
	return 0;
	}

/* This function returns rows sense of the problem loaded
*/
int sci_sym_get_row_sense(char *fname, unsigned long fname_len) {
	
	//check whether we have no input and one output argument or not
	CheckInputArgument(pvApiCtx, 0, 0) ; //no input argument
	CheckOutputArgument(pvApiCtx, 1, 1) ; //one output argument
	
	// Create a dummy string
	char *dummy=(char*)malloc(sizeof(char));
	char **correct=NULL; // Correct result of the function
	dummy[0]='\0';
	char **row_senses=&dummy;
	int num_rows=0;
	if(global_sym_env==NULL) //There is no environment opened.
		sciprint("Error: Symphony environment is not initialized.\n");
	else {
		int status=sym_get_num_rows(global_sym_env,&num_rows);
		if (status != FUNCTION_TERMINATED_ABNORMALLY) { //If function terminated normally
			char senses[num_rows];
			char *ptr=senses;
			// Take rows sense from symphony
			int status1=sym_get_row_sense(global_sym_env,ptr);
			if (status1 == FUNCTION_TERMINATED_ABNORMALLY) {
				sciprint("\n Is a problem loaded ? \n");
				show_termination_status(status1);
				row_senses=&dummy;
				num_rows=0;
				}
			else { // If function terminated normally
				// Convert every character to string
				correct=(char**)malloc(sizeof(char*) * num_rows);
				int iter=0;
				for (;iter < num_rows;++iter) {
					correct[iter]=(char*)malloc(sizeof(char)*2);
					correct[iter][0]=senses[iter];
					correct[iter][1]='\0';
					}
				row_senses=correct;
				show_termination_status(status1);// Show termination status of caller					
				}
			}
		else show_termination_status(status);
		}
	// Write output to scilab memory
	SciErr err=createMatrixOfString(pvApiCtx,nbInputArgument(pvApiCtx)+1,num_rows,1,row_senses);
	free(dummy); //free dummy variable
	dummy=NULL;
	if (correct){ // If we have allocated
		int iter=0;
		for (;iter < num_rows;++iter){
			free(correct[iter]); // Free each element in it	
			}
		free(correct); // Free it
		correct=NULL;
		}	
	if (err.iErr){ //Process error
        printError(&err, 0);
		AssignOutputVariable(pvApiCtx, 1) = 0;
        return 0;
    	}

	//assign result position to output argument
	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
	//ReturnArguments(pvApiCtx);
	return 0;	
	}

/*
 * Proto-type of function that converts column-major (sparse) representation
 * to row-major (sparse) representation .
*/
void column_major_to_row_major(int,int,int,double *,int *,int *,double *,int *,int *);


/* This function is to retrieve the problem's constraint matrix (sparse) .
 * Symphony uses column-major (sparse) representation.
 * Scilab uses row-major (sparse) representation.
 * So, This function takes column-major (sparse) representation from symphony ,
 * converts that to row-major (sparse) representation and writes to scilab's memory.
 *
 **/
int sci_sym_get_matrix(char *fname, unsigned long fname_len){
	int nz_ele=0;// No.of non-zero elements of the matrix
	int rows=0; //No. of rows in constraint matrix
	int columns=0; //No. of columns in constraint matrix

	/* Variables that store column-major representation of matrix.
	 * These variables will be filled by symphony
	*/
	int *column_start=NULL;// Starting index(in elements array) of each column 
	int *row_indices=NULL;// Row indices corresponding to each non-zero element
	double *elements=NULL;// Non-zero elements of matrix
	
	/* Variables that store row-major representation of matrix.
	 * Filled by a function column_major_to_row_major.
	*/
	double *new_list=NULL; // Non-zero elements of row-major representation
	int *count_per_row=NULL; //Count of non-zero elements in earch row
	int *column_position=NULL; //Column of each non-zero element

	//check whether we have no input and one output argument or not
	CheckInputArgument(pvApiCtx, 0, 0) ; //no input argument
	CheckOutputArgument(pvApiCtx, 1, 1) ; //one output argument

	if(global_sym_env==NULL) //There is no environment opened.
		sciprint("Error: Symphony environment is not initialized.\n");
	else { //There is an environment opened
		int status1=sym_get_num_elements(global_sym_env,&nz_ele); //No. of non-zero elements
		int status2=sym_get_num_cols(global_sym_env , &columns); //Columns
		int status3=sym_get_num_rows(global_sym_env , &rows); //Rows
		int status4=FUNCTION_TERMINATED_ABNORMALLY;
		
		//Make sure functions terminated normally
		if (status1 == status2 && status1 == status3 && status1 == FUNCTION_TERMINATED_NORMALLY){ 
			//Allocate memory for column-major representation			
			column_start=(int*)malloc(sizeof(int) * (columns+1));
			row_indices=(int*)malloc(sizeof(int) * nz_ele);
			elements=(double*)malloc(sizeof(double) * nz_ele);
			
			//Take column-major representation from symphony
			status4=sym_get_matrix(global_sym_env,&nz_ele,column_start,row_indices,elements); 
			if (status1 == status4) { //Check termination status of function, if normal
				//Show status of caller as normal termination
				show_termination_status(FUNCTION_TERMINATED_NORMALLY);
				
				//Allocate memory for row-major representation 
				new_list=(double*) calloc( nz_ele , sizeof(double));
				count_per_row=(int*) calloc(  rows, sizeof(int ) );
				column_position=(int*) calloc( nz_ele, sizeof(int));
				
				//Convert column-major representation to row-major representation
				column_major_to_row_major(rows,columns,nz_ele,elements,row_indices,column_start,new_list,count_per_row,column_position);
				
				/*
				(Important)Scilab considers indices from 1 , But we have column indices starting from 0 in column_position.
				 Hence add 1 to each index
				*/				
				int iter=0;
				for (;iter < nz_ele ; ++iter) column_position[iter]++;

				}
			else { //If termination status is abnormal
				show_termination_status(FUNCTION_TERMINATED_ABNORMALLY);
				sciprint("\n Is a problem loaded ? \n");
				}				
			}
		else //If termination status of any of functions is abnormal
			show_termination_status(FUNCTION_TERMINATED_ABNORMALLY);
			
		}
	
	//Copy the result to scilab. Location is position next to input arguments.
	SciErr err=createSparseMatrix(pvApiCtx,nbInputArgument(pvApiCtx)+1,rows,columns,nz_ele,count_per_row,column_position,new_list);

	/*
	 *Free allocated memory before exit
	*/
	free(row_indices);
	free(column_start);
	free(elements);
	free(new_list);
	free(count_per_row);
	free(column_position);

	if (err.iErr){ //Process error
        printError(&err, 0);
		AssignOutputVariable(pvApiCtx, 1) = 0;
        return 1;
    	}

	//assign result position to output argument
	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
	//ReturnArguments(pvApiCtx);
	return 0;
	}

/*
 * It converts column-major representation to row-major representation
 * :: ARGUMENTS ::
 * rows - No. of rows IN
 * columns - No. of columns IN
 * nz_ele - No. of non-zero elements IN 
 * elements - Non-zero elements in column-major representation IN
 * row_indices - Row index( starts from 0 : symphony) of each non-zero element	IN
 * column_start - Starting index in elements of each column 	IN
 * new_list - Non-zero elements in row-major representation	 OUT 
 * count_per_row - Count of non-zero elements in each row	OUT
 * column_position - Column index ( starts from 0 (we'll add 1 to each index later)) of each non-zero element	OUT
*/
void column_major_to_row_major(int rows,int columns,int nz_ele,double *elements,int *row_indices,int *column_start,double *new_list,int *count_per_row,int *column_position) {

	int iter=0,iter2,iter3=0,index=0;
	for (iter=0;iter < rows;++iter) {
		for (iter2=0;iter2 < nz_ele;++iter2) {
			if (row_indices[iter2] == iter) {
				count_per_row[iter]++; //Count of non-zero elements per row.
				new_list[index]=elements[iter2];
				for (iter3=0; iter3 < columns+1 ; ++iter3) {
					if (iter2 < column_start[iter3])
						break;
					}
				column_position[index] = iter3 - 1;
				index++ ;
				}
			}
		}
	}


/*
 * This function is used to get iteration count after solving a problem
*/
int sci_sym_get_iteration_count(char *fname, unsigned long fname_len){
	
	//check whether we have no input and one output argument or not
	CheckInputArgument(pvApiCtx, 0, 0) ; //no input argument
	CheckOutputArgument(pvApiCtx, 1, 1) ; //one output argument
	
	int iteration_count=0; // return value to the caller
	if(global_sym_env==NULL) //There is no environment opened.
		sciprint("Error: Symphony environment is not initialized.\n");
	else { //There is an environment opened
		 //Call symphony function
		int status=sym_get_iteration_count(global_sym_env,&iteration_count);
		show_termination_status(status);
		if (status == FUNCTION_TERMINATED_ABNORMALLY) {
			sciprint("\nHave you solved a problem ?\n");
			iteration_count = 0;
			}
		}
	// Write the result to scilab
	return returnDoubleToScilab(iteration_count);
	}

}
