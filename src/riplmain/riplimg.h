/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		riplimg.h
 *		Header file declaring the image data type.
 *
 *		Version 1.1, last update: 20 January 1998.
 *
 *		History:
 *			20/1/98:		version 1.1.
 *			16/1/98:		slight modifications.
 *			17/12/97:	first version.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#ifndef _RIPLIMG_H_INCLUDED
#define _RIPLIMG_H_INCLUDED

#include "ripldefs.h"

typedef enum tagriplRGBPlane {
    rgbpRed,
    rgbpGreen,
    rgbpBlue,
    rgbpData
} riplRGBPlane;

typedef struct tagriplRGBImage {
    unsigned cols, rows;
    unsigned long size;
    riplGrey *red,
        *green,
        *blue,
        *data;
    bool grey_scale;
    riplGraphicFormat format;
} riplRGBImage;

/*
void miscConvertToGreyScale(riplRGBImage *pimage) {

    unsigned long i;
    riplGrey *red_ptr,
        *green_ptr,
        *blue_ptr;

    RIPL_VALIDATE_IMAGE(pimage)

    red_ptr=pimage->red;
    green_ptr=pimage->green;
    blue_ptr=pimage->blue;
    for (i=0; i<pimage->size; i++) {
        *red_ptr=RIPL_RED_WEIGHT*(*red_ptr)
            +RIPL_GREEN_WEIGHT*(*green_ptr)
            +RIPL_BLUE_WEIGHT*(*blue_ptr);
        red_ptr++;
        green_ptr++;
        blue_ptr++;
    }
    pimage->data=pimage->red;
    pimage->red=NULL;
    riplFree(pimage->green);
    riplFree(pimage->blue);
    pimage->grey_scale=true;
}

void miscCreateGreyMapFromImage(riplGreyMap *pgreymap,
    riplRGBImage *pimage,
    riplRGBPlane plane) {

    RIPL_VALIDATE(pgreymap->data==NULL)

    pgreymap->cols=pimage->cols;
    pgreymap->rows=pimage->rows;
    pgreymap->size=pimage->size;
    switch (plane) {
        case rgbpRed:
            pgreymap->data=pimage->red;
        case rgbpGreen:
            pgreymap->data=pimage->green;
        case rgbpBlue:
            pgreymap->data=pimage->blue;
        case rgbpData:
            pgreymap->data=pimage->data;
    }
}
*/
#endif
