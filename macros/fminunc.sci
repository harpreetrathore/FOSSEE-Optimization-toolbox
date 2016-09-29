// Copyright (C) 2015 - IIT Bombay - FOSSEE
//
// This file must be used under the terms of the BSD.
// This source file is licensed as described in the file LICENSE, which
// you should have received as part of this distribution.  The terms
// are also available at
// https://opensource.org/licenses/BSD-3-Clause
// Author: R.Vidyadhar & Vignesh Kannan
// Organization: FOSSEE, IIT Bombay
// Email: toolbox@scilab.in

function [xopt,fopt,exitflag,output,gradient,hessian] = fminunc (varargin)
  	// Solves a multi-variable unconstrainted optimization problem
  	//
  	//   Calling Sequence
  	//   xopt = fminunc(f,x0)
  	//   xopt = fminunc(f,x0,options)
  	//   [xopt,fopt] = fminunc(.....)
  	//   [xopt,fopt,exitflag]= fminunc(.....)
  	//   [xopt,fopt,exitflag,output]= fminunc(.....)
  	//   [xopt,fopt,exitflag,output,gradient]=fminunc(.....)
  	//   [xopt,fopt,exitflag,output,gradient,hessian]=fminunc(.....)
  	//
  	//   Parameters
  	//   f : a function, representing the objective function of the problem 
  	//   x0 : a vector of doubles, containing the starting of variables.
  	//   options: a list, containing the option for user to specify. See below for details.
  	//   xopt : a vector of doubles, the computed solution of the optimization problem.
  	//   fopt : a scalar of double, the function value at x. 
  	//   exitflag : a scalar of integer, containing the flag which denotes the reason for termination of algorithm. See below for details.
  	//   output   : a structure, containing the information about the optimization. See below for details.
  	//   gradient : a vector of doubles, containing the the gradient of the solution.
  	//   hessian  : a matrix of doubles, containing the the hessian of the solution.
  	//
  	//   Description
  	//   Search the minimum of an unconstrained optimization problem specified by :
  	//   Find the minimum of f(x) such that 
  	//
  	//   <latex>
  	//    \begin{eqnarray}
  	//    &\mbox{min}_{x}
  	//    & f(x)\\
  	//    \end{eqnarray}
  	//   </latex>
  	//
  	//   The routine calls Ipopt for solving the Un-constrained Optimization problem, Ipopt is a library written in C++.
  	//
  	// The options allows the user to set various parameters of the Optimization problem. 
  	// It should be defined as type "list" and contains the following fields.
	// <itemizedlist>
	//   <listitem>Syntax : options= list("MaxIter", [---], "CpuTime", [---], "Gradient", ---, "Hessian", ---);</listitem>
	//   <listitem>MaxIter : a Scalar, containing the Maximum Number of Iteration that the solver should take.</listitem>
	//   <listitem>CpuTime : a Scalar, containing the Maximum amount of CPU Time that the solver should take.</listitem>
	//   <listitem>Gradient : a function, representing the gradient function of the Objective in Vector Form.</listitem>
	//   <listitem>Hessian : a function, representing the hessian function of the Objective in Symmetric Matrix Form.</listitem>
	//   <listitem>Default Values : options = list("MaxIter", [3000], "CpuTime", [600]);</listitem>
	// </itemizedlist>
	//
	// The exitflag allows to know the status of the optimization which is given back by Ipopt.
	// <itemizedlist>
	//   <listitem>exitflag=0 : Optimal Solution Found </listitem>
	//   <listitem>exitflag=1 : Maximum Number of Iterations Exceeded. Output may not be optimal.</listitem>
	//   <listitem>exitflag=2 : Maximum CPU Time exceeded. Output may not be optimal.</listitem>
	//   <listitem>exitflag=3 : Stop at Tiny Step.</listitem>
	//   <listitem>exitflag=4 : Solved To Acceptable Level.</listitem>
	//   <listitem>exitflag=5 : Converged to a point of local infeasibility.</listitem>
	// </itemizedlist>
	//
	// For more details on exitflag see the ipopt documentation, go to http://www.coin-or.org/Ipopt/documentation/
	//
	// The output data structure contains detailed informations about the optimization process. 
	// It has type "struct" and contains the following fields.
	// <itemizedlist>
	//   <listitem>output.Iterations: The number of iterations performed during the search</listitem>
	//   <listitem>output.Cpu_Time: The total cpu-time spend during the search</listitem>
	//   <listitem>output.Objective_Evaluation: The number of Objective Evaluations performed during the search</listitem>
	//   <listitem>output.Dual_Infeasibility: The Dual Infeasiblity of the final soution</listitem>
	//	 <listitem>output.Message: The output message for the problem</listitem>
	// </itemizedlist>
	//
  	// Examples
  	//     //Find x in R^2 such that it minimizes the Rosenbrock function 
  	//     //f = 100*(x2 - x1^2)^2 + (1-x1)^2
  	//     //Objective function to be minimised
  	//     function y= f(x)
  	//   	    y= 100*(x(2) - x(1)^2)^2 + (1-x(1))^2;
  	//     endfunction
  	//	 //Starting point  
  	//     x0=[-1,2];
  	//	 //Gradient of objective function
  	//     function y= fGrad(x)
  	//   	     y= [-400*x(1)*x(2) + 400*x(1)^3 + 2*x(1)-2, 200*(x(2)-x(1)^2)];
  	//     endfunction
  	//	 //Hessian of Objective Function
  	//     function y= fHess(x)
  	//   	     y= [1200*x(1)^2- 400*x(2) + 2, -400*x(1);-400*x(1), 200 ];
  	//     endfunction
  	//     //Options
  	//     options=list("MaxIter", [1500], "CpuTime", [500], "Gradient", fGrad, "Hessian", fHess);
	//
 	//    // Calling Sequence
  	//     [xopt,fopt,exitflag,output,gradient,hessian]=fminunc(f,x0,options)
	// // Press ENTER to continue
  	//
  	// Examples
  	//      //Find x in R^2 such that the below function is minimum
  	//      //f = x1^2 + x2^2
  	//      //Objective function to be minimised
  	//      function y= f(x)
  	//   	     y= x(1)^2 + x(2)^2;
  	//      endfunction
  	//	  //Starting point  
  	//      x0=[2,1];
	//
 	//    // Calling Sequence 
  	//      [xopt,fopt]=fminunc(f,x0)
	// // Press ENTER to continue
  	//
  	// Examples
  	//     //The below problem is an unbounded problem:
  	//     //Find x in R^2 such that the below function is minimum
  	//     //f = - x1^2 - x2^2
  	//     //Objective function to be minimised
  	//     function y= f(x)
  	//        y= -x(1)^2 - x(2)^2;
  	//     endfunction
  	//	 //Starting point  
  	//     x0=[2,1];
  	//	 //Gradient of objective function
  	//     function y= fGrad(x)
  	//   	     y= [-2*x(1),-2*x(2)];
  	//     endfunction
  	//	 //Hessian of Objective Function
  	//     function y= fHess(x)
  	//   	     y= [-2,0;0,-2];
  	//     endfunction
    //     //Options
  	//     options=list("MaxIter", [1500], "CpuTime", [500], "Gradient", fGrad, "Hessian", fHess);
  	//    // Calling Sequence  
  	//     [xopt,fopt,exitflag,output,gradient,hessian]=fminunc(f,x0,options)
  	// Authors
  	// R.Vidyadhar , Vignesh Kannan
    

	//To check the number of input and output arguments
   	[lhs , rhs] = argn();
	
	//To check the number of arguments given by the user
   	if ( rhs<2 | rhs>3 ) then
    		errmsg = msprintf(gettext("%s: Unexpected number of input arguments : %d provided while should be 2 or 5"), "fminunc", rhs);
    		error(errmsg)
   	end
 
	//Storing the 1st and 2nd Input Parameters  
   	fun = varargin(1);
   	x0 = varargin(2);
      
	//To check whether the 1st Input argument(fun) is a function or not
   	if (type(fun) ~= 13 & type(fun) ~= 11) then
   		errmsg = msprintf(gettext("%s: Expected function for Objective "), "fminunc");
   		error(errmsg);
   	end
   
	//To check whether the 2nd Input argument(x0) is a vector/scalar
   	if (type(x0) ~= 1) then
   		errmsg = msprintf(gettext("%s: Expected Vector/Scalar for Starting Point"), "fminunc");
   		error(errmsg);
  	end
   
	//To check and convert the 2nd Input argument(x0) to a row vector 
   	if((size(x0,1)~=1) & (size(x0,2)~=1)) then
   		errmsg = msprintf(gettext("%s: Expected Row Vector or Column Vector for x0 (Initial Value) "), "fminunc");
   		error(errmsg);
   	else
   		if(size(x0,2)==1) then
   			x0=x0';		//Converting x0 to a row vector, if it is a column vector
   		else 
   	 		x0=x0;		//Retaining the same, if it is already a row vector
   		end   	 	
        	s=size(x0);	
   	end
   

  	//To check the match between f (1st Parameter) and x0 (2nd Parameter)
   	if(execstr('init=fun(x0)','errcatch')==21) then
		errmsg = msprintf(gettext("%s: Objective function and x0 did not match"), "fminunc");
   		error(errmsg);
	end
	
	//Converting the User defined Objective function into Required form (Error Detectable)
   	function [y,check] = f(x)
   		if(execstr('y=fun(x)','errcatch')==32 | execstr('y=fun(x)','errcatch')==27)
			y=0;
			check=1;
		else
			y=fun(x);
			if (isreal(y)==%F) then
				y=0;
				check=1;
  			else
				check=0;
			end
		end
	endfunction
   
	//To check whether options has been entered by user   
   	if ( rhs<3  ) then
      		param = list();
       else
      		param =varargin(3); //Storing the 3rd Input Parameter in an intermediate list named 'param'
    
   	end
   
	//If options has been entered, then check its type for 'list'   
   	if (type(param) ~= 15) then
   		errmsg = msprintf(gettext("%s: 3rd Input parameter should be a list (ie. Options) "), "fminunc");
   		error(errmsg);
   	end
   
	//If options has been entered, then check whether an even number of entires has been entered   
   	if (modulo(size(param),2)) then
		errmsg = msprintf(gettext("%s: Size of parameters should be even"), "fminunc");
		error(errmsg);
   	end
   	
   	//Defining a function to calculate Gradient or Hessian if the respective user entry is OFF 
   	function [y,check]=gradhess(x,t)
		if t==1 then	//To return Gradient
			if(execstr('y=numderivative(fun,x)','errcatch')==10000)
				y=0;
				check=1;
			else
				y=numderivative(fun,x);
				if (isreal(y)==%F) then
					y=0;
					check=1;
  				else
					check=0;
				end
			end			
		else		//To return Hessian
			if(execstr('[grad,y]=numderivative(fun,x)','errcatch')==10000)
				y=0;
				check=1;
			else
				[grad,y]=numderivative(fun,x);
				if (isreal(y)==%F) then
					y=0;
					check=1;
  				else
					check=0;
				end	
			end
		end
   	endfunction

	//To set default values for options, if user doesn't enter options
   	options = list("MaxIter", [3000], "CpuTime", [600]);

	//Flags to check whether Gradient is "ON"/"OFF" and Hessian is "ON"/"OFF" 
   	flag1=0;
   	flag2=0;
   	fGrad=[];
   	fGrad1=[];
   	fHess=[];
   	fHess1=[];
 
	//To check the user entry for options and store it
   	for i = 1:(size(param))/2
       	select convstr(param(2*i-1),'l')
    		case "maxiter" then
          			if (type(param(2*i))~=1) then
          				errmsg = msprintf(gettext("%s: Value for Maximum Iteration should be a Constant"), "fminunc");
    	      			error(errmsg);
          			else
          				options(2) = param(2*i);    //Setting the maximum number of iterations as per user entry
          			end
       		case "cputime" then
          			if (type(param(2*i))~=1) then
          				errmsg = msprintf(gettext("%s: Value for Maximum Cpu-time should be a Constant"), "fminunc");
    	      			error(errmsg);
          			else
          				options(4) = param(2*i);    //Setting the maximum CPU time as per user entry
          			end
        	case "gradobj" then
					if (type(param(2*i))==10) then
        				if (convstr(param(2*i))=="off") then
        					flag1 =0;
        				else
        					errmsg = msprintf(gettext("%s: Unrecognized String [%s] entered for the option- %s."), "fminunc",param(2*i), param(2*i-1));
    	      				error(errmsg);
        				end
        			else
						flag1 = 1;
        				fGrad = param(2*i);
        			end       				      
        	case "hessian" then
        			if (type(param(2*i))==10) then
        				if (convstr(param(2*i))=="off") then
        					flag2 =0;
        				else
        					errmsg = msprintf(gettext("%s: Unrecognized String [%s] entered for the option- %s."), "fminunc",param(2*i), param(2*i-1));
    	      				error(errmsg);
        				end
        			else
        				flag2 = 1;
        				fHess = param(2*i);
        			end
        	else
    	     	 	errmsg = msprintf(gettext("%s: Unrecognized parameter name %s."), "fminunc", param(2*i-1));
    	      		error(errmsg)      	
    		end
   	end
   
  //To check for correct input of Gradient and Hessian functions from the user	     	
   if (flag1==1) then
   		if (type(fGrad) ~= 13 & type(fGrad) ~= 11) then
  			errmsg = msprintf(gettext("%s: Expected function for Gradient of Objective"), "fminunc");
   			error(errmsg);
   		end
   		
   		if(execstr('samplefGrad=fGrad(x0)','errcatch')==21)
			errmsg = msprintf(gettext("%s: Gradient function of Objective and x0 did not match"), "fminunc");
   			error(errmsg);
		end
		
		samplefGrad=fGrad(x0);
		
		if (size(samplefGrad,1)==s(2) & size(samplefGrad,2)==1) then
		elseif (size(samplefGrad,1)==1 & size(samplefGrad,2)==s(2)) then
		else
   			errmsg = msprintf(gettext("%s: Wrong Input for Objective Gradient function(3rd Parameter)---->Row Vector function of size [1 X %d] is Expected"), "fminunc",s(2));
   			error(errmsg);
   		end
   		
   		function [y,check] = fGrad1(x)
   			if(execstr('y=fGrad(x)','errcatch')==32 | execstr('y=fGrad(x)','errcatch')==27)
				y = 0;
				check=1;
			else
				y=fGrad(x);
				if (isreal(y)==%F) then
					y = 0;
					check=1;
  				else
					check=0;
				end
			end
  		endfunction
   	end
   	if (flag2==1) then
   		if (type(fHess) ~= 13 & type(fHess) ~= 11) then
  			errmsg = msprintf(gettext("%s: Expected function for Hessian of Objective"), "fminunc");
   			error(errmsg);
   		end
   		
   		if(execstr('samplefHess=fHess(x0)','errcatch')==21)
			errmsg = msprintf(gettext("%s: Hessian function of Objective and x0 did not match"), "fminunc");
   			error(errmsg);
		end
		
		samplefHess=fHess(x0);
		
   		if(size(samplefHess,1)~=s(2) | size(samplefHess,2)~=s(2)) then
   			errmsg = msprintf(gettext("%s: Wrong Input for Objective Hessian function(3rd Parameter)---->Symmetric Matrix function of size [%d X %d] is Expected "), "fminunc",s(2),s(2));
   			error(errmsg);
   		end
   		
   		function [y,check] = fHess1(x)
   			if(execstr('y=fHess(x)','errcatch')==32 | execstr('y=fHess(x)','errcatch')==27)
				y = 0;
				check=1;
			else
				y=fHess(x);
				if (isreal(y)==%F) then
					y = 0;
					check=1;
  				else
					check=0;
				end
			end
  		endfunction
   	end

        //Calling the Ipopt function for solving the above problem
	[xopt,fopt,status,iter,cpu,obj_eval,dual,gradient, hessian1] = solveminuncp(f,gradhess,flag1,fGrad1,flag2,fHess1,x0,options);
   
	//Calculating the values for output
   	xopt = xopt';
   	exitflag = status;
   	output = struct("Iterations", [],"Cpu_Time",[],"Objective_Evaluation",[],"Dual_Infeasibility",[],"Message","");
   	output.Iterations = iter;
    	output.Cpu_Time = cpu;
    	output.Objective_Evaluation = obj_eval;
    	output.Dual_Infeasibility = dual;
    
    //Converting hessian of order (1 x (numberOfVariables)^2) received from Ipopt to order (numberOfVariables x numberOfVariables)
    s=size(gradient)
    for i =1:s(2)
    	for j =1:s(2)
			hessian(i,j)= hessian1(j+((i-1)*s(2)))
		end
    end


	//In the cases of the problem not being solved, return NULL to the output matrices
	if( status~=0 & status~=1 & status~=2 & status~=3 & status~=4 & status~=7 ) then
		xopt=[]
		fopt=[]
		output = struct("Iterations", [],"Cpu_Time",[],"Message","");
		output.Iterations = iter;
    		output.Cpu_Time = cpu;
    		gradient=[]
		hessian=[]
	end
		
	
    //To print output message
    select status
    
    	case 0 then
        	printf("\nOptimal Solution Found.\n");
        	output.Message="Optimal Solution Found";
    	case 1 then
        	printf("\nMaximum Number of Iterations Exceeded. Output may not be optimal.\n");
        	output.Message="Maximum Number of Iterations Exceeded. Output may not be optimal";
    	case 2 then
       		printf("\nMaximum CPU Time exceeded. Output may not be optimal.\n");
       		output.Message="Maximum CPU Time exceeded. Output may not be optimal";
    	case 3 then
        	printf("\nStop at Tiny Step\n");
        	output.Message="Stop at Tiny Step";
    	case 4 then
        	printf("\nSolved To Acceptable Level\n");
        	output.Message="Solved To Acceptable Level";
    	case 5 then
        	printf("\nConverged to a point of local infeasibility.\n");
        	output.Message="Converged to a point of local infeasibility";
    	case 6 then
        	printf("\nStopping optimization at current point as requested by user.\n");
        	output.Message="Stopping optimization at current point as requested by user";
    	case 7 then
        	printf("\nFeasible point for square problem found.\n");
        	output.Message="Feasible point for square problem found";
    	case 8 then 
        	printf("\nIterates diverging; problem might be unbounded.\n");
        	output.Message="Iterates diverging; problem might be unbounded";
    	case 9 then
        	printf("\nRestoration Failed!\n");
        	output.Message="Restoration Failed!";
    	case 10 then
        	printf("\nError in step computation (regularization becomes too large?)!\n");
        	output.Message="Error in step computation (regularization becomes too large?)!";
    	case 11 then
        	printf("\nProblem has too few degrees of freedom.\n");
        	output.Message="Problem has too few degrees of freedom";
    	case 12 then
        	printf("\nInvalid option thrown back by Ipopt\n");
        	output.Message="Invalid option thrown back by Ipopt";
    	case 13 then
        	printf("\nNot enough memory.\n");
        	output.Message="Not enough memory";
    	case 15 then
        	printf("\nINTERNAL ERROR: Unknown SolverReturn value - Notify Ipopt Authors.\n");
        	output.Message="INTERNAL ERROR: Unknown SolverReturn value - Notify Ipopt Authors";
    	else
        	printf("\nInvalid status returned. Notify the Toolbox authors\n");
        	output.Message="Invalid status returned. Notify the Toolbox authors";
        	break;
        end
    	

endfunction
