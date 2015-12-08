/*
 *		misc.c
 *		Source file for miscellaneous image-processing support functions.
 *
 *		Version 1.1, last update: 9 May 1998.
 *
 *		History:
 *			9/5/98:		added miscSubimageF/G functions.
 *			10/2/98:		added rescaling functions.
 *			31/1/98:		renamed mathematical constants.
 *			23/1/98:		minor modification: M_PI --> RIPL_PI etc.
 *			21/1/98:		moved FFT functions to new file 'miscfft.c'.
 *			21/1/98:		added more FFT stuff.
 *			19/1/98:		added FFT functions.
 *			24/12/97:	bug in miscCarve fixed.
 *			16/12/97:	comments updated.
 *			15/12/97:	added miscCarve function.
 *			27/11/97:	first serious version.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "misc.h"
#include "riplallc.h"




/* Only needed for diagnostic purposes. */
#include <stdio.h>





/* Sets an entire output image to the specified grey level. */
void miscSetImage(const riplGreyMap *pgreyMap,
    riplGrey greyLevel) {

    RIPL_VALIDATE_GREYMAP(pgreyMap)

    memset(pgreyMap->data, (int)greyLevel, pgreyMap->size);
}

/*
 *	Generate a 1D Gaussian kernel of the specified variance.
 *	The size of the kernel is pointed to by pkernelSize.
 */
bool miscGenerateGaussian(double var,
    unsigned *pkernelSize,
    double **pkernel) {

    int centre;
    unsigned i1, i2, count=1;
    double c1, c2;

    RIPL_VALIDATE(var>=0)

    centre=riplRound(3.7*var-0.5);
    if (centre<0) centre=0;
    i1=i2=centre;
    *pkernelSize=2*i1+1;
    *pkernel=(double *)riplCalloc(*pkernelSize, sizeof(double));
    if (*pkernel==NULL) return false;
    (*pkernel)[i1]=c1=1.0/var/RIPL_M_SQRT_TWICE_PI;
    c2=0.5/var/var;
    while (i1>0) {
        i1--;
        i2++;
        (*pkernel)[i1]=(*pkernel)[i2]=c1*exp(-(double)(count*count*c2));
        count++;
    }
    return true;
}

/*
 *	Convolve a sequence and kernel to generate a result
 *	the same size of the original sequence.
 *
 *		pinputData			pointer to input 1D sequence (already allocated)
 *		poutputData			pointer to output 1D sequence (already allocated)
 *		sequence_length	length of input and output 1D sequences
 *		step					the increment between adjacent sequence elementspixels
 *		pkernel				pointer to kernel
 *		kernel_length		length of kernel
 *
 *		Produces output sequence of the same length of input (takes central part of result only).
 */
bool miscConvolve1DFP(const riplGrey *pinputData,
    riplGrey *poutputData,
    unsigned sequence_length,
    unsigned step,
    const double *pkernel,
    unsigned kernel_length) {

    const riplGrey *inP;
    unsigned xs=(kernel_length-1)/2, i, j;
    int lim1, lim2;
    riplGrey *ptr=poutputData;
    double temp;

    RIPL_VALIDATE(pinputData && poutputData && pkernel)
    RIPL_VALIDATE(sequence_length>0 && step>0 && kernel_length>0)

    for (i=xs; i<xs+sequence_length; i++) {
        temp=0;
        lim1=i-sequence_length+1;
        if (lim1<0) lim1=0;
        lim2=kernel_length-1;
        if (lim2>i) lim2=i;
        inP=pinputData+(i-lim1)*step;
        for (j=lim1; j<=lim2; j++, inP-=step) temp=temp+(*inP)*pkernel[j];
        *ptr=(riplGrey)riplRound(temp);
        ptr+=step;
    }
    return true;
}

/*
 *	Convolve an image and a 2D integer kernel to produce a result
 *	with the same dimensions as the original image.
 *	Assumes image is at least the same size as the kernel.
 *
 *		pinputData			pointer to input image (already allocated)
 *		poutputData			pointer to output image (already allocated)
 *		image_cols			no. of columns in images
 *		image_rows			no. of rows in images
 *		kernel				pointer to integer kernel
 *		kernel_cols			no. of columns in kernel
 *		kernel_rows			no. of rows in kernel
 *		scale					number by which each convolution sum is divided
 *		store_abs			TRUE: forces convolution sum to absolute value,
 *								FALSE: stores negative sums as zeroes.
 */
bool miscConvolve2DInt(const riplGrey *pinputData,
    riplGrey *poutputData,
    unsigned image_cols,
    unsigned image_rows,
    const int *pkernel,
    unsigned kernel_cols,
    unsigned kernel_rows,
    unsigned scale,
    bool store_abs) {

    unsigned col_offset=kernel_cols/2,
        row_offset=kernel_rows/2,
        col_ext=col_offset+image_cols-kernel_rows+1,
        row_ext=row_offset+image_rows-kernel_cols+1,
        r, c, k_col, k_row;
    const int *k_ptr;
    const riplGrey *inP, *ptr;
    riplGrey *outP;
    int sum;		/* Could well be negative as well as positive. */

    RIPL_VALIDATE(pinputData && poutputData && pkernel)
    RIPL_VALIDATE(image_cols>0 && image_rows>0)
    RIPL_VALIDATE(kernel_cols>0 && kernel_rows>0)
    RIPL_VALIDATE(scale>0)

    /* Set image to white to frame it. */
    memset(poutputData, RIPL_MAX_GREY, image_cols*image_rows);

    /* Do it! */
    inP=pinputData;
    outP=poutputData+row_offset*image_cols;
    for (r=row_offset; r<row_ext;
        r++, inP+=image_cols, outP+=image_cols) {
        for (c=col_offset; c<col_ext; c++) {
            sum=0;
            k_ptr=pkernel;
            ptr=inP+c-col_offset;
            for (k_col=0; k_col<kernel_cols; k_col++) {
                for (k_row=0; k_row<kernel_rows; k_row++) {
                    sum+=ptr[k_col]*(*k_ptr++);
                }
                ptr+=image_cols;
            }
            if (store_abs) sum=labs(sum);
            sum/=scale;
            sum=sum<0 ? 0:sum;
            sum=sum>RIPL_MAX_GREY ? RIPL_MAX_GREY:sum;
            outP[c]=(riplGrey)sum;
        }
    }
    return true;
}

/*
 *		As miscConvolve2DInt but with 'const double *' kernel.
 */
bool miscConvolve2DFP(const riplGrey *pinputData,
    riplGrey *poutputData,
    unsigned image_cols,
    unsigned image_rows,
    const double *pkernel,
    unsigned kernel_cols,
    unsigned kernel_rows,
    unsigned scale,
    bool store_abs) {

    unsigned col_offset=kernel_cols/2,
        row_offset=kernel_rows/2,
        col_ext=col_offset+image_cols-kernel_rows+1,
        row_ext=row_offset+image_rows-kernel_cols+1,
        r, c, k_col, k_row;
    const double *k_ptr;
    const riplGrey *inP, *ptr;
    riplGrey *outP;
    double sum;

    RIPL_VALIDATE(pinputData && poutputData && pkernel)
    RIPL_VALIDATE(image_cols>0 && image_rows>0)
    RIPL_VALIDATE(kernel_cols>0 && kernel_rows>0)
    RIPL_VALIDATE(scale>0)

    /* Set image to white to frame it. */
    memset(poutputData, RIPL_MAX_GREY, image_cols*image_rows);

    /* Do it! */
    inP=pinputData;
    outP=poutputData+row_offset*image_cols;
    for (r=row_offset; r<row_ext;
        r++, inP+=image_cols, outP+=image_cols) {
        for (c=col_offset; c<col_ext; c++) {
            sum=0;
            k_ptr=pkernel;
            ptr=inP+c-col_offset;
            for (k_col=0; k_col<kernel_cols; k_col++) {
                for (k_row=0; k_row<kernel_rows; k_row++) {
                    sum+=ptr[k_col]*(*k_ptr++);
                }
                ptr+=image_cols;
            }
            if (store_abs) sum=fabs(sum);
            sum/=scale;
            sum=sum<0 ? 0:sum;
            sum=sum>RIPL_MAX_GREY ? RIPL_MAX_GREY:sum;
            outP[c]=(riplGrey)sum;
        }
    }
    return true;
}

/*
 * Fills specified array with a window function
 * of the specified type (float).
 */
void miscGenerateWindowF(miscWindowFunction win_func,
    float *win_data,
    unsigned win_length) {

    RIPL_VALIDATE(win_data && win_length>0)

    switch (win_func) {
        case wfRectangular:
            {
                unsigned i;
                for (i=0; i<win_length; i++) win_data[i]=1;
            }
            return;
        case wfHanning:
            {
                unsigned i;
                float c=RIPL_M_TWICE_PI/(win_length-1);
                for (i=0; i<win_length; i++) {
                    win_data[i]=0.5-0.5*cos(c*i);
                }
            }
            return;
        case wfHamming:
            {
                unsigned i;
                float c=RIPL_M_TWICE_PI/(win_length-1);
                for (i=0; i<win_length; i++) {
                    win_data[i]=0.54-0.46*cos(c*i);
                }
            }
            return;
        case wfTriangular:
            {
                unsigned h_len=win_length/2;
                float delta=2/(float)(h_len*2+2), value=1-delta;
                unsigned i=h_len-1, j=h_len;
                if (win_length/2*2!=win_length) {
                    win_data[i+1]=1;
                    j++;
                }
                for (; j<win_length; i--, j++, value-=delta) {
                    win_data[i]=win_data[j]=value;
                }
            }
            return;
    }
}

/*
 * Fills specified array with a window function
 * of the specified type (double).
 */
void miscGenerateWindowD(miscWindowFunction win_func,
    double *win_data,
    unsigned win_length) {

    RIPL_VALIDATE(win_data && win_length>0)

    switch (win_func) {
        case wfRectangular:
            {
                unsigned i;
                for (i=0; i<win_length; i++) win_data[i]=1;
            }
            return;
        case wfHanning:
            {
                unsigned i;
                double c=RIPL_M_TWICE_PI/(win_length-1);
                for (i=0; i<win_length; i++) {
                    win_data[i]=0.5-0.5*cos(c*i);
                }
            }
            return;
        case wfHamming:
            {
                unsigned i;
                double c=RIPL_M_TWICE_PI/(win_length-1);
                for (i=0; i<win_length; i++) {
                    win_data[i]=0.54-0.46*cos(c*i);
                }
            }
            return;
        case wfTriangular:
            {
                unsigned h_len=win_length/2;
                double delta=2/(double)(h_len*2+2), value=1-delta;
                unsigned i=h_len-1, j=h_len;
                if (win_length/2*2!=win_length) {
                    win_data[i+1]=1;
                    j++;
                }
                for (; j<win_length; i--, j++, value-=delta) {
                    win_data[i]=win_data[j]=value;
                }
            }
            return;
    }
}

/* Multiplies vector by window of specified type (float). */
void miscApplyWindowF(miscWindowFunction win_func,
    float *win_data,
    unsigned win_length) {

    RIPL_VALIDATE(win_data && win_length>0)

    switch (win_func) {
        case wfRectangular:
            return;
        case wfHanning:
            {
                unsigned i;
                float c=RIPL_M_TWICE_PI/(win_length-1);
                for (i=0; i<win_length; i++) {
                    win_data[i]*=0.5-0.5*cos(c*i);
                }
            }
            return;
        case wfHamming:
            {
                unsigned i;
                float c=RIPL_M_TWICE_PI/(win_length-1);
                for (i=0; i<win_length; i++) {
                    win_data[i]*=0.54-0.46*cos(c*i);
                }
            }
            return;
        case wfTriangular:
            {
                unsigned h_len=win_length/2;
                float delta=2/(float)(h_len*2+2), value=1-delta;
                unsigned i=h_len-1, j=h_len;
                for (; j<win_length; i--, j++, value-=delta) {
                    win_data[i]*=value;
                    win_data[j]*=value;
                }
            }
            return;
    }
}

/* Multiplies vector by window of specified type (double). */
void miscApplyWindowD(miscWindowFunction win_func,
    double *win_data,
    unsigned win_length) {

    RIPL_VALIDATE(win_data && win_length>0)

    switch (win_func) {
        case wfRectangular:
            return;
        case wfHanning:
            {
                unsigned i;
                double c=RIPL_M_TWICE_PI/(win_length-1);
                for (i=0; i<win_length; i++) {
                    win_data[i]*=0.5-0.5*cos(c*i);
                }
            }
            return;
        case wfHamming:
            {
                unsigned i;
                double c=RIPL_M_TWICE_PI/(win_length-1);
                for (i=0; i<win_length; i++) {
                    win_data[i]*=0.54-0.46*cos(c*i);
                }
            }
            return;
        case wfTriangular:
            {
                unsigned h_len=win_length/2;
                double delta=2/(double)(h_len*2+2), value=1-delta;
                unsigned i=h_len-1, j=h_len;
                for (; j<win_length; i--, j++, value-=delta) {
                    win_data[i]*=value;
                    win_data[j]*=value;
                }
            }
            return;
    }
}

/*
 * Perform simplified histogram equalization on the specified array
 * of pixels.
 *
 *		pinputData			pointer to input image buffer (already allocated)
 *		poutputData			pointer to output image buffer (already allocated)
 *		pixels				number of pixels in buffers
 *
 * pinputData and poutputData can point to the same array (equalization
 * can be performed in-place).
 */
bool miscHistogramEQ(const riplGrey *pinputData,
    riplGrey *poutputData,
    unsigned pixels) {

    const riplGrey *inP;
    riplGrey min, max;
    static unsigned hist[RIPL_GREY_LEVELS], cum_hist[RIPL_GREY_LEVELS];
    riplGrey *outP;
    unsigned i;
    static riplGrey map[RIPL_GREY_LEVELS];

    RIPL_VALIDATE(pinputData)
    RIPL_VALIDATE(poutputData)
    RIPL_VALIDATE(pixels>0)

    /* Find minimum and maximum grey levels. */
    inP=pinputData;
    min=max=*inP++;
    for (i=1; i<pixels; i++, inP++) {
        if (*inP<min) min=*inP;
        if (*inP>max) max=*inP;
    }

    /* Subtract minimum level from each pixel and form histogram. */
    memset(hist, 0, RIPL_GREY_LEVELS*sizeof(unsigned));
    inP=pinputData;
    outP=poutputData;
    for (i=0; i<pixels; i++, inP++, outP++) {
        *outP=(riplGrey)(*inP-min);
        hist[*outP]++;
    }

    /*
     * Form cumulative histogram of image and rescale it so that
     * values are in range 0-RIPL_MAX_GREY.
     */
    cum_hist[0]=0;
    map[0]=0;
    for (i=1; i<RIPL_GREY_LEVELS; i++) {
        cum_hist[i]=cum_hist[i-1]+hist[i-1];
        map[i]=(riplGrey)((double)cum_hist[i]/pixels*RIPL_GREY_LEVELS);
    }

    /* Perform remapping based on cumulative histogram. */
    outP=poutputData;
    for (i=0; i<pixels; i++, outP++) *outP=map[*outP];

    /* Finished! */
    return true;
}

bool *miscGenerateMorphStruct(miscPredefinedStruct predef,
    unsigned struct_el_cols,
    unsigned struct_el_rows) {

    unsigned size=struct_el_cols*struct_el_rows;
    bool *element=(bool *)riplCalloc(size, sizeof(bool));

    RIPL_VALIDATE(predef!=psInvalid)
    RIPL_VALIDATE(element)
    switch (predef) {
        case psOnes:
            {
                unsigned i;
                for (i=0; i<size; i++) element[i]=1;
            }
            break;
        case psCross:
            {
                unsigned i, hnk=(struct_el_cols-1)/2,
                    hmk=(struct_el_rows-1)/2;
                for (i=0; i<struct_el_rows; i++) {
                    element[i*struct_el_cols+hnk]=1;
                }
                for (i=0; i<struct_el_cols; i++) {
                    element[hmk*struct_el_cols+i]=1;
                }
            }
            break;
        case psKite:
            {
                unsigned i, j,
                    hnk=(struct_el_cols-1)/2,
                    hmk=(struct_el_rows-1)/2,
                    lim=(struct_el_cols/2*2==struct_el_cols) ? 1:0;
                for (i=0; i<=hnk && i<=hmk; i++) {
                    for (j=hnk-i; j<=lim+hnk+i; j++) {
                        element[i*struct_el_cols+j]=
                            element[(struct_el_rows-i-1)*struct_el_cols+j]=1;
                    }
                }
                for (; i<=hmk; i++) {
                    for (j=0; j<struct_el_cols; j++) {
                        element[i*struct_el_cols+j]=
                            element[(struct_el_rows-i-1)*struct_el_cols+j]=1;
                    }
                }
            }
            break;
        case psDiag:
            {
                unsigned i, lim=(struct_el_cols<struct_el_rows)
                    ? struct_el_cols:struct_el_rows;
                for (i=0; i<lim; i++) {
                    element[i*struct_el_cols+i]=1;
                }
            }
            break;
    }
    return element;
}

/* Structs and defines used by miscCarve. */
struct tagwindow {
    unsigned left, top;
    unsigned width, height;
    unsigned nearest, level;
};
#define MAX_UNCLASSIFIED		1000

/*
 * Carves up an image based on mean grey level.
 * See 4th-year logbook.
 */
bool miscCarve(riplGreyMap *pgreymap,
    riplGrey conf,
    unsigned nclasses,
    const riplGrey *boundary,
    unsigned max_win_width,
    unsigned max_win_height,
    unsigned min_win_width,
    unsigned min_win_height,
    miscSubimageFunc function,
    void *arg) {

    /* Queue of unclassified subimages. */
    struct tagwindow subimage[MAX_UNCLASSIFIED];
    unsigned qget, qput, nsubimages;

    unsigned xoffset, yoffset;
    unsigned subimage_width, subimage_height;
    unsigned nearest, level;
    unsigned win_width, win_height;
    unsigned xwin, ywin, xedge, yedge;
    unsigned i, j, r, c;
    unsigned left, top, width, height;
    const riplGrey *inP;
    double temp;
    riplGrey mean, distance, diff;

    RIPL_VALIDATE_GREYMAP(pgreymap)

    subimage[0].left=0;
    subimage[0].top=0;
    subimage[0].width=pgreymap->cols;
    subimage[0].height=pgreymap->rows;
    subimage[0].nearest=nclasses;
    subimage[0].level=0;
    nsubimages=1;
    qget=0;
    qput=1;

    do {
        xoffset=subimage[qget].left;
        yoffset=subimage[qget].top;
        subimage_width=subimage[qget].width;
        subimage_height=subimage[qget].height;
        nearest=subimage[qget].nearest;
        level=subimage[qget].level;
        qget++;
        nsubimages--;
        if (qget==MAX_UNCLASSIFIED) qget=0;

        win_width=max_win_width>>level;
        win_height=max_win_height>>level;
        if (win_width<=min_win_width || win_height<=min_win_height) {
            /*
             * Too small to carve up any further.
             * Therefore assign to nearest class.
             */
            if (!function(pgreymap,
                xoffset, yoffset, subimage_width, subimage_height,
                nearest, arg))
                return false;
            continue;
        }

        xwin=subimage_width/win_width;
        xedge=subimage_width%win_width;
        if (xedge) xwin++;

        ywin=subimage_height/win_height;
        yedge=subimage_height%win_height;
        if (yedge) ywin++;

        height=win_height;
        for (i=ywin, top=yoffset; i>0; i--, top+=win_height) {
            if (i==1 && yedge) height=yedge;
            width=win_width;
            for (j=xwin, left=xoffset; j>0; j--, left+=win_width) {
                if (j==1 && xedge) width=xedge;
                inP=pgreymap->data+top*pgreymap->cols+left;
                temp=0.0;
                for (r=height; r>0; r--, inP+=pgreymap->cols-width) {
                    for (c=width; c>0; c--, inP++) {
                        temp+=*inP;
                    }
                }
                mean=(riplGrey)riplRound(temp/height/width);
                if (mean<=boundary[0]+conf) {
                    nearest=0;
                    diff=(riplGrey)abs((int)boundary[nearest]-mean);
                }
                else if (mean>=boundary[nclasses-1]-conf) {
                    nearest=nclasses-1;
                    diff=(riplGrey)abs((int)boundary[nearest]-mean);
                }
                else {
                    nearest=nclasses;
                    diff=RIPL_MAX_GREY;
                    for (r=0; r<nclasses; r++) {
                        distance=(riplGrey)abs((int)boundary[r]-mean);
                        if (distance<diff) {
                            nearest=r;
                            diff=distance;
                        }
                    }
                }
                if (diff<=conf) {
                    if (!function(pgreymap,
                        left, top, width, height,
                        nearest, arg))
                        return false;
                }
                else {
                    if (nsubimages==MAX_UNCLASSIFIED) {
                        riplMessage(itError,
                            "miscCarve: Too many marginal subimages!\n");
                        return false;
                    }
                    subimage[qput].left=left;
                    subimage[qput].top=top;
                    subimage[qput].width=width;
                    subimage[qput].height=height;
                    subimage[qput].nearest=nearest;
                    subimage[qput].level=level+1;
                    qput++;
                    if (qput==MAX_UNCLASSIFIED) qput=0;
                    nsubimages++;
                }
            }
        }
    } while (nsubimages);
    if (nsubimages==0) return true;
    return false;
}

/*
 * Rescaling functions added 10 February 1998.
 */

/*
 * (Float version).
 * Writes rescaled grey levels to 'out_vector'.
 */
void miscRescaleF(float *in_vector,
    riplGrey *out_vector,
    unsigned long size) {

    unsigned long i;
    float min, max;

    RIPL_VALIDATE(in_vector && out_vector && size>0)

    /* Find minimum and maximum values of output matrix. */
    min=max=in_vector[0];
    for (i=1; i<size; i++) {
        if (in_vector[i]>max) max=in_vector[i];
        if (in_vector[i]<min) min=in_vector[i];
    }

    /* Scale output to grey levels. */
    if (max!=0.0) {
        for (i=0; i<size; i++) {
            out_vector[i]=(riplGrey)(RIPL_MAX_GREY*(in_vector[i]-min)/max);
        }
    }
    else memset(out_vector, 0, size);
}

/*
 * (Double version).
 * Writes rescaled grey levels to 'out_vector'.
 */
void miscRescaleD(double *in_vector,
    riplGrey *out_vector,
    unsigned long size) {

    unsigned long i;
    double min, max;

    RIPL_VALIDATE(in_vector && out_vector && size>0)

    /* Find minimum and maximum values of output matrix. */
    min=max=in_vector[0];
    for (i=1; i<size; i++) {
        if (in_vector[i]>max) max=in_vector[i];
        if (in_vector[i]<min) min=in_vector[i];
    }

    /* Scale output to grey levels. */
    if (max!=0.0) {
        for (i=0; i<size; i++) {
            out_vector[i]=(riplGrey)(RIPL_MAX_GREY*(in_vector[i]-min)/max);
        }
    }
    else memset(out_vector, 0, size);
}

/*
 * (riplGrey version).
 * Writes rescaled grey levels to 'out_vector'.
 * 'in_vector' and 'out_vector' can point to the same memory block.
 */
void miscRescaleG(riplGrey *in_vector,
    riplGrey *out_vector,
    unsigned long size) {

    unsigned long i;
    riplGrey min, max;

    RIPL_VALIDATE(in_vector && out_vector && size>0)

    /* Find minimum and maximum values of output matrix. */
    min=max=in_vector[0];
    for (i=1; i<size; i++) {
        if (in_vector[i]>max) max=in_vector[i];
        if (in_vector[i]<min) min=in_vector[i];
    }

    /* Scale output to grey levels. */
    if (max!=0.0) {
        for (i=0; i<size; i++) {
            out_vector[i]=(riplGrey)
                (RIPL_MAX_GREY*((double)in_vector[i]-min)/(double)max);
        }
    }
    else memset(out_vector, 0, size);
}

/*
 * (riplMidGrey version).
 * Writes rescaled grey levels to 'out_vector'.
 */
void miscRescaleMG(riplMidGrey *in_vector,
    riplGrey *out_vector,
    unsigned long size) {

    unsigned long i;
    riplMidGrey min, max;

    RIPL_VALIDATE(in_vector && out_vector && size>0)

    /* Find minimum and maximum values of output matrix. */
    min=max=in_vector[0];
    for (i=1; i<size; i++) {
        if (in_vector[i]>max) max=in_vector[i];
        if (in_vector[i]<min) min=in_vector[i];
    }

    /* Scale output to grey levels. */
    if (max!=0.0) {
        for (i=0; i<size; i++) {
            out_vector[i]=(riplGrey)
                (RIPL_MAX_GREY*((double)in_vector[i]-min)/(double)max);
        }
    }
    else memset(out_vector, 0, size);
}

/*
 * (riplBigGrey version).
 * Writes rescaled grey levels to 'out_vector'.
 */
void miscRescaleBG(riplBigGrey *in_vector,
    riplGrey *out_vector,
    unsigned long size) {

    unsigned long i;
    riplBigGrey min, max;

    RIPL_VALIDATE(in_vector && out_vector && size>0)

    /* Find minimum and maximum values of output matrix. */
    min=max=in_vector[0];
    for (i=1; i<size; i++) {
        if (in_vector[i]>max) max=in_vector[i];
        if (in_vector[i]<min) min=in_vector[i];
    }

    /* Scale output to grey levels. */
    if (max!=0.0) {
        for (i=0; i<size; i++) {
            out_vector[i]=(riplGrey)
                (RIPL_MAX_GREY*((double)in_vector[i]-min)/(double)max);
        }
    }
    else memset(out_vector, 0, size);
}

/*
 * miscSubimageF---added 9 May 1998.
 *
 * Arguments:
 *		pinput		input image buffer (already allocated)
 *		poutput		output image buffer (already allocated, can == pinput)
 *		rows, cols	dimensions of image
 *		w_r, w_c		dimensions of window
 *		d_r, d_c		rowwise and columnwise overlap
 *		wp				window profile to employ
 *		func			transform to apply to each subimage
 *		arg			optional arguments to pass to `func'.
 *
 * Implements extended raised-cosine (or triangular) 2-D window as described
 * in project report `Image-Processing Techniques with Application in Image
 * Restoration' by Richard A. Cook.
 *
 * Processing function expects float buffer.
 */
bool miscSubimageF(const riplGrey *pinput,
    riplGrey *poutput,
    unsigned rows,				/* originally unsigned long */
    unsigned cols,				/* originally unsigned long */
    unsigned w_r,
    unsigned w_c,
    unsigned d_r,
    unsigned d_c,
    miscWindowProfile wp,
    miscSubimageFunctionF func,
    void *arg) {

    unsigned nw_r, nw_c;
    unsigned nr_r, nr_c,
        ns_r, ne_r,
        ns_c, ne_c,
        wl_r_l, wr_r_l,
        wl_c_l, wr_c_l;		/* originally unsigned long */
    float *win_r, *win_c;
    unsigned i, j;
    unsigned row, col,
        start_r, end_r,
        start_c, end_c;		/* originally unsigned long */
    bool i1, i2,
        j1, j2;
    riplGrey *ptr;
    const riplGrey *cptr;
    riplGrey *temp_g;
    float *temp, *fptr1, *fptr2;
    float *acc;
    unsigned l_r, l_c,
        k1, k2;					/* originally unsigned long */

    RIPL_VALIDATE(pinput && poutput && func)
    if (w_r>rows || w_c>cols) return false;				/* Window too big. */
    if (d_r>(w_r>>1) || d_c>(w_c>>1)) return false;		/* Overlap too big. */
    /* Allocate memory for window, accumulator image and profiles. */
    temp_g=(riplGrey *)riplCalloc(w_r*w_c, sizeof(riplGrey));
    RIPL_VALIDATE(temp_g)
    temp=(float *)riplCalloc(w_r*w_c, sizeof(float));
    RIPL_VALIDATE(temp)
    acc=(float *)riplCalloc(rows*cols, sizeof(float));
    RIPL_VALIDATE(acc)
    win_r=(float *)riplCalloc(d_r, sizeof(float));
    RIPL_VALIDATE(win_r)
    win_c=(float *)riplCalloc(d_c, sizeof(float));
    RIPL_VALIDATE(win_c)
    /* Number of windows in row and column directions. */
    nw_r=(unsigned)(rows-d_r)/(w_r-d_r);
    nw_c=(unsigned)(cols-d_c)/(w_c-d_c);
    /* No. of spare pixels at start and end of rows and columns. */
    nr_r=rows-(w_r+(nw_r-1)*(w_r-d_r));
    nr_c=cols-(w_c+(nw_c-1)*(w_c-d_c));
    /* No. of padding pixels at start and end of rows and columns. */
    ns_r = ceil(static_cast<float>(nr_r) / 2);
    ne_r=nr_r-ns_r;
    ns_c = ceil(static_cast<float>(nr_c) / 2);
    ne_c=nr_c-ns_c;
    /* Edge corrections. */
    wl_r_l=w_r-d_r;
    if (ns_r>0) {
        wl_r_l=ns_r;
        nw_r=nw_r+1;
    }
    wr_r_l=w_r-d_r;
    if (ne_r>0) {
        wr_r_l=ne_r;
        nw_r=nw_r+1;
    }
    wl_c_l=w_c-d_c;
    if (ns_c>0) {
        wl_c_l=ns_c;
        nw_c=nw_c+1;
    }
    wr_c_l=w_c-d_c;
    if (ne_c>0) {
        wr_c_l=ne_c;
        nw_c=nw_c+1;
    }
    /* Generate window profiles. */
    switch (wp) {
    case wpCosine:
        for (i=0; i<d_r; i++) win_r[i]=0.5*(1.0+cos((float)i*RIPL_M_PI/(d_r-1)));
        for (i=0; i<d_c; i++) win_c[i]=0.5*(1.0+cos((float)i*RIPL_M_PI/(d_c-1)));
        break;
    case wpTriangular:
        for (i=0; i<d_r; i++) win_r[i]=(float)i/(d_r-1);
        for (i=0; i<d_c; i++) win_c[i]=(float)i/(d_c-1);
        break;
    }
    /* Loop through windows. */
    row=0;
    for (i=0; i<nw_r; i++) {
        if (i==0) {
            start_r=0;
            i1=false;
            i2=true;
            end_r=wl_r_l+d_r-1;
        }
        else {
            if (i==nw_r-1) {
                start_r=row;
                i1=true;
                i2=false;
                end_r=row+wr_r_l+d_r-1;
            }
            else {
                start_r=row;
                i1=i2=true;
                end_r=row+w_r-1;
            }
        }
        col=0;
        for (j=0; j<nw_c; j++) {
            if (j==0) {
                start_c=0;
                j1=false;
                j2=true;
                end_c=wl_c_l+d_c-1;
            }
            else {
                if (j==nw_c-1) {
                    start_c=col;
                    j1=true;
                    j2=false;
                    end_c=col+wr_c_l+d_c-1;
                }
                else {
                    start_c=col;
                    j1=j2=true;
                    end_c=col+w_c-1;
                }
            }
            /* Copy region of image into 'temp'. */
            l_r=end_r-start_r+1;
            l_c=end_c-start_c+1;
            cptr=pinput+start_r*cols+start_c;
            fptr1=temp;
            for (k1=0; k1<l_r; k1++) {
                for (k2=0; k2<l_c; k2++) fptr1[k2]=(float)cptr[k2];
                cptr+=cols;
                fptr1+=l_c;
            }
            /* Applying processing here. */
            func(temp, l_r, l_c, arg);
            /* Multiply by window. */

            if (i1==true) {
                for (k1=0; k1<l_c; k1++) {
                    for (k2=0; k2<d_r; k2++) temp[k2*l_c+k1]*=win_r[d_r-k2-1];
                }
            }
            if (i2==true) {
                for (k1=0; k1<l_c; k1++) {
                    for (k2=0; k2<d_r; k2++) temp[(l_r-d_r+k2)*l_c+k1]*=win_r[k2];
                }
            }
            if (j1==true) {
                for (k1=0; k1<l_r; k1++) {
                    for (k2=0; k2<d_c; k2++) temp[k1*l_c+k2]*=win_c[d_c-k2-1];
                }
            }
            if (j2==true) {
                for (k1=0; k1<l_r; k1++) {
                    for (k2=0; k2<d_c; k2++) temp[k1*l_c+l_c-d_c+k2]*=win_c[k2];
                }
            }
            /* Add 'temp' image into accumulated image. */
            fptr1=temp;
            fptr2=acc+start_r*cols+start_c;
            for (k1=0; k1<l_r; k1++) {
                for (k2=0; k2<l_c; k2++) fptr2[k2]+=fptr1[k2];
                fptr1+=l_c;
                fptr2+=cols;
            }
            if (j==0) col=wl_c_l;
            else col+=w_c-d_c;
        }
        if (i==0) row=wl_r_l;
        else row+=w_r-d_r;
    }
    ptr=poutput;
    fptr1=acc;
    for (k1=0; k1<rows; k1++) {
        for (k2=0; k2<cols; k2++) {
            ptr[k2]=(riplGrey)fptr1[k2];
        }
        ptr+=cols;
        fptr1+=cols;
    }
    riplFree(temp_g);
    riplFree(temp);
    riplFree(acc);
    riplFree(win_r);
    riplFree(win_c);
    return true;
}

/*
 * miscSubimageG---added 9 May 1998.
 *
 * Arguments:
 *		pinput		input image buffer (already allocated)
 *		poutput		output image buffer (already allocated, can == pinput)
 *		rows, cols	dimensions of image
 *		w_r, w_c		dimensions of window
 *		d_r, d_c		rowwise and columnwise overlap
 *		wp				window profile to employ
 *		func			transform to apply to each subimage
 *		arg			optional arguments to pass to `func'.
 *
 * Implements extended raised-cosine (or triangular) 2-D window as described
 * in project report `Image-Processing Techniques with Application in Image
 * Restoration' by Richard A. Cook.
 *
 * Processing function expects riplGrey buffer.
 */
bool miscSubimageG(const riplGrey *pinput,
    riplGrey *poutput,
    unsigned rows,				/* originally unsigned long */
    unsigned cols,				/* originally unsigned long */
    unsigned w_r,
    unsigned w_c,
    unsigned d_r,
    unsigned d_c,
    miscWindowProfile wp,
    miscSubimageFunctionG func,
    void *arg) {

    unsigned nw_r, nw_c;
    unsigned nr_r, nr_c,
        ns_r, ne_r,
        ns_c, ne_c,
        wl_r_l, wr_r_l,
        wl_c_l, wr_c_l;		/* originally unsigned long */
    float *win_r, *win_c;
    unsigned i, j;
    unsigned row, col,
        start_r, end_r,
        start_c, end_c;		/* originally unsigned long */
    bool i1, i2,
        j1, j2;
    riplGrey *ptr;
    const riplGrey *cptr;
    riplGrey *temp_g;
    float *temp, *fptr1, *fptr2;
    float *acc;
    unsigned l_r, l_c,
        k1, k2;					/* originally unsigned long */

    RIPL_VALIDATE(pinput && poutput && func)
    if (w_r>rows || w_c>cols) return false;				/* Window too big. */
    if (d_r>(w_r>>1) || d_c>(w_c>>1)) return false;		/* Overlap too big. */
    /* Allocate memory for window, accumulator image and profiles. */
    temp_g=(riplGrey *)riplCalloc(w_r*w_c, sizeof(riplGrey));
    RIPL_VALIDATE(temp_g)
    temp=(float *)riplCalloc(w_r*w_c, sizeof(float));
    RIPL_VALIDATE(temp)
    acc=(float *)riplCalloc(rows*cols, sizeof(float));
    RIPL_VALIDATE(acc)
    win_r=(float *)riplCalloc(d_r, sizeof(float));
    RIPL_VALIDATE(win_r)
    win_c=(float *)riplCalloc(d_c, sizeof(float));
    RIPL_VALIDATE(win_c)
    /* Number of windows in row and column directions. */
    nw_r=(unsigned)(rows-d_r)/(w_r-d_r);
    nw_c=(unsigned)(cols-d_c)/(w_c-d_c);
    /* No. of spare pixels at start and end of rows and columns. */
    nr_r=rows-(w_r+(nw_r-1)*(w_r-d_r));
    nr_c=cols-(w_c+(nw_c-1)*(w_c-d_c));
    /* No. of padding pixels at start and end of rows and columns. */
    ns_r = ceil(static_cast<float>(nr_r) / 2);
    ne_r=nr_r-ns_r;
    ns_c = ceil(static_cast<float>(nr_c) / 2);
    ne_c=nr_c-ns_c;
    /* Edge corrections. */
    wl_r_l=w_r-d_r;
    if (ns_r>0) {
        wl_r_l=ns_r;
        nw_r=nw_r+1;
    }
    wr_r_l=w_r-d_r;
    if (ne_r>0) {
        wr_r_l=ne_r;
        nw_r=nw_r+1;
    }
    wl_c_l=w_c-d_c;
    if (ns_c>0) {
        wl_c_l=ns_c;
        nw_c=nw_c+1;
    }
    wr_c_l=w_c-d_c;
    if (ne_c>0) {
        wr_c_l=ne_c;
        nw_c=nw_c+1;
    }
    /* Generate window profiles. */
    switch (wp) {
    case wpCosine:
        for (i=0; i<d_r; i++) win_r[i]=0.5*(1.0+cos((float)i*RIPL_M_PI/(d_r-1)));
        for (i=0; i<d_c; i++) win_c[i]=0.5*(1.0+cos((float)i*RIPL_M_PI/(d_c-1)));
        break;
    case wpTriangular:
        for (i=0; i<d_r; i++) win_r[i]=(float)i/(d_r-1);
        for (i=0; i<d_c; i++) win_c[i]=(float)i/(d_c-1);
        break;
    }
    /* Loop through windows. */
    row=0;
    for (i=0; i<nw_r; i++) {
        if (i==0) {
            start_r=0;
            i1=false;
            i2=true;
            end_r=wl_r_l+d_r-1;
        }
        else {
            if (i==nw_r-1) {
                start_r=row;
                i1=true;
                i2=false;
                end_r=row+wr_r_l+d_r-1;
            }
            else {
                start_r=row;
                i1=i2=true;
                end_r=row+w_r-1;
            }
        }
        col=0;
        for (j=0; j<nw_c; j++) {
            if (j==0) {
                start_c=0;
                j1=false;
                j2=true;
                end_c=wl_c_l+d_c-1;
            }
            else {
                if (j==nw_c-1) {
                    start_c=col;
                    j1=true;
                    j2=false;
                    end_c=col+wr_c_l+d_c-1;
                }
                else {
                    start_c=col;
                    j1=j2=true;
                    end_c=col+w_c-1;
                }
            }
            /* Copy region of image into 'temp'. */
            l_r=end_r-start_r+1;
            l_c=end_c-start_c+1;
            cptr=pinput+start_r*cols+start_c;
            ptr=temp_g;
            for (k1=0; k1<l_r; k1++) {
                for (k2=0; k2<l_c; k2++) ptr[k2]=cptr[k2];
                cptr+=cols;
                ptr+=l_c;
            }
            /* Apply processing here. */
            func(temp_g, l_r, l_c, arg);
            /* Copy 'temp_g' into 'temp'. */
            ptr=temp_g;
            fptr1=temp;
            for (k1=0; k1<l_r; k1++) {
                for (k2=0; k2<l_c; k2++) fptr1[k2]=(float)ptr[k2];
                ptr+=l_c;
                fptr1+=l_c;
            }
            /* Multiply by window. */
            if (i1==true) {
                for (k1=0; k1<l_c; k1++) {
                    for (k2=0; k2<d_r; k2++) temp[k2*l_c+k1]*=win_r[d_r-k2-1];
                }
            }
            if (i2==true) {
                for (k1=0; k1<l_c; k1++) {
                    for (k2=0; k2<d_r; k2++) temp[(l_r-d_r+k2)*l_c+k1]*=win_r[k2];
                }
            }
            if (j1==true) {
                for (k1=0; k1<l_r; k1++) {
                    for (k2=0; k2<d_c; k2++) temp[k1*l_c+k2]*=win_c[d_c-k2-1];
                }
            }
            if (j2==true) {
                for (k1=0; k1<l_r; k1++) {
                    for (k2=0; k2<d_c; k2++) temp[k1*l_c+l_c-d_c+k2]*=win_c[k2];
                }
            }
            /* Add 'temp' image into accumulated image. */
            fptr1=temp;
            fptr2=acc+start_r*cols+start_c;
            for (k1=0; k1<l_r; k1++) {
                for (k2=0; k2<l_c; k2++) fptr2[k2]+=fptr1[k2];
                fptr1+=l_c;
                fptr2+=cols;
            }
            if (j==0) col=wl_c_l;
            else col+=w_c-d_c;
        }
        if (i==0) row=wl_r_l;
        else row+=w_r-d_r;
    }
    ptr=poutput;
    fptr1=acc;
    for (k1=0; k1<rows; k1++) {
        for (k2=0; k2<cols; k2++) {
            ptr[k2]=(riplGrey)fptr1[k2];
        }
        ptr+=cols;
        fptr1+=cols;
    }
    riplFree(temp_g);
    riplFree(temp);
    riplFree(acc);
    riplFree(win_r);
    riplFree(win_c);
    return true;
}

