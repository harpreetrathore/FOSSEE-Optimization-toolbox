/*
 * Quadratic Programming Toolbox for Scilab using IPOPT library
 * Authors :
	Sai Kiran
	Keyur Joshi
	Iswarya
	Harpreet Singh
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
	
	CheckInputArgument(pvApiCtx, 11, 11); // We need total 10 input arguments.
	CheckOutputArgument(pvApiCtx, 7, 7);
	
	// Error management variable
	SciErr sciErr;
	int retVal=0, *piAddressVarQ = NULL,*piAddressVarP = NULL,*piAddressVarCM = NULL,*piAddressVarCUB = NULL,*piAddressVarCLB = NULL, 		*piAddressVarLB = NULL,*piAddressVarUB = NULL,*piAddressVarG = NULL,*piAddressVarParam = NULL;
	double *QItems=NULL,*PItems=NULL,*ConItems=NULL,*conUB=NULL,*conLB=NULL,*varUB=NULL,*varLB=NULL,*init_guess = NULL;
	double *cpu_time=NULL, *max_iter=NULL, x,f,iter;
	static unsigned int nVars = 0,nCons = 0;
	unsigned int temp1 = 0,temp2 = 0;


	////////// Manage the input argument //////////
	
	
	//Number of Variables
	getIntFromScilab(1,&nVars);

	//Number of Constraints
	getIntFromScilab(2,&nCons);

	temp1 = nVars;
	temp2 = nCons;

	//Q matrix from scilab
	/* get Address of inputs */
	sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarQ);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	/* Check that the first input argument is a real matrix (and not complex) */
	if ( !isDoubleType(pvApiCtx, piAddressVarQ) ||  isVarComplex(pvApiCtx, piAddressVarQ) )
	{
		Scierror(999, "%s: Wrong type for input argument #%d: A real matrix expected.\n", fname, 3);
		return 0;
	}

	/* get matrix */
	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarQ, &temp1, &temp1, &QItems);
	if (sciErr.iErr)
	{
	printError(&sciErr, 0);
	return 0;
	}
	
	//P matrix from scilab
	/* get Address of inputs */
	sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddressVarP);
	if (sciErr.iErr)
	{
	printError(&sciErr, 0);
	return 0;
	}

	/* Check that the first input argument is a real matrix (and not complex) */
	if ( !isDoubleType(pvApiCtx, piAddressVarP) ||  isVarComplex(pvApiCtx, piAddressVarP) )
	{
		Scierror(999, "%s: Wrong type for input argument #%d: A real matrix expected.\n", fname, 4);
		return 0;
	}
	
	temp1 = 1;
	temp2 = nVars; 
	/* get matrix */
	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarP, &temp1,&temp2, &PItems);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if (nCons!=0)
	{
		//conMatrix matrix from scilab
		/* get Address of inputs */
		sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddressVarCM);
		if (sciErr.iErr)
		{
		printError(&sciErr, 0);
		return 0;
		}

		/* Check that the first input argument is a real matrix (and not complex) */
		if ( !isDoubleType(pvApiCtx, piAddressVarCM) ||  isVarComplex(pvApiCtx, piAddressVarCM) )
		{
		Scierror(999, "%s: Wrong type for input argument #%d: A real matrix expected.\n", fname, 5);
		return 0;
		}
		temp1 = nCons;
		temp2 = nVars;

		/* get matrix */
		sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarCM,&temp1, &temp2, &ConItems);
		if (sciErr.iErr)
		{
		printError(&sciErr, 0);
		return 0;
		}


		//conLB matrix from scilab
		/* get Address of inputs */
		sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddressVarCLB);
		if (sciErr.iErr)
		{
		printError(&sciErr, 0);
		return 0;
		}

		/* Check that the first input argument is a real matrix (and not complex) */
		if ( !isDoubleType(pvApiCtx, piAddressVarCLB) ||  isVarComplex(pvApiCtx, piAddressVarCLB) )
		{
		Scierror(999, "%s: Wrong type for input argument #%d: A real matrix expected.\n", fname, 6);
		return 0;
		}
		temp1 = nCons;
		temp2 = 1;

		/* get matrix */
		sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarCLB,&temp1, &temp2, &conLB);
		if (sciErr.iErr)
		{
		printError(&sciErr, 0);
		return 0;
		}
		
		//conUB matrix from scilab
		/* get Address of inputs */
		sciErr = getVarAddressFromPosition(pvApiCtx, 7, &piAddressVarCUB);
		if (sciErr.iErr)
		{
		printError(&sciErr, 0);
		return 0;
		}

		/* Check that the first input argument is a real matrix (and not complex) */
		if ( !isDoubleType(pvApiCtx, piAddressVarCUB) ||  isVarComplex(pvApiCtx, piAddressVarCUB) )
		{
		Scierror(999, "%s: Wrong type for input argument #%d: A real matrix expected.\n", fname, 7);
		return 0;
		}

		temp1 = nCons;
		temp2 = 1;

		/* get matrix */
		sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarCUB,&temp1, &temp2, &conUB);
		if (sciErr.iErr)
		{
		printError(&sciErr, 0);
		return 0;
		}
		
	}

	//varLB matrix from scilab
	/* get Address of inputs */
	sciErr = getVarAddressFromPosition(pvApiCtx, 8, &piAddressVarLB);
	if (sciErr.iErr)
	{
	printError(&sciErr, 0);
	return 0;
	}

	/* Check that the first input argument is a real matrix (and not complex) */
	if ( !isDoubleType(pvApiCtx, piAddressVarLB) ||  isVarComplex(pvApiCtx, piAddressVarLB) )
	{
		Scierror(999, "%s: Wrong type for input argument #%d: A real matrix expected.\n", fname, 8);
		return 0;
	}
	temp1 = 1;
	temp2 = nVars;

	/* get matrix */
	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarLB, &temp1,&temp2, &varLB);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	//varUB matrix from scilab
	/* get Address of inputs */
	sciErr = getVarAddressFromPosition(pvApiCtx, 9, &piAddressVarUB);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}
	/* Check that the first input argument is a real matrix (and not complex) */
	if ( !isDoubleType(pvApiCtx, piAddressVarUB) ||  isVarComplex(pvApiCtx, piAddressVarUB) )
	{
		Scierror(999, "%s: Wrong type for input argument #%d: A real matrix expected.\n", fname, 9);
		return 0;
	}

	temp1 = 1;
	temp2 = nVars;

	/* get matrix */
	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarUB, &temp1,&temp2, &varUB);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}
	
	/* get matrix */
	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarLB, &temp1,&temp2, &varLB);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	//Initial Value of variables from scilab
	/* get Address of inputs */
	sciErr = getVarAddressFromPosition(pvApiCtx, 10, &piAddressVarG);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}
	/* Check that the first input argument is a real matrix (and not complex) */
	if ( !isDoubleType(pvApiCtx, piAddressVarG) ||  isVarComplex(pvApiCtx, piAddressVarG) )
	{
		Scierror(999, "%s: Wrong type for input argument #%d: A real matrix expected.\n", fname, 10);
		return 0;
	}

	temp1 = 1;
	temp2 = nVars;

	/* get matrix */
	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarG, &temp1,&temp2, &init_guess);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}
	
	//Setting the parameters
	/* get Address of inputs */
	sciErr = getVarAddressFromPosition(pvApiCtx, 11, &piAddressVarParam);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	temp1 = 1;
	temp2 = 1;

	/* get matrix */
	sciErr = getMatrixOfDoubleInList(pvApiCtx, piAddressVarParam, 2, &temp1,&temp2, &max_iter);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	/* get matrix */
	sciErr = getMatrixOfDoubleInList(pvApiCtx, piAddressVarParam, 4, &temp1,&temp2, &cpu_time);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

		using namespace Ipopt;

		SmartPtr<QuadNLP> Prob = new QuadNLP(nVars,nCons,QItems,PItems,ConItems,conUB,conLB,varUB,varLB,init_guess);
		SmartPtr<IpoptApplication> app = IpoptApplicationFactory();
  		app->RethrowNonIpoptException(true);

		// Change some options
		// Note: The following choices are only examples, they might not be
		//       suitable for your optimization problem.
		app->Options()->SetNumericValue("tol", 1e-7);
		app->Options()->SetIntegerValue("max_iter", (int)*max_iter);
		app->Options()->SetNumericValue("max_cpu_time", *cpu_time);
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
	
		
	return 0;
	}

}

/*
hessian_constan
jacobian _constant

j_s_d constant : yes
*/

