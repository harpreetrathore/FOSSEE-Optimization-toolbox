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

function [xopt,fopt,iter] = symphony (varargin)

  // Solves a  mixed integer linear programming constrained optimization problem.
  //
  //   Calling Sequence
  //   
  //   xopt = symphony(nbVar,nbCon,objCoef,isInt,LB,UB,conMatrix,conLB,conUB)
  //   xopt = symphony(nbVar,nbCon,objCoef,isInt,LB,UB,conMatrix,conLB,conUB,objSense)
  //   xopt = symphony(nbVar,nbCon,objCoef,isInt,LB,UB,conMatrix,conLB,conUB,objSense,options)
  //   [xopt,fopt,iter] = symphony( ... )
  //   
  //   Parameters
  //   
  //   nbVar = a 1 x 1 matrix of doubles, number of variables
  //   nbCon = a 1 x 1 matrix of doubles, number of constraints
  //   objCoeff = a 1 x n matrix of doubles, where n is number of variables, contains coefficients of the variables in the objective 
  //   isInt = a 1 x n matrix of boolean, where n is number of variables, representing wether a variable is constrained to be an integer 
  //   LB = a 1 x n matrix of doubles, where n is number of variables, contains lower bounds of the variables. Bound can be negative infinity
  //   UB = a 1 x n matrix of doubles, where n is number of variables, contains upper bounds of the variables. Bound can be infinity
  //   conMatrix = a m x n matrix of doubles, where n is number of variables and m is number of constraints, contains  matrix representing the constraint matrix 
  //   conLB = a m x 1 matrix of doubles, where m is number of constraints, contains lower bounds of the constraints. 
  //   conUB = a m x 1 matrix of doubles, where m is number of constraints, contains upper bounds of the constraints
  //   objSense = The sense (maximization/minimization) of the objective. Use 1(sym_minimize ) or -1 (sym_maximize) here
  
  //   xopt = a nx1 matrix of doubles, the computed solution of the optimization problem
  //   fopt = a 1x1 matrix of doubles, the function value at x
  //   iter = a 1x1 matrix of doubles, contains the number od iterations done by symphony
  //   
  //   Description
  //   
  //   Search the minimum or maximum of a constrained mixed integer linear programming optimization problem specified by :
  //   find the minimum or maximum of f(x) such that 
  //
  //   <latex>
  //    \begin{eqnarray}
  //    \mbox{min}_{x}
  //    & & f(x) \\
  //    & \text{subject to}
  //    & & conLB \geq C(x) \leq conUB \\
  //    & & & lb \geq x \leq ub \\
  //    \end{eqnarray}
  //   </latex>
  //   
  //   
  //   
  //   
  //   


//To check the number of input and output argument
   [lhs , rhs] = argn();
	
//To check the number of argument given by user
   if ( rhs < 9 | rhs > 11 ) then
    errmsg = msprintf(gettext("%s: Unexpected number of input arguments : %d provided while should be in the set [9 10 11]"), "Symphony", rhs);
    error(errmsg)
   end
   
   nbVar = varargin(1);
   nbCon = varargin(2);
   objCoef = varargin(3);
   isInt = varargin(4);
   LB = varargin(5);
   UB = varargin(6);
   conMatrix = varargin(7);
   conLB = varargin(8);
   conUB = varargin(9);
   
   if ( rhs<10 ) then
      objSense = 1;
   else
      objSense = varargin(10);
   end
   
   if (rhs<11) then
      options = [];
   else
      options = varargin(11);
   end
   

//Check the size of constraint which should equal to the number of constraints
   if ( size(LB) ~= nbCon) then
      errmsg = msprintf(gettext("%s: The Lower Bound is not equal to the number of variables"), "Symphony");
      error(errmsg);
   end

//Check the size of Lower Bound which should equal to the number of variables
   if ( size(LB) ~= nbVar) then
      errmsg = msprintf(gettext("%s: The Lower Bound is not equal to the number of variables"), "Symphony");
      error(errmsg);
   end

//Check the size of Upper Bound which should equal to the number of variables
   if ( size(UB) ~= nbVar) then
      errmsg = msprintf(gettext("%s: The Upper Bound is not equal to the number of variables"), "Symphony");
      error(errmsg);
   end

//Check the size of Lower Bound which should equal to the number of constraints
   if ( size(conLB) ~= nbCon) then
      errmsg = msprintf(gettext("%s: The Lower Bound of constraints is not equal to the number of constraints"), "Symphony");
      error(errmsg);
   end

//Check the size of Upper Bound which should equal to the number of constraints
   if ( size(conUB) ~= nbCon) then
      errmsg = msprintf(gettext("%s: The Upper Bound of constraints is not equal to the number of constraints"), "Symphony");
      error(errmsg);
   end

   [xopt,fopt,iter] = symphony_call(nbVar,nbCon,objCoef,isInt,LB,UB,conMatrix,conLB,conUB,objSense,options);

endfunction
