/*
 * Quadratic Programming Toolbox for Scilab using IPOPT library
 * Authors :
	Sai Kiran
	Keyur Joshi
	Iswarya
 */

#include "QuadNLP.hpp"
#include "IpIpoptData.hpp"

extern "C"{
#include <api_scilab.h>
#include <Scierror.h>
#include <BOOL.h>
#include <localization.h>
#include <sciprint.h>


double x_static,i, *op_obj_x = NULL,*op_obj_value = NULL;

using namespace Ipopt;

QuadNLP::~QuadNLP()
		 {
			free(finalX_);
			free(finalZl_);
			free(finalZu_);}

//get NLP info such as number of variables,constraints,no.of elements in jacobian and hessian to allocate memory
bool QuadNLP::get_nlp_info(Index& n, Index& m, Index& nnz_jac_g, Index& nnz_h_lag, IndexStyleEnum& index_style){
	n=numVars_; // Number of variables
	m=numConstr_; // Number of constraints
	nnz_jac_g = n*m; // No. of elements in Jacobian of constraints 
	nnz_h_lag = n*(n+1)/2; // No. of elements in lower traingle of Hessian of the Lagrangian.
	index_style=C_STYLE; // Index style of matrices
	return true;
	}

//get variable and constraint bound info
bool QuadNLP::get_bounds_info(Index n, Number* x_l, Number* x_u, Index m, Number* g_l, Number* g_u){
	
	unsigned int i;
	for(i=0;i<n;i++){
		x_l[i]=varLB_[i];
		x_u[i]=varUB_[i];
		}

	for(i=0;i<m;i++){
		g_l[i]=conLB_[i];
		g_u[i]=conUB_[i];
		}
	return true;
	}

//get value of objective function at vector x
bool QuadNLP::eval_f(Index n, const Number* x, bool new_x, Number& obj_value){
	unsigned int i,j;
	Number temp;
	obj_value=0;

	for (i=0;i<n;++i){
		for (j=0;j<n;++j){
			obj_value+=x[i]*x[j]*qMatrix_[n*i+j];			
			}
		obj_value+=x[i]*lMatrix_[i];
		}
	return true;
	}

//get value of gradient of objective function at vector x.
bool QuadNLP::eval_grad_f(Index n, const Number* x, bool new_x, Number* grad_f){
	unsigned int i,j;
	for(i=0;i<n;i++){
		grad_f[i]=lMatrix_[i];
		for(j=0;j<n;j++)
			grad_f[i]+=(qMatrix_[n*i+j]+qMatrix_[n*j+i])*x[j];
		}
	return true;
	}

//Get the values of constraints at vector x.
bool QuadNLP::eval_g(Index n, const Number* x, bool new_x, Index m, Number* g){
	unsigned int i,j;
	for(i=0;i<m;i++){
		g[i]=0;
		for(j=0;j<n;j++)
			g[i]+=x[j]*conMatrix_[n*i+j];
		}
	return true;
	}

// This method sets initial values for required vectors . For now we are assuming 0 to all values. 
bool QuadNLP::get_starting_point(Index n, bool init_x, Number* x,
				 bool init_z, Number* z_L, Number* z_U,
				 Index m, bool init_lambda,
				 Number* lambda){
	if (init_x == true){ //we need to set initial values for vector x
		for (Index var=0;var<n;++var)
			x[var]=0.0;//initialize with 0 or we can change.
		}

	if (init_z == true){ //we need to provide initial values for vector bound multipliers
		for (Index var=0;var<n;++var){
			z_L[var]=0.0; //initialize with 0 or we can change.
			z_U[var]=0.0;//initialize with 0 or we can change.
			}
		}
	
	if (init_lambda == true){ //we need to provide initial values for lambda values.
		for (Index var=0;var<m;++var){
			lambda[var]=0.0; //initialize with 0 or we can change.
			}
		}

	return true;
	}
/* Return either the sparsity structure of the Jacobian of the constraints, or the values for the Jacobian of the constraints at the point x.

*/ 
bool QuadNLP::eval_jac_g(Index n, const Number* x, bool new_x,
			 Index m, Index nele_jac, Index* iRow, Index *jCol,
			 Number* values){
	
	//It asked for structure of jacobian.
	if (values==NULL){ //Structure of jacobian (full structure)
		int index=0;
		for (int var=0;var<m;++var)//no. of constraints
			for (int flag=0;flag<n;++flag){//no. of variables
				iRow[index]=var;
				jCol[index]=flag;
				index++;
				}
		}
	//It asked for values
	else { 
		int index=0;
		for (int var=0;var<m;++var)
			for (int flag=0;flag<n;++flag)
				values[index++]=conMatrix_[n*var+flag];
		}
	return true;
	}

/*
 * Return either the sparsity structure of the Hessian of the Lagrangian, 
 * or the values of the Hessian of the Lagrangian  for the given values for
 * x,lambda,obj_factor.
*/
bool QuadNLP::eval_h(Index n, const Number* x, bool new_x,
		     Number obj_factor, Index m, const Number* lambda,
		     bool new_lambda, Index nele_hess, Index* iRow,
		     Index* jCol, Number* values){

	if (values==NULL){
		Index idx=0;
		for (Index row = 0; row < n; row++) {
			for (Index col = 0; col <= row; col++) {
				iRow[idx] = row;
				jCol[idx] = col;
				idx++;
		  		}
			}
		}
	else {
		Index index=0;
		for (Index row=0;row < n;++row){
			for (Index col=0; col <= row; ++col){
				values[index++]=obj_factor*(qMatrix_[n*row+col]+qMatrix_[n*col+row]);
				}
			}
		}
	return true;
	}


void QuadNLP::finalize_solution(SolverReturn status,
				Index n, const Number* x, const Number* z_L, const Number* z_U,
				Index m, const Number* g, const Number* lambda, Number obj_value,
				const IpoptData* ip_data,
				IpoptCalculatedQuantities* ip_cq){
	
	finalX_ = (double*)malloc(sizeof(double) * numVars_ * 1);
	for (Index i=0; i<n; i++) 
	{
    		 finalX_[i] = x[i];
	}
	
	finalZl_ = (double*)malloc(sizeof(double) * numVars_ * 1);
	for (Index i=0; i<n; i++) 
	{
    		 finalZl_[i] = z_L[i];
	}

	finalZu_ = (double*)malloc(sizeof(double) * numVars_ * 1);
	for (Index i=0; i<n; i++) 
	{
    		 finalZu_[i] = z_U[i];
	}

	finalLambda_ = (double*)malloc(sizeof(double) * numConstr_ * 1);
	for (Index i=0; i<m; i++) 
	{
    		 finalLambda_[i] = lambda[i];
	}

	iter_ = ip_data->iter_count();
	finalObjVal_ = obj_value;
	status_ = status;
	
   }

	const double * QuadNLP::getX()
	{	
		return finalX_;
	}

	const double * QuadNLP::getZl()
	{	
		return finalZl_;
	}

	const double * QuadNLP::getZu()
	{	
		return finalZu_;
	}

	const double * QuadNLP::getLambda()
	{	
		return finalLambda_;
	}

	double QuadNLP::getObjVal()
	{	
		return finalObjVal_;
	}

	double QuadNLP::iterCount()
	{	
		return (double)iter_;
	}

	int QuadNLP::returnStatus()
	{	
		return status_;
	}

}
