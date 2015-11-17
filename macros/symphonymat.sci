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

function [xopt,fopt,status,iter] = symphonymat (varargin)
  // Solves a mixed integer linear programming constrained optimization problem in intlinprog format.
  //
  //   Calling Sequence
  //   xopt = symphonymat(f,intcon,A,b)
  //   xopt = symphonymat(f,intcon,A,b,Aeq,beq)
  //   xopt = symphonymat(f,intcon,A,b,Aeq,beq,lb,ub)
  //   xopt = symphonymat(f,intcon,A,b,Aeq,beq,lb,ub,options)
  //   [xopt,fopt,status,output] = symphonymat( ... )
  //   
  //   Parameters
  //   f : a vector of doubles, where n is number of variables, contains coefficients of the variables in the objective 
  //   intcon : Vector of integer constraints, specified as a vector of positive integers. The values in intcon indicate the components of the decision variable x that are integer-valued. intcon has values from 1 through number of variable.
  //   A : Linear inequality constraint matrix, specified as a matrix of doubles. A represents the linear coefficients in the constraints A*x ≤ b. A has size M-by-N, where M is the number of constraints and N is number of variables
  //   b : Linear inequality constraint vector, specified as a vector of doubles. b represents the constant vector in the constraints A*x ≤ b. b has length M, where A is M-by-N
  //   Aeq : Linear equality constraint matrix, specified as a matrix of doubles. Aeq represents the linear coefficients in the constraints Aeq*x = beq. Aeq has size Meq-by-N, where Meq is the number of constraints and N is number of variables
  //   beq : Linear equality constraint vector, specified as a vector of doubles. beq represents the constant vector in the constraints Aeq*x = beq. beq has length Meq, where Aeq is Meq-by-N. 
  //   lb : Lower bounds, specified as a vector or array of doubles. lb represents the lower bounds elementwise in lb ≤ x ≤ ub.
  //   ub : Upper bounds, specified as a vector or array of doubles. ub represents the upper bounds elementwise in lb ≤ x ≤ ub.
  //   options : a list containing the the parameters to be set.
  //   xopt : a 1xn matrix of doubles, the computed solution of the optimization problem
  //   fopt : a 1x1 matrix of doubles, the function value at x
  //   output : The output data structure contains detailed informations about the optimization process.
  //   
  //   Description
  //   Search the minimum or maximum of a constrained mixed integer linear programming optimization problem specified by :
  //   find the minimum or maximum of f(x) such that 
  //
  //   <latex>
  //    \begin{eqnarray}
  //    &\mbox{min}_{x}
  //    & f(x) \\
  //    & \text{subject to} & A.x \leq b \\
  //    & & Aeq.x \leq beq \\
  //    & & lb \leq x \leq ub \\
  //    \end{eqnarray}
  //   </latex>
  //   
  //   We are calling SYMPHONY written in C by gateway files for the actual computation. SYMPHONY was originally written by ​Ted Ralphs, ​Menal Guzelsoy and ​Ashutosh Mahajan.
  //
  // Examples
  //    // Objective function
  //    c = [350*5,330*3,310*4,280*6,500,450,400,100]';
  //    // Lower Bound of variable
  //    lb = repmat(0,1,8);
  //    // Upper Bound of variables
  //    ub = [repmat(1,1,4) repmat(%inf,1,4)];
  //    // Constraint Matrix
  //    Aeq = [5,3,4,6,1,1,1,1;
  //                 5*0.05,3*0.04,4*0.05,6*0.03,0.08,0.07,0.06,0.03;
  //                 5*0.03,3*0.03,4*0.04,6*0.04,0.06,0.07,0.08,0.09;]
  //    beq = [ 25, 1.25, 1.25]
  //    intcon = [1 2 3 4];
  //    // Calling Symphony
  //    [x,f,status,output] = symphonymat(c,intcon,[],[],Aeq,beq,lb,ub)
  //
  // Examples 
  //    // An advanced case where we set some options in symphony
  //    // This problem is taken from 
  //    // P.C.Chu and J.E.Beasley 
  //    // "A genetic algorithm for the multidimensional knapsack problem",
  //    // Journal of Heuristics, vol. 4, 1998, pp63-86.
  //    // The problem to be solved is:
  //    // Max  sum{j=1,...,n} p(j)x(j)
  //    // st   sum{j=1,...,n} r(i,j)x(j) <= b(i)       i=1,...,m
  //    //                     x(j)=0 or 1
  //    // The function to be maximize i.e. P(j)
  //    objCoef = -1*[   504 803 667 1103 834 585 811 856 690 832 846 813 868 793 ..
  //            825 1002 860 615 540 797 616 660 707 866 647 746 1006 608 ..
  //            877 900 573 788 484 853 942 630 591 630 640 1169 932 1034 ..
  //            957 798 669 625 467 1051 552 717 654 388 559 555 1104 783 ..
  //            959 668 507 855 986 831 821 825 868 852 832 828 799 686 ..
  //            510 671 575 740 510 675 996 636 826 1022 1140 654 909 799 ..
  //            1162 653 814 625 599 476 767 954 906 904 649 873 565 853 1008 632]';
  //    //Constraint Matrix
  //    conMatrix = [   //Constraint 1
  //                    42 41 523 215 819 551 69 193 582 375 367 478 162 898 ..
  //                    550 553 298 577 493 183 260 224 852 394 958 282 402 604 ..
  //                    164 308 218 61 273 772 191 117 276 877 415 873 902 465 ..
  //                    320 870 244 781 86 622 665 155 680 101 665 227 597 354 ..
  //                    597 79 162 998 849 136 112 751 735 884 71 449 266 420 ..
  //                    797 945 746 46 44 545 882 72 383 714 987 183 731 301 ..
  //                    718 91 109 567 708 507 983 808 766 615 554 282 995 946 651 298;
  //                    //Constraint 2
  //                    509 883 229 569 706 639 114 727 491 481 681 948 687 941 ..
  //                    350 253 573 40 124 384 660 951 739 329 146 593 658 816 ..
  //                    638 717 779 289 430 851 937 289 159 260 930 248 656 833 ..
  //                    892 60 278 741 297 967 86 249 354 614 836 290 893 857 ..
  //                    158 869 206 504 799 758 431 580 780 788 583 641 32 653 ..
  //                    252 709 129 368 440 314 287 854 460 594 512 239 719 751 ..
  //                    708 670 269 832 137 356 960 651 398 893 407 477 552 805 881 850;
  //                    //Constraint 3
  //                    806 361 199 781 596 669 957 358 259 888 319 751 275 177 ..
  //                    883 749 229 265 282 694 819 77 190 551 140 442 867 283 ..
  //                    137 359 445 58 440 192 485 744 844 969 50 833 57 877 ..
  //                    482 732 968 113 486 710 439 747 174 260 877 474 841 422 ..
  //                    280 684 330 910 791 322 404 403 519 148 948 414 894 147 ..
  //                    73 297 97 651 380 67 582 973 143 732 624 518 847 113 ..
  //                    382 97 905 398 859 4 142 110 11 213 398 173 106 331 254 447 ;
  //                    //Constraint 4
  //                    404 197 817 1000 44 307 39 659 46 334 448 599 931 776 ..
  //                    263 980 807 378 278 841 700 210 542 636 388 129 203 110 ..
  //                    817 502 657 804 662 989 585 645 113 436 610 948 919 115 ..
  //                    967 13 445 449 740 592 327 167 368 335 179 909 825 614 ..
  //                    987 350 179 415 821 525 774 283 427 275 659 392 73 896 ..
  //                    68 982 697 421 246 672 649 731 191 514 983 886 95 846 ..
  //                    689 206 417 14 735 267 822 977 302 687 118 990 323 993 525 322;
  //                    //Constrain 5
  //                    475 36 287 577 45 700 803 654 196 844 657 387 518 143 ..
  //                    515 335 942 701 332 803 265 922 908 139 995 845 487 100 ..
  //                    447 653 649 738 424 475 425 926 795 47 136 801 904 740 ..
  //                    768 460 76 660 500 915 897 25 716 557 72 696 653 933 ..
  //                    420 582 810 861 758 647 237 631 271 91 75 756 409 440 ..
  //                    483 336 765 637 981 980 202 35 594 689 602 76 767 693 ..
  //                    893 160 785 311 417 748 375 362 617 553 474 915 457 261 350 635 ;
  //     ];
  //    nbVar = size(objCoef,2)
  //    conUB=[11927 13727 11551 13056 13460 ];
  //    // Lower Bound of variables
  //    lb = repmat(0,1,nbVar)
  //    // Upper Bound of variables
  //    ub = repmat(1,1,nbVar)
  //    // Lower Bound of constrains
  //    intcon = [];
  //    for i = 1:nbVar
  //        intcon = [intcon i];
  //    end
  //    options = list("time_limit", 25);
  //    // The expected solution :
  //    // Output variables
  //    xopt = [0 1 1 0 0 1 0 1 0 1 0 0 0 0 0 0 0 1 0 0 0 0 1 0 1 1 0 1 1 0 1 ..
  //            0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 1 1 ..
  //            0 0 1 0 0 1 0 1 0 0 1 0 0 1 0 1 0 0 0 0 0 1 1 0 0 0 0 0 1 1 0 0 1 0 0 1 0]
  //    // Optimal value
  //    fopt = [ 24381 ]
  //    // Calling Symphony
  //    [x,f,status,output] = symphonymat(objCoef,intcon,conMatrix,conUB,[],[],lb,ub,options);
  // 
  // Authors
  // Keyur Joshi, Saikiran, Iswarya, Harpreet Singh
    
    
//To check the number of input and output argument
   [lhs , rhs] = argn();
	
//To check the number of argument given by user
   if ( rhs < 4 | rhs == 5 | rhs == 7 | rhs > 9 ) then
    errmsg = msprintf(gettext("%s: Unexpected number of input arguments : %d provided while should be in the set [4 6 8 9]"), "Symphony", rhs);
    error(errmsg)
   end
   
   
   objCoef = varargin(1)
   intcon = varargin(2)
   A = varargin(3)
   b = varargin(4)

   if (size(objCoef,2)~=1) then
	errmsg = msprintf(gettext("%s: Objective Coefficients should be a column matrix"), "Symphonymat");
	error(errmsg);
   end


   nbVar = size(objCoef,1);

   if ( rhs<5 ) then
      Aeq = []
      beq = []
   else
      Aeq = varargin(5);
      beq = varargin(6);
   end
   
   if ( rhs<7 ) then
      lb = repmat(-%inf,1,nbVar);
      ub = repmat(%inf,1,nbVar);
   else
      lb = varargin(7);
      ub = varargin(8);
   end
   
   if (rhs<9|size(varargin(9))==0) then
      options = list();
   else
      options = varargin(9);
   end

   nbConInEq = size(A,1);
   nbConEq = size(Aeq,1);

// Check if the user gives row vector 
// and Changing it to a column matrix

   if (size(lb,2)== [nbVar]) then
	lb = lb';
   end

   if (size(ub,2)== [nbVar]) then
	ub = ub';
   end

   if (size(b,2)== [nbConInEq]) then
	b = b';
   end

   if (size(beq,2)== [nbConEq]) then
	beq = beq';
   end

    for i=1:size(intcon,2)
	if(intcon(i)>nbVar) then
		errmsg = msprintf(gettext("%s: The values inside intcon should not exceed total number of variable "), "Symphonymat");
		error(errmsg);
	end

	if (intcon(i)<1) then
		errmsg = msprintf(gettext("%s: The values inside intcon should be greater than 0 "), "Symphonymat");
		error(errmsg);
	end

	if(modulo(intcon(i),1)) then
		errmsg = msprintf(gettext("%s: The values inside intcon should be integer "), "Symphonymat");
		error(errmsg);
	end
    end

   //Check the size of inequality constraint which should equal to the number of inequality constraints
   if ( size(A,2) ~= nbVar & size(A,2) ~= 0) then
	errmsg = msprintf(gettext("%s: The size of inequality constraint is not equal to the number of variables"), "Symphonymat");
	error(errmsg);
   end


   //Check the size of lower bound of inequality constraint which should equal to the number of constraints
   if ( size(b,1) ~= size(A,1)) then
      errmsg = msprintf(gettext("%s: The Lower Bound of inequality constraint is not equal to the number of constraint"), "Symphonymat");
      error(errmsg);
   end

   //Check the size of equality constraint which should equal to the number of inequality constraints
   if ( size(Aeq,2) ~= nbVar & size(Aeq,2) ~= 0) then
	errmsg = msprintf(gettext("%s: The size of equality constraint is not equal to the number of variables"), "Symphonymat");
	error(errmsg);
   end

   //Check the size of upper bound of equality constraint which should equal to the number of constraints
   if ( size(beq,1) ~= size(Aeq,1)) then
	errmsg = msprintf(gettext("%s: The equality constraint upper bound is not equal to the number of equality constraint"), "Symphonymat");
	error(errmsg);
   end

   //Check the size of Lower Bound which should equal to the number of variables
   if ( size(lb,1) ~= nbVar) then
      errmsg = msprintf(gettext("%s: The Lower Bound is not equal to the number of variables"), "Symphonymat");
      error(errmsg);
   end

   //Check the size of Upper Bound which should equal to the number of variables
   if ( size(ub,1) ~= nbVar) then
      errmsg = msprintf(gettext("%s: The Upper Bound is not equal to the number of variables"), "Symphonymat");
      error(errmsg);
   end
   
   if (type(options) ~= 15) then
      errmsg = msprintf(gettext("%s: Options should be a list "), "Symphonymat");
      error(errmsg);
   end
   

   if (modulo(size(options),2)) then
	errmsg = msprintf(gettext("%s: Size of parameters should be even"), "Symphonymat");
	error(errmsg);
   end


    //Changing the inputs in symphony's format 
    conMatrix = [A;Aeq]
    nbCon = size(conMatrix,1);
    conLB = [repmat(-%inf,size(A,1),1); beq];
    conUB = [b;beq] ;
    
    isInt = repmat(%f,1,nbVar);
    for i=1:size(intcon,2)
        isInt(intcon(i)) = %t
    end
    
    objSense = 1;

    //Changing into row vector
    lb = lb';
    ub = ub';
    objCoef = objCoef';
    

   [xopt,fopt,status,iter] = symphony_call(nbVar,nbCon,objCoef,isInt,lb,ub,conMatrix,conLB,conUB,objSense,options);

endfunction
