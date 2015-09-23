/*
 * Quadratic Programming Toolbox for Scilab using IPOPT library
 * Authors :
	Sai Kiran
	Keyur Joshi
	Iswarya
 */


#include "sci_iofunc.hpp"
#include "IpIpoptApplication.hpp"
#include "QuadNLP.hpp"

extern "C"{
#include <api_scilab.h>
#include <Scierror.h>
#include <BOOL.h>
#include <localization.h>
#include <sciprint.h>

int j;
double *op_x, *op_obj,*p;

bool readSparse(int arg,int *iRows,int *iCols,int *iNbItem,int** piNbItemRow, int** piColPos, double** pdblReal){
	SciErr sciErr;
	int* piAddr = NULL;
	int iType   = 0;
	int iRet    = 0;
	sciErr = getVarAddressFromPosition(pvApiCtx, arg, &piAddr);
	if(sciErr.iErr)	{
		printError(&sciErr, 0);
		return false;
		}
	sciprint("\ndone\n");
	if(isSparseType(pvApiCtx, piAddr)){
		sciprint("done\n");
		sciErr =getSparseMatrix(pvApiCtx, piAddr, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal);
		if(sciErr.iErr)	{
			printError(&sciErr, 0);
			return false;
			}
		}

	else {
		sciprint("\nSparse matrix required\n");
		return false;
		}
	return true;
	}

int sci_solveqp(char *fname)
{
	
	CheckInputArgument(pvApiCtx, 9, 9); // We need total 9 input arguments.
	CheckOutputArgument(pvApiCtx, 7, 7);

	double *QItems=NULL,*PItems=NULL,*ConItems=NULL,*conUB=NULL,*conLB=NULL,*varUB=NULL,*varLB=NULL,x,f,iter;
	unsigned int nVars,nCons;


	unsigned int arg = 1,temp1,temp2;

	if ( !getIntFromScilab(arg,&nVars) && arg++ && !getIntFromScilab(arg,&nCons) && arg++ &&
		!getDoubleMatrixFromScilab(arg,&temp1,&temp2,&QItems) && temp1 == nVars && temp2 == nVars && arg++ &&
		!getDoubleMatrixFromScilab(arg,&temp1,&temp2,&PItems) && temp2 == nVars && arg++ &&
		!getDoubleMatrixFromScilab(arg,&temp1,&temp2,&ConItems) && temp1 == nCons &&((nCons !=0 && temp2 == nVars)||(temp2==0)) && arg++ &&
		!getDoubleMatrixFromScilab(arg,&temp1,&temp2,&conLB)  && temp2 == nCons && arg++ &&
		!getDoubleMatrixFromScilab(arg,&temp1,&temp2,&conUB)  && temp2 == nCons && arg++ &&
		!getDoubleMatrixFromScilab(arg,&temp1,&temp2,&varLB) && temp2 == nVars && arg++ && 
		!getDoubleMatrixFromScilab(arg,&temp1,&temp2,&varUB) && temp2 == nVars){



		using namespace Ipopt;
		SmartPtr<QuadNLP> Prob = new QuadNLP(nVars,nCons,QItems,PItems,ConItems,conUB,conLB,varUB,varLB);
		SmartPtr<IpoptApplication> app = IpoptApplicationFactory();
  		app->RethrowNonIpoptException(true);

		// Change some options
		// Note: The following choices are only examples, they might not be
		//       suitable for your optimization problem.
		app->Options()->SetNumericValue("tol", 1e-7);
		app->Options()->SetStringValue("mu_strategy", "adaptive");

		// Indicates whether all equality constraints are linear 
		app->Options()->SetStringValue("jac_c_constant", "yes");
		// Indicates whether all inequality constraints are linear 
		app->Options()->SetStringValue("jac_d_constant", "yes");	
		// Indicates whether the problem is a quadratic problem 
		app->Options()->SetStringValue("hessian_constant", "yes");
	
		// Initialize the IpoptApplication and process the options
		ApplicationReturnStatus status;
	 	status = app->Initialize();
		if (status != Solve_Succeeded) {
		  	sciprint("\n*** Error during initialization!\n");
			return0toScilab();
	   	 return (int) status;
	 	 }
		 // Ask Ipopt to solve the problem
		
		 status = app->OptimizeTNLP(Prob);

		double *fX = Prob->getX();
		double ObjVal = Prob->getObjVal();
		double *Zl = Prob->getZl();
		double *Zu = Prob->getZu();
		double *Lambda = Prob->getLambda();
		double iteration = Prob->iterCount();
		int stats = Prob->returnStatus();
		SciErr sciErr;
		sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, nVars, fX);
		if (sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 2,1,1,&ObjVal);
		if (sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = createMatrixOfInteger32(pvApiCtx, nbInputArgument(pvApiCtx) + 3,1,1,&stats);
		if (sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 4,1,1,&iteration);
		if (sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 5, 1, nVars, Zl);
		if (sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 6, 1, nVars, Zu);
		if (sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
		
		sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 7, 1, nCons, Lambda);
		if (sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
		

		AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
		AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;		
		AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx) + 3;
		AssignOutputVariable(pvApiCtx, 4) = nbInputArgument(pvApiCtx) + 4;
		AssignOutputVariable(pvApiCtx, 5) = nbInputArgument(pvApiCtx) + 5;
		AssignOutputVariable(pvApiCtx, 6) = nbInputArgument(pvApiCtx) + 6;
		AssignOutputVariable(pvApiCtx, 7) = nbInputArgument(pvApiCtx) + 7;	

  		// As the SmartPtrs go out of scope, the reference count
  		// will be decremented and the objects will automatically
  		// be deleted.
	

		}
	else {

		sciprint("\nError:: check argument %d\n",arg);
		return0toScilab();
		return 1;
		}
		
	return 0;
	}

}

/*
hessian_constan
jacobian _constant

j_s_d constant : yes
*/

