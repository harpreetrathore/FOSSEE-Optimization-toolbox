// Copyright (C) 2015 - IIT Bombay - FOSSEE
//
// Author: Harpreet Singh
// Organization: FOSSEE, IIT Bombay
// Email: harpreet.mertia@gmail.com
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [xopt,fopt,exitflag,output,lambda] = qpipopt (varargin)
  // Solves a linear quadratic problem.
  //
  //   Calling Sequence
  //   xopt = qpipopt(nbVar,nbCon,Q,p,LB,UB,conMatrix,conLB,conUB)
  //   xopt = qpipopt(nbVar,nbCon,Q,p,LB,UB,conMatrix,conLB,conUB,x0)
  //   xopt = qpipopt(nbVar,nbCon,Q,p,LB,UB,conMatrix,conLB,conUB,x0,param)
  //   [xopt,fopt,exitflag,output,lamda] = qpipopt( ... )
  //   
  //   Parameters
  //   nbVar : a double, number of variables
  //   nbCon : a double, number of constraints
  //   Q : a symmetric matrix of doubles, represents coefficients of quadratic in the quadratic problem.
  //   p : a vector of doubles, represents coefficients of linear in the quadratic problem
  //   LB : a vector of doubles, contains lower bounds of the variables.
  //   UB : a vector of doubles, where n is number of variables, contains upper bounds of the variables.
  //   conMatrix : a matrix of doubles, contains  matrix representing the constraint matrix 
  //   conLB : a vector of doubles, contains lower bounds of the constraints. 
  //   conUB : a vector of doubles, contains upper bounds of the constraints. 
  //   x0 : a vector of doubles, contains initial guess of variables.
  //   param : a list containing the the parameters to be set.
  //   xopt : a vector of doubles, the computed solution of the optimization problem.
  //   fopt : a double, the function value at x.
  //   exitflag : Integer identifying the reason the algorithm terminated.
  //   output : Structure containing information about the optimization.
  //   lambda : Structure containing the Lagrange multipliers at the solution x (separated by constraint type).
  //   
  //   Description
  //   Search the minimum of a constrained linear quadratic optimization problem specified by :
  //   find the minimum of f(x) such that 
  //
  //   <latex>
  //    \begin{eqnarray}
  //    &\mbox{min}_{x}
  //    & 1/2*x'*Q*x + p'*x  \\
  //    & \text{subject to} & conLB \leq C(x) \leq conUB \\
  //    & & lb \leq x \leq ub \\
  //    \end{eqnarray}
  //   </latex>
  //   
  //   We are calling IPOpt for solving the quadratic problem, IPOpt is a library written in C++. The code has been written by ​Andreas Wächter and ​Carl Laird.
  //
  // Examples
  //      //Find x in R^6 such that:
  //      conMatrix= [1,-1,1,0,3,1;
  //                 -1,0,-3,-4,5,6;
  //                  2,5,3,0,1,0
  //                  0,1,0,1,2,-1;
  //                 -1,0,2,1,1,0];
  //      conLB=[1;2;3;-%inf;-%inf];
  //      conUB = [1;2;3;-1;2.5];
  //      lb=[-1000;-10000; 0; -1000; -1000; -1000];
  //      ub=[10000; 100; 1.5; 100; 100; 1000];
  //      //and minimize 0.5*x'*Q*x + p'*x with
  //      p=[1; 2; 3; 4; 5; 6]; Q=eye(6,6);
  //      nbVar = 6;
  //      nbCon = 5;
  //      x0 = repmat(0,nbVar,1);
  //	  param = list("MaxIter", 300, "CpuTime", 100);
  //      [xopt,fopt,exitflag,output,lambda]=qpipopt(nbVar,nbCon,Q,p,lb,ub,conMatrix,conLB,conUB,x0,param)
  //    
  // Examples 
  //    //Find the value of x that minimize following function
  //    // f(x) = 0.5*x1^2 + x2^2 - x1*x2 - 2*x1 - 6*x2
  //    // Subject to:
  //    // x1 + x2 ≤ 2
  //    // –x1 + 2x2 ≤ 2
  //    // 2x1 + x2 ≤ 3
  //    // 0 ≤ x1, 0 ≤ x2.
  //	Q = [1 -1; -1 2]; 
  //	p = [-2; -6];
  //    conMatrix = [1 1; -1 2; 2 1];
  //	conUB = [2; 2; 3];
  //	conLB = [-%inf; -%inf; -%inf];
  //	lb = [0; 0];
  //	ub = [%inf; %inf];
  //	nbVar = 2;
  //	nbCon = 3;
  //	[xopt,fopt,exitflag,output,lambda] = qpipopt(nbVar,nbCon,Q,p,lb,ub,conMatrix,conLB,conUB)
  // 
  // Authors
  // Keyur Joshi, Saikiran, Iswarya, Harpreet Singh
    
    
//To check the number of input and output argument
   [lhs , rhs] = argn();
	
//To check the number of argument given by user
   if ( rhs < 9 | rhs > 11 ) then
    errmsg = msprintf(gettext("%s: Unexpected number of input arguments : %d provided while should be 9, 10 or 11"), "qpipopt", rhs);
    error(errmsg)
   end
   
   
   nbVar = varargin(1);
   nbCon = varargin(2);
   Q = varargin(3);
   p = varargin(4);
   LB = varargin(5);
   UB = varargin(6);
   conMatrix = varargin(7);
   conLB = varargin(8);
   conUB = varargin(9);

    if (size(LB,2)==0) then
        LB = repmat(-%inf,nbVar,1);
    end
    
    if (size(UB,2)==0) then
        UB = repmat(%inf,nbVar,1);
    end
    
   if ( rhs<10 | size(varargin(10)) ==0 ) then
      x0 = repmat(0,nbVar,1);
   else
      x0 = varargin(10);
  end
   
   if ( rhs<11 | size(varargin(11)) ==0 ) then
      param = list(); 
   else
      param =varargin(11);
   end
   
   if (type(param) ~= 15) then
      errmsg = msprintf(gettext("%s: param should be a list "), "qpipopt");
      error(errmsg);
   end
   
   if (modulo(size(param),2)) then
	errmsg = msprintf(gettext("%s: Size of parameters should be even"), "qpipopt");
	error(errmsg);
   end


   options = list(..
      "MaxIter"     , [3000], ...
      "CpuTime"   , [600] ...
      );
      

   for i = 1:(size(param))/2
       	select param(2*i-1)
    	case "MaxIter" then
          		options(2*i) = param(2*i);
       	case "CpuTime" then
          		options(2*i) = param(2*i);
    	else
    	      errmsg = msprintf(gettext("%s: Unrecognized parameter name ''%s''."), "qpipopt", param(2*i-1));
    	      error(errmsg)
    	end
   end

// Check if the user gives row vector 
// and Changing it to a column matrix

   if (size(p,2)== [nbVar]) then
   	p=p';
   end

   if (size(LB,2)== [nbVar]) then
	LB = LB';
   end

   if (size(UB,2)== [nbVar]) then
      UB = UB';
   end

   if (size(conUB,2)== [nbCon]) then
      conUB = conUB';
   end

   if (size(conLB,2)== [nbCon]) then
      conLB = conLB';
   end

   if (size(x0,2)== [nbVar]) then
	x0=x0';
   end

   //IPOpt wants it in row matrix form
   p = p';
   LB = LB';
   UB = UB';
   conLB = conLB';
   conUB = conUB';
   x0 = x0';
   
   //Checking the Q matrix which needs to be a symmetric matrix
   if ( ~isequal(Q,Q') ) then
      errmsg = msprintf(gettext("%s: Q is not a symmetric matrix"), "qpipopt");
      error(errmsg);
   end

   //Check the size of Q which should equal to the number of variable
   if ( size(Q) ~= [nbVar nbVar]) then
      errmsg = msprintf(gettext("%s: The Size of Q is not equal to the number of variables"), "qpipopt");
      error(errmsg);
   end
   
   //Check the size of p which should equal to the number of variable
   if ( size(p,2) ~= [nbVar]) then
      errmsg = msprintf(gettext("%s: The Size of p is not equal to the number of variables"), "qpipopt");
      error(errmsg);
   end
   
   if (nbCon) then
          //Check the size of constraint which should equal to the number of variables
       if ( size(conMatrix,2) ~= nbVar) then
          errmsg = msprintf(gettext("%s: The size of constraints is not equal to the number of variables"), "qpipopt");
          error(errmsg);
       end
   end

   //Check the number of constraint
   if ( size(conMatrix,1) ~= nbCon) then
      errmsg = msprintf(gettext("%s: The number of constraints is not equal to the number of constraint given i.e. %d"), "qpipopt", nbCon);
      error(errmsg);
   end

   //Check the size of Lower Bound which should equal to the number of variables
   if ( size(LB,2) ~= nbVar) then
      errmsg = msprintf(gettext("%s: The Lower Bound is not equal to the number of variables"), "qpipopt");
      error(errmsg);
   end

   //Check the size of Upper Bound which should equal to the number of variables
   if ( size(UB,2) ~= nbVar) then
      errmsg = msprintf(gettext("%s: The Upper Bound is not equal to the number of variables"), "qpipopt");
      error(errmsg);
   end

   //Check the size of constraints of Lower Bound which should equal to the number of constraints
   if ( size(conLB,2) ~= nbCon) then
      errmsg = msprintf(gettext("%s: The Lower Bound of constraints is not equal to the number of constraints"), "qpipopt");
      error(errmsg);
   end

   //Check the size of constraints of Upper Bound which should equal to the number of constraints
   if ( size(conUB,2) ~= nbCon) then
      errmsg = msprintf(gettext("%s: The Upper Bound of constraints is not equal to the number of constraints"), "qpipopt");
      error(errmsg);
   end
    
   //Check the size of initial of variables which should equal to the number of variables
   if ( size(x0,2) ~= nbVar) then
      warnmsg = msprintf(gettext("%s: Ignoring initial guess of variables as it is not equal to the number of variables"), "qpipopt");
      warning(warnmsg);
   end

    
   
   [xopt,fopt,status,iter,Zl,Zu,lmbda] = solveqp(nbVar,nbCon,Q,p,conMatrix,conLB,conUB,LB,UB,x0,options);
   
   xopt = xopt';
   exitflag = status;
   output = struct("Iterations"      , []);
   output.Iterations = iter;
   lambda = struct("lower"           , [], ..
                   "upper"           , [], ..
                   "constraint"      , []);
   
   lambda.lower = Zl;
   lambda.upper = Zu;
   lambda.constraint = lmbda;
    

endfunction
