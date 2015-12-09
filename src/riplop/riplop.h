/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		riplop.h
 *		Header file for operator stuff.
 *
 *		Version 1.1, last update: 24 January 1998.
 *
 *		History:
 *			24/1/98:		renamed RIPL_PARSEERROR to RIPL_PARAMERROR.
 *			23/1/98:		changes reflecting addition of new return code
 *							RIPL_USERERROR.
 *			20/1/98:		moved many functions into new file 'riplpars.c'.
 *			19/1/98:		inserted Watcom compiler preprocessor stuff
 *							(to suppress compiler warnings).
 *			27/11/97:	first implemented.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */

#pragma once

#include <functional>

#include "ripl.h"

/** Describes an operator */
struct riplOperator
{
private:
    /*
     * Operator execute function. This function is passed any remaining
     * command-line arguments and pointers to the input and output greymaps.
     * This function should return the number of arguments removed from
     * the command line or:
     *			RIPL_PARAMERROR			if a parse error occurs
     *			RIPL_EXECUTEERROR			if an execution error occurs
     *			RIPL_USERERROR				if the user supplies illogical values etc.
     */
    using ExecuteFunc = std::function<int(unsigned, const char**, riplGreyMap*, riplGreyMap*)>;

    // Provides helper string to be displayed to user
    using HelpFunc = std::function<const char*()>;

public:
    const char* name;
    unsigned match_chars;
    const char* comment;
    ExecuteFunc execute;
    HelpFunc help;
};

/*
 * This list of all available operators should be defined elsewhere
 * (most probably in riplreg.c source file which is defined by the
 * library developer.
 */
extern const riplOperator riplOperators[];
extern const unsigned riplNumOperators;

const char *riplGetOperatorSummary(void);
bool riplOperatorHelp(const char *op);
unsigned riplOperatorExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap);
