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

function [xopt,fopt,iter] = symphony_call(nbVar,nbCon,objCoef,isInt,LB,UB,conMatrix,conLB,conUB,objSense,options)

    //Opening Symphony environment 
    sym_open();

   //Choosing to launch basic or advanced version
    if(~issparse(conMatrix)) then
        sym_loadProblemBasic(nbVar,nbCon,LB,UB,objCoef,isInt,objSense,conMatrix,conLB,conUB);
    else
        // Changing to Constraint Matrix into sparse matrix
        conMatrix_advanced=sparse(conMatrix);
        sym_loadProblem(nbVar,nbCon,LB,UB,objCoef,isInt,objSense,conMatrix_advanced,conLB,conUB);
    end


    //Setting Options for the Symphpony
    setOptions(options);

    op = sym_solve();
    
    xopt = [];
    fopt = [];
    status = [];
    iter = [];
    
     if (~op) then
            xopt = sym_getVarSoln();
            fopt = sym_getObjVal();
            iter = sym_getIterCount();
    end
    
//    //Closing Symphony Environment
//    sym_close();

endfunction
