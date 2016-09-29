// Copyright (C) 2015 - IIT Bombay - FOSSEE
//
// This file must be used under the terms of the BSD.
// This source file is licensed as described in the file LICENSE, which
// you should have received as part of this distribution.  The terms
// are also available at
// https://opensource.org/licenses/BSD-3-Clause
// Author: Harpreet Singh
// Organization: FOSSEE, IIT Bombay
// Email: toolbox@scilab.in

function setOptions(varargin)

    options = varargin(1);
    nbOpt = size(options);
    

    if (nbOpt~=0) then
        for i = 1:(nbOpt/2)
        
        //Setting the parameters
          
            //Check if the given parameter is String
            if (type(options(2*i)) == 10 ) then
                sym_setStrParam(options(2*i - 1),options(2*i));
          
            //Check if the given parameter is Double
            elseif(type(options(2*i))==1) then
                sym_setDblParam(options(2*i - 1),options(2*i));
             
            //Check if the given parameter is Integer
            elseif(type(options(2*i))==8)
                sym_setIntParam(options(2*i - 1),options(2*i)); 
            end
         end
    end
    
endfunction

