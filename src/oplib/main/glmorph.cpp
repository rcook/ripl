#include "glmorph.h"

#include "alloc.h"
#include "thresh.h"

/* Prototypes of static functions. */
static bool	erosion(riplGreyMap *pinputGreyMap,
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
bool glmorphApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    miscOperatorType operator_type,
    miscPredefinedStruct predefined_struct,
    unsigned struct_el_cols,
    unsigned struct_el_rows,
    bool *struct_el) {

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

/* Perform grey-level erosion. */
static bool	erosion(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    unsigned struct_el_cols,
    unsigned struct_el_rows,
    bool *struct_el) {

    riplGrey *outP=poutputGreyMap->data();

    unsigned row, col, i1, j1, i2, j2,
        hnk1=struct_el_cols/2, hnk2=struct_el_cols-hnk1-1,
        hmk1=struct_el_rows/2, hmk2=struct_el_rows-hmk1-1,
        tk, lk, ta, la, ba, ra;
    riplGrey min, value;
    for (ta=0, ba=hmk2, tk=hmk1, row=0;
        row<pinputGreyMap->height(); row++) {
        for (la=0, ra=hnk2, lk=hnk1, col=0;
            col<pinputGreyMap->width(); col++) {
            for (min=RIPL_MAX_GREY, i1=ta, i2=tk; i1<=ba; i1++, i2++) {
                for (j1=la, j2=lk; j1<=ra; j1++, j2++) {
                    if (!struct_el[i2*struct_el_cols+j2]) continue;
                    value=*(pinputGreyMap->data()
                        +i1*pinputGreyMap->width()+j1);
                    if (value<min) min=value;
                }
            }
            *outP++=min;
            if (col>=hnk1) la++;
            if (ra<pinputGreyMap->width()-hnk2) ra++;
            if (lk>0) lk--;
        }
        if (row>=hmk1) ta++;
        if (ba<pinputGreyMap->height()-hmk2) ba++;
        if (tk>0) tk--;
    }
    return true;
}

/* Perform grey-level dilation. */
static bool dilation(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    unsigned struct_el_cols,
    unsigned struct_el_rows,
    bool *struct_el) {

    riplGrey *outP=poutputGreyMap->data();
    
    unsigned row, col, i1, j1, i2, j2,
        hnk1=struct_el_cols/2, hnk2=struct_el_cols-hnk1-1,
        hmk1=struct_el_rows/2, hmk2=struct_el_rows-hmk1-1,
        tk, lk, ta, la, ba, ra;
    riplGrey max, value;
    for (ta=0, ba=hmk2, tk=hmk1, row=0;
        row<pinputGreyMap->height(); row++) {
        for (la=0, ra=hnk2, lk=hnk1, col=0;
            col<pinputGreyMap->width(); col++) {
            for (max=0, i1=ta, i2=tk; i1<=ba; i1++, i2++) {
                for (j1=la, j2=lk; j1<=ra; j1++, j2++) {
                    if (!struct_el[i2*struct_el_cols+j2]) continue;
                    value=*(pinputGreyMap->data()
                        +i1*pinputGreyMap->width()+j1);
                    if (value>max) max=value;
                }
            }
            *outP++=max;
            if (col>=hnk1) la++;
            if (ra<pinputGreyMap->width()-hnk2) ra++;
            if (lk>0) lk--;
        }
        if (row>=hmk1) ta++;
        if (ba<pinputGreyMap->height()-hmk2) ba++;
        if (tk>0) tk--;
    }
    return true;
}

/* Perform grey-level opening---erosion followed by dilation. */
static bool opening(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    unsigned struct_el_cols,
    unsigned struct_el_rows,
    bool *struct_el)
{
    if (!erosion(pinputGreyMap, poutputGreyMap, struct_el_cols, struct_el_rows, struct_el))
    {
        return false;
    }

    pinputGreyMap->swap(*poutputGreyMap);

    return dilation(pinputGreyMap, poutputGreyMap, struct_el_cols, struct_el_rows, struct_el);
}

/* Perform grey-level closing---dilation followed by erosion. */
static bool closing(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    unsigned struct_el_cols,
    unsigned struct_el_rows,
    bool *struct_el)
{
    if (!dilation(pinputGreyMap, poutputGreyMap, struct_el_cols, struct_el_rows, struct_el))
    {
        return false;
    }

    pinputGreyMap->swap(*poutputGreyMap);

    return erosion(pinputGreyMap, poutputGreyMap, struct_el_cols, struct_el_rows, struct_el);
}

/* Command-line version. */
int glmorphExecute(unsigned argc, const char **argv,
    riplGreyMap *pinputGreyMap, riplGreyMap *poutputGreyMap) {
    unsigned struct_el_cols, struct_el_rows, struct_el_no, i;
    miscOperatorType operator_type=otInvalid;
    miscPredefinedStruct predefined_struct=psInvalid;
    bool *struct_el, *ptr;
    bool result;
    if (argc<4) {
        riplMessage(itError, "Syntax error!\n"
            "Usage: " RIPL_EXENAME " " RIPL_CMDLINE
            " glmorph <op> <ncols> <nrows> <el>\n"
            "Or:    " RIPL_EXENAME " " RIPL_CMDLINE
            " glmorph <op> <predef-el> <ncols> <nrows>\n");
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
        riplMessage(itError, "glmorph: <op> should be 'erosion', 'dilation', "
            "'opening' or 'closing'.\n");
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
                "Usage: " RIPL_EXENAME " " RIPL_CMDLINE
                " glmorph <op> <ncols> <nrows> <el>\n"
                "Or:    " RIPL_EXENAME " " RIPL_CMDLINE
                " glmorph <op> <predef-el> <ncols> <nrows>\n");
            return RIPL_PARAMERROR;
        }
        /* ALTERNATIVE SYNTAX SPECIFIED. */
        if (!riplArgGet_unsigned(argv[2], &struct_el_cols)) {
            riplMessage(itError, "glmorph: <ncols> should be an integer >= 1.\n");
            return RIPL_PARAMERROR;
        }
        if (struct_el_cols<1) {
            riplMessage(itError, "glmorph: <ncols> should be an integer >= 1.\n");
            return RIPL_PARAMERROR;
        }
        if (!riplArgGet_unsigned(argv[3], &struct_el_rows)) {
            riplMessage(itError, "glmorph: <nrows> should be an integer >= 1.\n");
            return RIPL_PARAMERROR;
        }
        if (struct_el_rows<1) {
            riplMessage(itError, "glmorph: <nrows> should be an integer >= 1.\n");
            return RIPL_PARAMERROR;
        }
        if (!glmorphApplyOperator(pinputGreyMap,
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
        riplMessage(itError, "glmorph: <ncols> should be an integer >= 1.\n");
        return RIPL_PARAMERROR;
    }
    if (!riplArgGet_unsigned(argv[2], &struct_el_rows)) {
        riplMessage(itError, "glmorph: <nrows> should be an integer >= 1.\n");
        return RIPL_PARAMERROR;
    }
    if (struct_el_rows<1) {
        riplMessage(itError, "glmorph: <nrows> should be an integer >= 1.\n");
        return RIPL_PARAMERROR;
    }
    struct_el_no=struct_el_cols*struct_el_rows;
    if (argc<struct_el_no+3) {
        riplMessage(itError, "Incorrect number of parameters!\n"
            "Usage: " RIPL_EXENAME " " RIPL_CMDLINE
            " glmorph <op> <ncols> <nrows> <el>\n"
            "Or:    " RIPL_EXENAME " " RIPL_CMDLINE
            " glmorph <op> <predef-el> <ncols> <nrows>\n");
        return RIPL_PARAMERROR;
    }
    ptr=struct_el=(bool *)riplCalloc(struct_el_no, sizeof(bool));
    for (i=0; i<struct_el_no; i++, ptr++) {
        if (!riplArgGet_bool(argv[3+i], ptr)) {
            riplFree(struct_el);
            riplMessage(itError, "glmorph: <el> values should be boolean.\n");
            return RIPL_PARAMERROR;
        }
    }
    result=glmorphApplyOperator(pinputGreyMap,
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
const char *glmorphHelp(void) {
    return "glmorph <op> <ncols> <nrows> <el>\n"
        "   Or:    " RIPL_EXENAME " " RIPL_CMDLINE
        " glmorph <op> <predef-el> <ncols> <nrows>\n\n"
        "   Applies the grey-level morphological operator specified in <op>\n"
        "   to the image using the structuring element <el> with the\n"
        "   specified number of columns and rows, <ncols> and <nrows>. <op>\n"
        "   must be one of 'erosion', 'dilation', 'opening' or 'closing'.\n"
        "   The alternative syntax allows one to specified a predefined\n"
        "   structuring element <predef-el> which must be one of 'ones',\n"
        "   'cross', 'kite' or 'diag'. Foreground is light and background\n"
        "   dark.\n";
}

