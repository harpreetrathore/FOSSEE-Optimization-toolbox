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

function [xopt,fopt,iter] = symphony_mat (varargin)
    

  // Solves a mixed integer linear programming constrained optimization problem.
  //
  //   Calling Sequence
  //   xopt = symphony_mat(f,intcon,A,b)
  //   xopt = symphony_mat(f,intcon,A,b,Aeq,beq)
  //   xopt = symphony_mat(f,intcon,A,b,Aeq,beq,lb,ub)
  //   xopt = symphony_mat(f,intcon,A,b,Aeq,beq,lb,ub,options)
  //   [xopt,fopt,iter] = symphony_mat( ... )
  //   
  //   Parameters
  //   f = a nx1 matrix of doubles, 
  //   intcon = 
  //   A = 
  //   b = 
  //   Aeq = 
  //   beq = 
  //   lb = 
  //   ub = 
  //   options = 
  //   
  //   xopt = a 1xn matrix of doubles, the computed solution of the optimization problem
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
    
  
//To check the number of input and output argument
   [lhs , rhs] = argn();
	
//To check the number of argument given by user
   if ( rhs < 4 | rhs = 5 | rhs = 7 | rhs > 9 ) then
    errmsg = msprintf(gettext("%s: Unexpected number of input arguments : %d provided while should be in the set [4 6 8 9]"), "Symphony", rhs);
    error(errmsg)
   end
   
   
   objCoef = varargin(1)
   intcon = varargin(2)
   A = varargin(3)
   b = varargin(4)
   
   nbVar = size(f,2);
   nbCon = size(A,1);
   
   if ( rhs<4 ) then
      Aeq = []
      beq = []
   else
      Aeq = varargin(5);
      beq = varargin(6);
      
      //Check the size of equality constraint which should equal to the number of inequality constraints
        if ( size(Aeq,2) ~= nbVar) then
            errmsg = msprintf(gettext("%s: The size of equality constraint is not equal to the number of variables"), "Symphony");
            error(errmsg);
        end
      
      //Check the size of upper bound of inequality constraint which should equal to the number of constraints
        if ( size(beq,2) ~= size(Aeq,1)) then
            errmsg = msprintf(gettext("%s: The equality constraint upper bound is not equal to the number of equality constraint"), "Symphony");
            error(errmsg);
        end
        
   end
   
   if ( rhs<6 ) then
      lb = repmat(-%inf,1,nbVar);
      ub = repmat(%inf,1,nbVar);
   else
      lb = varargin(7);
      ub = varargin(8);
   end
   
   if (rhs<9) then
      options = [];
   else
      options = varargin(9);
   end
   

//Check the size of lower bound of inequality constraint which should equal to the number of constraints
   if ( size(b,2) ~= size(A,1)) then
      errmsg = msprintf(gettext("%s: The Lower Bound of inequality constraint is not equal to the number of constraint"), "Symphony");
      error(errmsg);
   end

//Check the size of Lower Bound which should equal to the number of variables
   if ( size(lb,2) ~= nbVar) then
      errmsg = msprintf(gettext("%s: The Lower Bound is not equal to the number of variables"), "Symphony");
      error(errmsg);
   end

//Check the size of Upper Bound which should equal to the number of variables
   if ( size(ub,2) ~= nbVar) then
      errmsg = msprintf(gettext("%s: The Upper Bound is not equal to the number of variables"), "Symphony");
      error(errmsg);
   end
   
    //Changing the inputs in symphony's format 
    conMatrix = [A;Aeq]
    nbCon = size(conMatrix,1);
    conLB = [repmat(-%inf,1,size(A,1)), beq]';
    conUB = [b,beq]' ; 
    
    objSense = 1;
    
   [xopt,fopt,iter] = symphony_call(nbVar,nbCon,objCoef,isInt,lb,ub,conMatrix,conLB,conUB,objSense,options);

endfunction
