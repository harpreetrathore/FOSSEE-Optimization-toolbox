mode(1)
//
// Demo of qpipopt.sci
//

//Find x in R^6 such that:
conMatrix= [1,-1,1,0,3,1;
-1,0,-3,-4,5,6;
2,5,3,0,1,0
0,1,0,1,2,-1;
-1,0,2,1,1,0];
conLB=[1;2;3;-%inf;-%inf];
conUB = [1;2;3;-1;2.5];
lb=[-1000;-10000; 0; -1000; -1000; -1000];
ub=[10000; 100; 1.5; 100; 100; 1000];
//and minimize 0.5*x'*Q*x + p'*x with
p=[1; 2; 3; 4; 5; 6]; Q=eye(6,6);
nbVar = 6;
nbCon = 5;
x0 = repmat(0,nbVar,1);
param = list("MaxIter", 300, "CpuTime", 100);
[xopt,fopt,exitflag,output,lambda]=qpipopt(nbVar,nbCon,Q,p,lb,ub,conMatrix,conLB,conUB,x0,param)
halt()   // Press return to continue
 
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
halt()   // Press return to continue
 
//========= E N D === O F === D E M O =========//
