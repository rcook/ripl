/*
 *		binmorph.c
 *		Source file for binary morphological operators.
 *
 *		Version 1.1, last update: 24 January 1998.
 *
 *		History:
 *			24/1/98:		introduced RIPL_PARAMERROR.
 *			16/12/97:	updated comments.
 *			27/11/97:	first implemented.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#include "binmorph.h"
#include "thresh.h"

#define BINMORPH_THRESHOLD			(RIPL_GREY_LEVELS/2)

/* Prototypes of static functions. */
static bool erosion(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    unsigned struct_el_cols,
    unsigned struct_el_rows,
    bool *struct_el);
static bool dilation(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    unsigned struct_el_cols,
    unsigned struct_el_rows,
    bool *struct_el);
static bool opening(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    unsigned struct_el_cols,
    unsigned struct_el_rows,
    bool *struct_el);
static bool closing(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    unsigned struct_el_cols,
    unsigned struct_el_rows,
    bool *struct_el);

/* Internal entrypoint. */
bool binmorphApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    miscOperatorType operator_type,
    miscPredefinedStruct predefined_struct,
    unsigned struct_el_cols,
    unsigned struct_el_rows,
    bool *struct_el) {

    riplGrey *temp;
    bool result;

    RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)
    RIPL_VALIDATE(struct_el_cols>0 && struct_el_rows>0)
    RIPL_VALIDATE((predefined_struct>psNone) ? struct_el==NULL:struct_el!=NULL)

    if (predefined_struct>psNone) {
        struct_el=miscGenerateMorphStruct(predefined_struct,
            struct_el_cols,
            struct_el_rows);
        RIPL_VALIDATE(struct_el)
    }
    /* Threshold image about BINMORPH_THRESHOLD. */
    if (!thresholdApplyOperator(pinputGreyMap,
        poutputGreyMap,
        BINMORPH_THRESHOLD)) return false;
    temp=pinputGreyMap->data;
    pinputGreyMap->data=poutputGreyMap->data;
    poutputGreyMap->data=temp;
    switch (operator_type) {
        case otErosion:
            result=erosion(pinputGreyMap, poutputGreyMap,
                struct_el_cols, struct_el_rows, struct_el);
            break;
        case otDilation:
            result=dilation(pinputGreyMap, poutputGreyMap,
                struct_el_cols, struct_el_rows, struct_el);
            break;
        case otOpening:
            result=opening(pinputGreyMap, poutputGreyMap,
                struct_el_cols, struct_el_rows, struct_el);
            break;
        case otClosing:
            result=closing(pinputGreyMap, poutputGreyMap,
                struct_el_cols, struct_el_rows, struct_el);
            break;
        default:
            result=false;
            break;
    }
    if (predefined_struct>psNone) riplFree(struct_el);
    return result;
}

/* Perform binary erosion. */
static bool erosion(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    unsigned struct_el_cols,
    unsigned struct_el_rows,
    bool *struct_el) {

    riplGrey *outP=poutputGreyMap->data;
    unsigned row, col, i1, j1, i2, j2,
        hnk1=struct_el_cols/2, hnk2=struct_el_cols-hnk1-1,
        hmk1=struct_el_rows/2, hmk2=struct_el_rows-hmk1-1,
        tk, lk, ta, la, ba, ra;
    bool set;

    for (ta=0, ba=hmk2, tk=hmk1, row=0;
        row<pinputGreyMap->rows; row++) {
        for (la=0, ra=hnk2, lk=hnk1, col=0;
            col<pinputGreyMap->cols; col++) {
            for (set=true, i1=ta, i2=tk; i1<=ba && set; i1++, i2++) {
                for (j1=la, j2=lk; j1<=ra && set; j1++, j2++) {
                    if (!struct_el[i2*struct_el_cols+j2]) continue;
                    /* Test pixel. */
                    if (!*(pinputGreyMap->data
                        +i1*pinputGreyMap->cols+j1)) set=false;
                }
            }
            *outP++=(riplGrey)(set ? RIPL_MAX_GREY:0);
            if (col>=hnk1) la++;
            if (ra<pinputGreyMap->cols-hnk2) ra++;
            if (lk>0) lk--;
        }
        if (row>=hmk1) ta++;
        if (ba<pinputGreyMap->rows-hmk2) ba++;
        if (tk>0) tk--;
    }
    return true;
}

/* Perform binary dilation. */
static bool dilation(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    unsigned struct_el_cols,
    unsigned struct_el_rows,
    bool *struct_el) {
    riplGrey *outP=poutputGreyMap->data;
    unsigned row, col, i1, j1, i2, j2,
        hnk1=struct_el_cols/2, hnk2=struct_el_cols-hnk1-1,
        hmk1=struct_el_rows/2, hmk2=struct_el_rows-hmk1-1,
        tk, lk, ta, la, ba, ra;
    bool set;
    for (ta=0, ba=hmk2, tk=hmk1, row=0;
        row<pinputGreyMap->rows; row++) {
        for (la=0, ra=hnk2, lk=hnk1, col=0;
            col<pinputGreyMap->cols; col++) {
            for (set=false, i1=ta, i2=tk; i1<=ba && !set; i1++, i2++) {
                for (j1=la, j2=lk; j1<=ra && !set; j1++, j2++) {
                    if (!struct_el[i2*struct_el_cols+j2]) continue;
                    /* Test pixel. */
                    if (*(pinputGreyMap->data
                        +i1*pinputGreyMap->cols+j1)) set=true;
                }
            }
            *outP++=(riplGrey)(set ? RIPL_MAX_GREY:0);
            if (col>=hnk1) la++;
            if (ra<pinputGreyMap->cols-hnk2) ra++;
            if (lk>0) lk--;
        }
        if (row>=hmk1) ta++;
        if (ba<pinputGreyMap->rows-hmk2) ba++;
        if (tk>0) tk--;
    }
    return true;
}

/* Perform binary opening---erosion followed by dilation. */
static bool opening(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    unsigned struct_el_cols,
    unsigned struct_el_rows,
    bool *struct_el) {
    riplGrey *temp;
    if (!erosion(pinputGreyMap, poutputGreyMap,
        struct_el_cols, struct_el_rows, struct_el)) return false;
    temp=pinputGreyMap->data;
    pinputGreyMap->data=poutputGreyMap->data;
    poutputGreyMap->data=temp;
    return dilation(pinputGreyMap, poutputGreyMap,
        struct_el_cols, struct_el_rows, struct_el);
}

/* Perform binary closing---dilation followed by erosion. */
static bool closing(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    unsigned struct_el_cols,
    unsigned struct_el_rows,
    bool *struct_el) {
    riplGrey *temp;
    if (!dilation(pinputGreyMap, poutputGreyMap,
        struct_el_cols, struct_el_rows, struct_el)) return false;
    temp=pinputGreyMap->data;
    pinputGreyMap->data=poutputGreyMap->data;
    poutputGreyMap->data=temp;
    return erosion(pinputGreyMap, poutputGreyMap,
        struct_el_cols, struct_el_rows, struct_el);
}

/* Command-line version. */
int binmorphExecute(unsigned argc, const char **argv,
    riplGreyMap *pinputGreyMap, riplGreyMap *poutputGreyMap) {
    unsigned struct_el_cols, struct_el_rows, struct_el_no, i;
    miscOperatorType operator_type=otInvalid;
    miscPredefinedStruct predefined_struct=psInvalid;
    bool *struct_el, *ptr;
    bool result;
    if (argc<4) {
        riplMessage(itError, "Syntax error!\n"
            "Usage: " RIPL_EXENAME " " RIPL_CMDLINE
            " binmorph <op> <ncols> <nrows> <el>\n"
            "Or:    " RIPL_EXENAME " " RIPL_CMDLINE
            " binmorph <op> <predef-el> <ncols> <nrows>\n");
        return RIPL_PARAMERROR;
    }
    if (riplMatch(argv[0], "erosion", 3)) operator_type=otErosion;
    else {
        if (riplMatch(argv[0], "dilation", 3)) operator_type=otDilation;
        else {
            if (riplMatch(argv[0], "opening", 3)) operator_type=otOpening;
            else {
                if (riplMatch(argv[0], "closing", 3)) operator_type=otClosing;
            }
        }
    }
    if (operator_type==otInvalid) {
        riplMessage(itError, "binmorph: <op> should be 'erosion', "
            "'dilation', 'opening' or 'closing'.\n");
        return RIPL_PARAMERROR;
    }
    /* Test to see if we have a 'unsigned' value or not. */
    if (!riplArgGet_unsigned(argv[1], &struct_el_cols)) {
        /* If it's not then the user may be specifying the alternative syntax. */
        if (riplMatch(argv[1], "ones", 3)) predefined_struct=psOnes;
        else {
            if (riplMatch(argv[1], "cross", 3)) predefined_struct=psCross;
            else {
                if (riplMatch(argv[1], "kite", 3)) predefined_struct=psKite;
                else {
                    if (riplMatch(argv[1], "diag", 3)) predefined_struct=psDiag;
                }
            }
        }
        if (predefined_struct==psInvalid) {
            /* Command line does not conform to either syntax. */
            riplMessage(itError, "Syntax error!\n"
                "   Usage: " RIPL_EXENAME " " RIPL_CMDLINE
                " binmorph <op> <ncols> <nrows> <el>\n"
                "   Or:    " RIPL_EXENAME " " RIPL_CMDLINE
                " binmorph <op> <predef-el> <ncols> <nrows>\n");
            return RIPL_PARAMERROR;
        }
        /* ALTERNATIVE SYNTAX SPECIFIED. */
        if (!riplArgGet_unsigned(argv[2], &struct_el_cols)) {
            riplMessage(itError, "binmorph: <ncols> should be an integer >= 1.\n");
            return RIPL_PARAMERROR;
        }
        if (struct_el_cols<1) {
            riplMessage(itError, "binmorph: <ncols> should be an integer >= 1.\n");
            return RIPL_PARAMERROR;
        }
        if (!riplArgGet_unsigned(argv[3], &struct_el_rows)) {
            riplMessage(itError, "binmorph: <nrows> should be an integer >= 1.\n");
            return RIPL_PARAMERROR;
        }
        if (struct_el_rows<1) {
            riplMessage(itError, "binmorph: <nrows> should be an integer >= 1.\n");
            return RIPL_PARAMERROR;
        }
        if (!binmorphApplyOperator(pinputGreyMap,
            poutputGreyMap,
            operator_type,
            predefined_struct,
            struct_el_cols,
            struct_el_rows,
            NULL)) return RIPL_EXECUTEERROR;
        return 4;
    }
    /* STANDARD SYNTAX SPECIFIED. */
    if (struct_el_cols<1) {
        riplMessage(itError, "binmorph: <ncols> should be an integer >= 1.\n");
        return RIPL_PARAMERROR;
    }
    if (!riplArgGet_unsigned(argv[2], &struct_el_rows)) {
        riplMessage(itError, "binmorph: <nrows> should be an integer >= 1.\n");
        return RIPL_PARAMERROR;
    }
    if (struct_el_rows<1) {
        riplMessage(itError, "binmorph: <nrows> should be an integer >= 1.\n");
        return RIPL_PARAMERROR;
    }
    struct_el_no=struct_el_cols*struct_el_rows;
    if (argc<struct_el_no+3) {
        riplMessage(itError, "Incorrect number of parameters!\n"
            "Usage: " RIPL_EXENAME " " RIPL_CMDLINE
            " binmorph <op> <ncols> <nrows> <el>\n"
            "Or:    " RIPL_EXENAME " " RIPL_CMDLINE
            " binmorph <op> <predef-el> <ncols> <nrows>\n");
        return RIPL_PARAMERROR;
    }
    ptr=struct_el=(bool *)riplCalloc(struct_el_no, sizeof(bool));
    for (i=0; i<struct_el_no; i++, ptr++) {
        if (!riplArgGet_bool(argv[3+i], ptr)) {
            riplFree(struct_el);
            riplMessage(itError, "binmorph: <el> values should be boolean.\n");
            return RIPL_PARAMERROR;
        }
    }
    result=binmorphApplyOperator(pinputGreyMap,
        poutputGreyMap,
        operator_type,
        psNone,
        struct_el_cols,
        struct_el_rows,
        struct_el);
    riplFree(struct_el);
    if (!result) return RIPL_EXECUTEERROR;
    return struct_el_no+3;
}

/* Return help information. */
const char *binmorphHelp(void) {
    return "binmorph <op> <ncols> <nrows> <el>\n"
        "   Or:    " RIPL_EXENAME " " RIPL_CMDLINE
        " binmorph <op> <predef-el> <ncols> <nrows>\n\n"
        "   Applies the binary morphological operator specified in <op>\n"
        "   to the image using the structuring element <el> with the\n"
        "   specified number of columns and rows, <ncols> and <nrows>. <op>\n"
        "   must be one of 'erosion', 'dilation', 'opening' or 'closing'.\n"
        "   The alternative syntax allows one to specified a predefined\n"
        "   structuring element <predef-el> which must be one of 'ones',\n"
        "   'cross', 'kite' or 'diag'. Foreground is assumed white and\n"
        "   background black.\n\n"
        "   Note that exactly the same result can be achieved by\n"
        "   thresholding the image at a level of 128 (using 'threshold 128'\n"
        "   operator) and then applying the grey-level morphological\n"
        "   operators 'glmorph'. This version of the operators is optimized\n"
        "   for binary images and so is slightly quicker.\n";
}

