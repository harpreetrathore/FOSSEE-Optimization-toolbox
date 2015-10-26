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


function [xopt,fopt,exitflag,output,lambda] = qpipoptmat (varargin)
  // Solves a linear quadratic problem.
  //
  //   Calling Sequence
  //   xopt = qpipoptmat(nbVar,nbCon,Q,p,LB,UB,conMatrix,conLB,conUB)
  //   x = qpipoptmat(H,f)
  //   x = qpipoptmat(H,f,A,b)
  //   x = qpipoptmat(H,f,A,b,Aeq,beq)
  //   x = qpipoptmat(H,f,A,b,Aeq,beq,lb,ub)
  //   [xopt,fopt,exitflag,output,lamda] = qpipoptmat( ... )
  //   
  //   Parameters
  //   H : a n x n matrix of doubles, where n is number of variables, represents coefficients of quadratic in the quadratic problem.
  //   f : a n x 1 matrix of doubles, where n is number of variables, represents coefficients of linear in the quadratic problem
  //   A : a m x n matrix of doubles, represents the linear coefficients in the inequality constraints
  //   b : a column vector of doubles, represents the linear coefficients in the inequality constraints
  //   Aeq : a meq x n matrix of doubles, represents the linear coefficients in the equality constraints
  //   beq : a vector of doubles, represents the linear coefficients in the equality constraints
  //   LB : a n x 1 matrix of doubles, where n is number of variables, contains lower bounds of the variables.
  //   UB : a n x 1 matrix of doubles, where n is number of variables, contains upper bounds of the variables.
  //   xopt : a nx1 matrix of doubles, the computed solution of the optimization problem.
  //   fopt : a 1x1 matrix of doubles, the function value at x.
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
  //    & 1/2*x'*H*x + f'*x  \\
  //    & \text{subject to} & A.x \leq b \\
  //    & & Aeq.x \leq beq \\
  //    & & lb \leq x \leq ub \\
  //    \end{eqnarray}
  //   </latex>
  //   
  //   We are calling IPOpt for solving the quadratic problem, IPOpt is a library written in C++. The code has been written by ​Andreas Wächter and ​Carl Laird.
  //
  // Examples
  //      //Find x in R^6 such that:
  //      
  //      Aeq= [1,-1,1,0,3,1;
  //           -1,0,-3,-4,5,6;
  //           2,5,3,0,1,0];
  //      beq=[1; 2; 3];
  //      A= [0,1,0,1,2,-1;
  //          -1,0,2,1,1,0];
  //      b = [-1; 2.5];
  //      lb=[-1000; -10000; 0; -1000; -1000; -1000];
  //      ub=[10000; 100; 1.5; 100; 100; 1000];
  //      //and minimize 0.5*x'*Q*x + p'*x with
  //      f=[1; 2; 3; 4; 5; 6]; H=eye(6,6);
  //      [xopt,fopt,exitflag,output,lambda]=qpipoptmat(H,f,A,b,Aeq,beq,lb,ub)
  //      clear H f A b Aeq beq lb ub;
  //    
  // Examples 
  //    //Find the value of x that minimize following function
  //    // f(x) = 0.5*x1^2 + x2^2 - x1*x2 - 2*x1 - 6*x2
  //    // Subject to:
  //    // x1 + x2 ≤ 2
  //    // –x1 + 2x2 ≤ 2
  //    // 2x1 + x2 ≤ 3
  //    // 0 ≤ x1, 0 ≤ x2.
  //	H = [1 -1; -1 2]; 
  //	f = [-2; -6];
  //    A = [1 1; -1 2; 2 1];
  //	b = [2; 2; 3];
  //	lb = [0; 0];
  //	ub = [%inf; %inf];
  //	[xopt,fopt,exitflag,output,lambda] = qpipoptmat(H,f,A,b,[],[],lb,ub)
  //
  // Authors
  // Keyur Joshi, Saikiran, Iswarya, Harpreet Singh
    
    
//To check the number of input and output argument
   [lhs , rhs] = argn();
	
//To check the number of argument given by user
   if ( rhs < 2 | rhs == 3 | rhs == 5 | rhs == 7 | rhs > 8 ) then
    errmsg = msprintf(gettext("%s: Unexpected number of input arguments : %d provided while should be in the set of [2 4 6 8]"), "qpipopt", rhs);
    error(errmsg)
   end
   
   H = varargin(1);
   f = varargin(2);
   nbVar = size(H,1);

   
   if ( rhs<2 ) then
      A = []
      b = []
   else
      A = varargin(3);
      b = varargin(4);
  end
      
   if ( rhs<4 ) then
      Aeq = []
      beq = []
   else
      Aeq = varargin(5);
      beq = varargin(6);
  end
  
  if ( rhs<6 ) then
      LB = repmat(-%inf,nbVar,1);
      UB = repmat(%inf,nbVar,1);
   else
      LB = varargin(7);
      UB = varargin(8);
  end
  
   nbConInEq = size(A,1);
   nbConEq = size(Aeq,1);

   //Checking the H matrix which needs to be a symmetric matrix
   if ( H~=H') then
      errmsg = msprintf(gettext("%s: H is not a symmetric matrix"), "qpipoptmat");
      error(errmsg);
   end

   //Check the size of H which should equal to the number of variable
   if ( size(H) ~= [nbVar nbVar]) then
      errmsg = msprintf(gettext("%s: The Size of H is not equal to the number of variables"), "qpipoptmat");
      error(errmsg);
   end
   
   //Check the size of f which should equal to the number of variable
   if ( size(f,1) ~= [nbVar]) then
      errmsg = msprintf(gettext("%s: The Size of f is not equal to the number of variables"), "qpipoptmat");
      error(errmsg);
   end
   
   
   //Check the size of inequality constraint which should be equal to the number of variables
   if ( size(A,2) ~= nbVar & size(A,2) ~= 0) then
      errmsg = msprintf(gettext("%s: The size of inequality constraints is not equal to the number of variables"), "qpipoptmat");
      error(errmsg);
   end

   //Check the size of equality constraint which should be equal to the number of variables
   if ( size(Aeq,2) ~= nbVar & size(Aeq,2) ~= 0 ) then
      errmsg = msprintf(gettext("%s: The size of equality constraints is not equal to the number of variables"), "qpipoptmat");
      error(errmsg);
   end


   //Check the size of Lower Bound which should be equal to the number of variables
   if ( size(LB,1) ~= nbVar) then
      errmsg = msprintf(gettext("%s: The Lower Bound is not equal to the number of variables"), "qpipoptmat");
      error(errmsg);
   end

//Check the size of Upper Bound which should equal to the number of variables
   if ( size(UB,1) ~= nbVar) then
      errmsg = msprintf(gettext("%s: The Upper Bound is not equal to the number of variables"), "qpipoptmat");
      error(errmsg);
   end

//Check the size of constraints of Lower Bound which should equal to the number of constraints
   if ( size(b,1) ~= nbConInEq & size(b,1) ~= 0) then
      errmsg = msprintf(gettext("%s: The Lower Bound of inequality constraints is not equal to the number of constraints"), "qpipoptmat");
      error(errmsg);
   end

//Check the size of constraints of Upper Bound which should equal to the number of constraints
   if ( size(beq,1) ~= nbConEq & size(beq,1) ~= 0) then
      errmsg = msprintf(gettext("%s: The Upper Bound of equality constraints is not equal to the number of constraints"), "qpipoptmat");
      error(errmsg);
   end
   
   //Converting it into ipopt format
   f = f';
   LB = LB';
   UB = UB';
   conMatrix = [Aeq;A];
   nbCon = size(conMatrix,1);
   conLB = [beq; repmat(-%inf,nbConInEq,1)]';
   conUB = [beq;b]' ; 
   [xopt,fopt,status,iter,Zl,Zu,lmbda] = solveqp(nbVar,nbCon,H,f,conMatrix,conLB,conUB,LB,UB);
   
   xopt = xopt';
   exitflag = status;
   output = struct("Iterations"      , []);
   output.Iterations = iter;
   lambda = struct("lower"           , [], ..
                   "upper"           , [], ..
                   "ineqlin"         , [], ..
                   "eqlin"           , []);
   
   lambda.lower = Zl;
   lambda.upper = Zu;
   lambda.eqlin = lmbda(1:nbConEq);
   lambda.ineqlin = lmbda(nbConEq+1:nbCon);
    

endfunction
