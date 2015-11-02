// Copyright (C) 2015 - IIT Bombay - FOSSEE
//
// Author: Harpreet Singh
// Organization: FOSSEE, IIT Bombay
// Email: harpreet.mertia@gmail.com
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->


//
// assert_close --
//   Returns 1 if the two real matrices computed and expected are close,
//   i.e. if the relative distance between computed and expected is lesser than epsilon.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_close ( computed, expected, epsilon )
  if expected==0.0 then
    shift = norm(computed-expected);
  else
    shift = norm(computed-expected)/norm(expected);
  end
//  if shift < epsilon then
//    flag = 1;
//  else
//    flag = 0;
//  end
//  if flag <> 1 then pause,end
    flag = assert_checktrue ( shift < epsilon );
endfunction
//
// assert_equal --
//   Returns 1 if the two real matrices computed and expected are equal.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
//function flag = assert_equal ( computed , expected )
//  if computed==expected then
//    flag = 1;
//  else
//    flag = 0;
//  end
//  if flag <> 1 then pause,end
//endfunction

//Find the value of x that minimize following function
// f(x) = 0.5*x1^2 + x2^2 - x1*x2 - 2*x1 - 6*x2
// Subject to:
// x1 + x2 ≤ 2
// –x1 + 2x2 ≤ 2
// 2x1 + x2 ≤ 3
// 0 ≤ x1, 0 ≤ x2.
Q = [1 -1; -1 2];
p = [-2; -6];
conMatrix = [1 1; -1 2; 2 1];
conUB = [2; 2; 3];
conLB = [-%inf; -%inf; -%inf];
lb = [0; 0];
ub = [%inf; %inf];
nbVar = 2;
nbCon = 3;
[xopt,fopt,exitflag,output,lambda] = qpipopt(nbVar,nbCon,Q,p,lb,ub,conMatrix,conLB,conUB)

assert_close ( x , [0.6666667 1.3333333]' , 1.e-7 );
assert_close ( f , [ - 8.2222223] , 1.e-7 );

assert_checkequal( exitflag , 0 );
