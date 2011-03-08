/*
 *		ripltdfs.h
 *		Header file for general Ripl toolbox constants and definitions.
 *
 *		Version 1.1, last update: 30 January 1998.
 *
 *		History:
 *			30/1/98:		modified power-of-2 macros.
 *			25/1/98:		created first version.
 *
 *		Copyright © 1998, Richard A. Cook.
 */
#ifndef _RIPLTDFS_H_INCLUDED
#define _RIPLTDFS_H_INCLUDED

#include <math.h>
#include "ripl.h"

#define RIPL_IS_POWER_OF_2(n)				((log(n)/RIPL_M_LN2)==(unsigned)(log(n)/RIPL_M_LN2))
#define RIPL_VALIDATE_IS_POWER_OF_2(n)	RIPL_VALIDATE(RIPL_IS_POWER_OF_2(n))

/* Enumerated data type defining transform direction. */
typedef enum tagriplTransformType {
	ttForward=1,
	ttInverse=-1
} riplTransformType;

#endif

