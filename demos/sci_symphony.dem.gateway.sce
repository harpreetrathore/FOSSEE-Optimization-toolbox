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

demopath = get_absolute_file_path("sci_symphony.dem.gateway.sce");

subdemolist = ["Symphony", "symphony.dem.sce"; "SymphonyMat", "symphonymat.dem.sce"; "Qpipopt", "qpipopt.dem.sce"; "QpipoptMat", "qpipoptmat.dem.sce";];

subdemolist(:,2) = demopath + subdemolist(:,2);
