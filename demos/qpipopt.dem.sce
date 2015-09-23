mode(1)
//
// Demo of qpipopt.sci
//

//Find x in R^6 such that:
halt()   // Press return to continue
 
conMatrix= [1,-1,1,0,3,1;
-1,0,-3,-4,5,6;
2,5,3,0,1,0
0,1,0,1,2,-1;
-1,0,2,1,1,0];
conLB=[1 2 3 -%inf -%inf]';
conUB = [1 2 3 -1 2.5]';
lb=[-1000 -10000 0 -1000 -1000 -1000];
ub=[10000 100 1.5 100 100 1000];
//and minimize 0.5*x'*Q*x + p'*x with
p=[1 2 3 4 5 6]; Q=eye(6,6);
nbVar = 6;
nbCon = 5;
[xopt,fopt,exitflag,output,lambda]=qpipopt(nbVar,nbCon,Q,p,lb,ub,conMatrix,conLB,conUB)
halt()   // Press return to continue
 
//min. -8*x1 -16*x2 + x1^2 + 4* x2^2
//  such that
//     x1 + x2 <= 5,
//     x1 <= 3,
//     x1 >= 0,
//     x2 >= 0
conMatrix= [1 1];
conLB=[-%inf];
conUB = [5];
lb=[0,0];
ub=[3,%inf];
//and minimize 0.5*x'*Q*x + p'*x with
p=[-8,-16];
Q=[1,0;0,4];
nbVar = 2;
nbCon = 1;
[xopt,fopt,exitflag,output,lambda] = qpipopt(nbVar,nbCon,Q,p,lb,ub,conMatrix,conLB,conUB)
halt()   // Press return to continue
 
//========= E N D === O F === D E M O =========//
