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
  //   [xopt,fopt,exitflag,output,lamda] = qpipopt( ... )
  //   
  //   Parameters
  //   nbVar : a 1 x 1 matrix of doubles, number of variables
  //   nbCon : a 1 x 1 matrix of doubles, number of constraints
  //   Q : a n x n matrix of doubles, where n is number of variables, represents coefficients of quadratic in the quadratic problem.
  //   p : a n x 1 matrix of doubles, where n is number of variables, represents coefficients of linear in the quadratic problem
  //   LB : a n x 1 matrix of doubles, where n is number of variables, contains lower bounds of the variables.
  //   UB : a n x 1 matrix of doubles, where n is number of variables, contains upper bounds of the variables.
  //   conMatrix : a m x n matrix of doubles, where n is number of variables and m is number of constraints, contains  matrix representing the constraint matrix 
  //   conLB : a m x 1 matrix of doubles, where m is number of constraints, contains lower bounds of the constraints. 
  //   conUB : a m x 1 matrix of doubles, where m is number of constraints, contains upper bounds of the constraints.
  //   xopt : a 1xn matrix of doubles, the computed solution of the optimization problem.
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
  //      
  //      conMatrix= [1,-1,1,0,3,1;
  //                 -1,0,-3,-4,5,6;
  //                  2,5,3,0,1,0
  //                  0,1,0,1,2,-1;
  //                 -1,0,2,1,1,0];
  //      conLB=[1 2 3 -%inf -%inf]';
  //      conUB = [1 2 3 -1 2.5]';
  //      //with  x between ci and cs:
  //      lb=[-1000 -10000 0 -1000 -1000 -1000];
  //      ub=[10000 100 1.5 100 100 1000];
  //      //and minimize 0.5*x'*Q*x + p'*x with
  //      p=[1 2 3 4 5 6]; Q=eye(6,6);
  //      nbVar = 6;
  //      nbCon = 5;
  //      [xopt,fopt,exitflag,output,lambda]=qpipopt(nbVar,nbCon,Q,p,lb,ub,conMatrix,conLB,conUB)
  //    
  // Examples 
  //      //min. -8*x1 -16*x2 + x1^2 + 4* x2^2
  //      //  such that
  //      //     x1 + x2 <= 5,
  //      //     x1 <= 3,
  //      //     x1 >= 0,
  //      //     x2 >= 0
  //      conMatrix= [1 1];
  //      conLB=[-%inf];
  //      conUB = [5];
  //      //with  x between ci and cs:
  //      lb=[0,0];
  //      ub=[3,%inf];
  //      //and minimize 0.5*x'*Q*x + p'*x with
  //      p=[-8,-16]; 
  //      Q=[1,0;0,4];
  //      nbVar = 2;
  //      nbCon = 1;
  //      [xopt,fopt,exitflag,output,lambda] = qpipopt(nbVar,nbCon,Q,p,lb,ub,conMatrix,conLB,conUB)
  // 
  // Authors
  // Keyur Joshi, Saikiran, Iswarya, Harpreet Singh
    
    
//To check the number of input and output argument
   [lhs , rhs] = argn();
	
//To check the number of argument given by user
   if ( rhs ~= 9 ) then
    errmsg = msprintf(gettext("%s: Unexpected number of input arguments : %d provided while should be 9"), "qpipopt", rhs);
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
   conLB = conLB';  //IPOpt wants it in row matrix form 
   conUB = varargin(9);
   conUB = conUB';	//IPOpt wants it in row matrix form
   
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
   
   
//Check the size of constraint which should equal to the number of constraints
   if ( size(conMatrix,1) ~= nbCon) then
      errmsg = msprintf(gettext("%s: The Lower Bound is not equal to the number of variables"), "qpipopt");
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
      errmsg = msprintf(gettext("%s: The Upper Bound of constraints is not equal to the number of constraints"), "qp_ipopt");
      error(errmsg);
   end
    
   [xopt,fopt,status,iter,Zl,Zu,lmbda] = solveqp(nbVar,nbCon,Q,p,conMatrix,conLB,conUB,LB,UB);
   
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
