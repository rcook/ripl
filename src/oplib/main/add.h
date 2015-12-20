/*
 *		add.h
 *		Header file for adding operator.
 *
 *		Version 1.1, last update: 24 January 1998.
 *
 *		History:
            24/1/98:		introduced RIPL_PARAMERROR.
 *			16/12/97:	updated comments.
 *			15/12/97:	first implemented.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */

#pragma once

#include "ripl.h"
#include <vector>

namespace ripl { namespace oplib {

    /** Adds two images together with specified weight applied to second image */
    void add(riplGreyMap& output, const riplGreyMap& input0, const riplGreyMap& input1, float weight);

    /** Command-line entry point **/
    int addExecute(riplGreyMap& output, const std::vector<std::string>& args, const riplGreyMap& input);

    /** Provide help for operator */
    const char* addHelp();

}} // namespace ripl::oplib
