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

mode(-1)
lines(0)

//Symphony toolbox help builder
//By Keyur Joshi, Sai Kiran and Iswarya

toolbox_title = "Symphony"

help_dir = get_absolute_file_path('builder_help.sce');

tbx_builder_help_lang("en_US", help_dir);

clear toolbox_title;
