/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		riplpbm.c
 *		Source file for Portable Bitmap (PBM/PGM/PPM) import filter functions.
 *
 *		Version 1.1, last update: 10 February 1998.
 *
 *		History:
 *			10/2/98:		added message if image too big.
 *			20/1/98:		version 1.1.
 *			19/1/98:		modified included files list (added 'riplallc.h').
 *			16/12/97:	amendment to bug fix.
 *			15/12/97:	bug fix (newlines in binary formats).
 *			27/11/97:	last substantial update.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "riplpbm.h"
#include "riplallc.h"
#include "ripldbug.h"

/* Typedef and constants for  local use only. */
#define BUFFER_SIZE				256
#define TOKEN_SIZE				64
struct tagFile {
    FILE *pfile, *pfileHandle;
    riplGrey buffer[BUFFER_SIZE];
    char token[TOKEN_SIZE+1];
    size_t items, pos;
};
enum tagStatus {
    sSuccess=0,
    sError,
    sFileErr,
    sEOF,
    sMem
};

/* Prototypes of static functions. */
static bool find_char(struct tagFile *ppbmFile,
    enum tagStatus *preturnStatus,
    char ch);
static const char *get_token(struct tagFile *ppbmFile,
    enum tagStatus *preturnStatus);
static struct tagFile *open_file(const char *pfileName);
static void close_file(struct tagFile *ppbmFile);
static bool flush_file(struct tagFile *ppbmFile,
    enum tagStatus *preturnStatus);
static const char *read_token(struct tagFile *ppbmFile,
    enum tagStatus *preturnStatus);
static void skip_line(struct tagFile *ppbmFile,
    enum tagStatus *preturnStatus);
static bool skip_space(struct tagFile *ppbmFile,
    enum tagStatus *preturnStatus);
static bool skip_chars(struct tagFile *ppbmFile,
    enum tagStatus *preturnStatus,
    unsigned nchars);

/* Load a PBM-format file and return a new greymap object. */
riplGreyMap *riplPBMLoadFile(const char *pfileName,
    riplGraphicFormat graphicFormat) {

    enum tagStatus return_status=sSuccess;
    struct tagFile *pbm_file;
    unsigned max_intensity, row, col, data_size;
    unsigned i;
    riplGrey value, *buffer, *buf_ptr;
    riplGrey *ptr;
    riplGreyMap *pgrey_map=(riplGreyMap *)riplMalloc(sizeof(riplGreyMap));

    RIPL_VALIDATE(pfileName)
    RIPL_VALIDATE(graphicFormat!=gfInvalid)
    RIPL_VALIDATE(pgrey_map)

    pbm_file=open_file(pfileName);
    /* Read in and discard the P token. */
    if (!read_token(pbm_file, &return_status)) {
        close_file(pbm_file);
        return NULL;
    }
    /* Read in the number of columns and rows. */
    pgrey_map->cols=atoi(get_token(pbm_file, &return_status));
    pgrey_map->rows=atoi(get_token(pbm_file, &return_status));
    if (return_status!=sSuccess) {
        close_file(pbm_file);
        return NULL;
    }
    if (pgrey_map->cols>RIPL_MAX_COLS || pgrey_map->rows>RIPL_MAX_ROWS) {
        close_file(pbm_file);
        riplMessage(itError, "Unable to load image file %s!\n"
            "[Image too big]\n",
            pfileName);
        return NULL;
    }
    pgrey_map->size=pgrey_map->cols*pgrey_map->rows;
    pgrey_map->data=(riplGrey *)riplCalloc(pgrey_map->size, sizeof(riplGrey));
    RIPL_VALIDATE(pgrey_map->data)
    switch (graphicFormat) {
        /* The ASCII-encoded PBM-family formats. */
        case gfPBMASCII:	/* ASCII-encoded portable bitmap. */
            for (ptr=pgrey_map->data, i=0;
                return_status==sSuccess && i<pgrey_map->size; i++)
                *ptr++=(1-atof(get_token(pbm_file, &return_status)))*RIPL_MAX_GREY;
            break;
        case gfPGMASCII:	/* ASCII-encoded portable greymap. */
            max_intensity=atoi(get_token(pbm_file, &return_status));
            ptr=pgrey_map->data;
            if (max_intensity==RIPL_MAX_GREY) {
                for (i=0; return_status==sSuccess && i<pgrey_map->size; i++)
                    *ptr++=(riplGrey)atoi(get_token(pbm_file, &return_status));
            }
            else {
                for (i=0; return_status==sSuccess && i<pgrey_map->size; i++)
                    *ptr++=(riplGrey)
                        (atof(get_token(pbm_file, &return_status))
                        /max_intensity*RIPL_MAX_GREY);
            }
            break;
        case gfPPMASCII:	/* ASCII-encoded portable pixelmap. */
            max_intensity=atoi(get_token(pbm_file, &return_status));
            ptr=pgrey_map->data;
            if (max_intensity==RIPL_MAX_GREY) {
                for (i=0; return_status==sSuccess && i<pgrey_map->size; i++)
                    *ptr++=(riplGrey)riplRound(
                        RIPL_RED_WEIGHT*atoi(get_token(pbm_file, &return_status))
                        +RIPL_GREEN_WEIGHT*atoi(get_token(pbm_file, &return_status))
                        +RIPL_BLUE_WEIGHT*atoi(get_token(pbm_file, &return_status)));
            }
            else {
                for (i=0; return_status==sSuccess && i<pgrey_map->size; i++)
                    *ptr++=(riplGrey)riplRound((
                        RIPL_RED_WEIGHT*atof(get_token(pbm_file, &return_status))
                        +RIPL_GREEN_WEIGHT*atof(get_token(pbm_file, &return_status))
                        +RIPL_BLUE_WEIGHT*atof(get_token(pbm_file, &return_status)))
                        /max_intensity*RIPL_MAX_GREY);
            }
            break;
        /* The binary-encoded PBM-family formats. */
        case gfPBMBinary:	/* Binary-encoded portable bitmap. */
/*			skip_space(pbm_file, &return_status);*/
            skip_chars(pbm_file, &return_status, 1);
            if (return_status!=sSuccess) break;
            flush_file(pbm_file, &return_status);
            if (return_status!=sSuccess) break;

            data_size = pgrey_map->cols >> 3;
            if ((data_size % 8) != 0) {
              ++data_size;
            }

            buffer=(riplGrey *)riplCalloc(data_size, sizeof(riplGrey));
            for (ptr=pgrey_map->data, row=0; row<pgrey_map->rows; row++) {
                if (fread(buffer, sizeof(riplGrey), data_size,
                    pbm_file->pfile)!=data_size) {
                    return_status=sEOF;
                    break;
                }
                for (buf_ptr=buffer, col=0; col<pgrey_map->cols; col++, value<<=1) {
                    if (col%8==0) value=*buf_ptr++;
                    *ptr++=(riplGrey)((1-(value&0x80)>0)*RIPL_MAX_GREY);
                }
            }
            riplFree(buffer);
            break;
        case gfPGMBinary:	/* Binary-encoded portable greymap. */
            max_intensity=atoi(get_token(pbm_file, &return_status));
            if (max_intensity!=RIPL_MAX_GREY) {
                return_status=sError;
                break;
            }
/*			skip_space(pbm_file, &return_status);*/
            skip_chars(pbm_file, &return_status, 1);
            if (return_status!=sSuccess) break;
            flush_file(pbm_file, &return_status);
            if (return_status!=sSuccess) break;
            if (fread(pgrey_map->data, sizeof(riplGrey), pgrey_map->size,
                pbm_file->pfile)!=pgrey_map->size) return_status=sEOF;
            break;
        case gfPPMBinary:
            max_intensity=atoi(get_token(pbm_file, &return_status));
            if (max_intensity!=RIPL_MAX_GREY) {
                return_status=sError;
                break;
            }
/*			skip_space(pbm_file, &return_status);*/
            skip_chars(pbm_file, &return_status, 1);
            if (return_status!=sSuccess) break;
            flush_file(pbm_file, &return_status);
            if (return_status!=sSuccess) break;
            data_size=pgrey_map->cols*3;
            buffer=(riplGrey *)riplCalloc(data_size, sizeof(riplGrey));
            for (ptr=pgrey_map->data, row=0; row<pgrey_map->rows; row++) {
                if (fread(buffer, sizeof(riplGrey), data_size,
                    pbm_file->pfile)!=data_size) {
                    return_status=sEOF;
                    break;
                }
                buf_ptr=buffer;
                for (col=0; col<pgrey_map->cols; col++, buf_ptr+=3) {
                    *ptr++=(riplGrey)riplRound(
                        RIPL_RED_WEIGHT*buf_ptr[0]
                        +RIPL_GREEN_WEIGHT*buf_ptr[1]
                        +RIPL_BLUE_WEIGHT*buf_ptr[2]);
                }
            }
            riplFree(buffer);
            break;

    }
    close_file(pbm_file);
    return pgrey_map;
}

/* Save specified greymap under specified filename and subformat. */
bool riplPBMSaveFile(const char *pfileName,
    riplGraphicFormat graphicFormat,
    const riplGreyMap *pgrey_map) {
    FILE *pfile;
    if (graphicFormat!=gfPGMBinary) return sError;
    pfile=fopen(pfileName, "wb");
    if (!pfile)
    {
        return false;
    }

    fprintf(pfile, "P5\n"
        "# %u-Level Portable Greymap.\n"
        "# PGM Export Filter by Richard Cook " __DATE__ "\n"
        "%u %u\n%u\n",
        RIPL_GREY_LEVELS,
        pgrey_map->cols, pgrey_map->rows,
        RIPL_MAX_GREY);
    fwrite(pgrey_map->data, sizeof(riplGrey), pgrey_map->size, pfile);
    fclose(pfile);
    return true;
}

/* Support functions. */
static struct tagFile *open_file(const char *pfileName) {
    struct tagFile *pbm_file=(struct tagFile *)riplMalloc(sizeof(struct tagFile));
    pbm_file->pfile=fopen(pfileName, "rb");
    pbm_file->pfileHandle=NULL;
    pbm_file->items=pbm_file->pos=0;
    return pbm_file;
}

static void close_file(struct tagFile *ppbmFile) {
    if (ppbmFile->pfile != NULL) {
        fclose(ppbmFile->pfile);
    }

    if (ppbmFile->pfileHandle != NULL) {
        fclose(ppbmFile->pfileHandle);
    }

    riplFree(ppbmFile);
}

#ifdef __BORLANDC__
#	pragma argsused
#endif
static bool flush_file(struct tagFile *ppbmFile,
    enum tagStatus *preturnStatus) {
    bool result=true;
    if (fseek(ppbmFile->pfileHandle,
        (long)ppbmFile->pos-ppbmFile->items,
        SEEK_CUR)) {
        result=false;
    }
    ppbmFile->pfile=ppbmFile->pfileHandle;
    ppbmFile->pfileHandle=NULL;
    ppbmFile->items=ppbmFile->pos=0;
    return result;
}

static const char *read_token(struct tagFile *ppbmFile,
    enum tagStatus *preturnStatus) {
    size_t start_pos;
    unsigned token_len;
    char *ptr;

    if (ppbmFile->pfile) {
        ppbmFile->pfileHandle=ppbmFile->pfile;
        ppbmFile->pfile=NULL;
        ppbmFile->items=fread(ppbmFile->buffer,
            sizeof(char), BUFFER_SIZE, ppbmFile->pfileHandle);
        if (ferror(ppbmFile->pfileHandle)) {
            *preturnStatus=sFileErr;
            return NULL;
        }
        ppbmFile->pos=0;
    }
    else {
        if (!ppbmFile->pfileHandle) {
            *preturnStatus=sError;
            return NULL;
        }
    }

    /* Find beginning of token. */
    if (!skip_space(ppbmFile, preturnStatus)) return NULL;
    start_pos=ppbmFile->pos;
    token_len=0;

    /* Find end of token. */
    do {
        while (ppbmFile->pos<ppbmFile->items
            && !isspace(ppbmFile->buffer[ppbmFile->pos])) ppbmFile->pos++;
        if (ppbmFile->pos==ppbmFile->items) {
            if (feof(ppbmFile->pfileHandle)) {
                *preturnStatus=sEOF;
                return NULL;
            }
            ptr=ppbmFile->token+token_len;
            token_len+=ppbmFile->pos-start_pos;
            if (token_len>TOKEN_SIZE) {
                *preturnStatus=sError;
                return NULL;
            }
            while (start_pos<ppbmFile->pos) {
                *ptr++=(char)ppbmFile->buffer[start_pos++];
            }
            ppbmFile->items=fread(ppbmFile->buffer,
                sizeof(char), BUFFER_SIZE, ppbmFile->pfileHandle);
            if (ferror(ppbmFile->pfileHandle)) {
                *preturnStatus=sFileErr;
                return NULL;
            }
            start_pos=ppbmFile->pos=0;
        }
        else {
            ptr=ppbmFile->token+token_len;
            token_len+=ppbmFile->pos-start_pos;
            if (token_len>TOKEN_SIZE) {
                *preturnStatus=sError;
                return NULL;
            }
            while (start_pos<ppbmFile->pos) {
                *ptr++=(char)ppbmFile->buffer[start_pos++];
            }
            break;
        }
    } while (!isspace(ppbmFile->buffer[ppbmFile->pos]));
    ppbmFile->token[token_len]='\0';
    return ppbmFile->token;
}

static void skip_line(struct tagFile *ppbmFile,
    enum tagStatus *preturnStatus) {

    if (!find_char(ppbmFile, preturnStatus, '\n')) return;
    skip_space(ppbmFile, preturnStatus);
}

static bool skip_space(struct tagFile *ppbmFile,
    enum tagStatus *preturnStatus) {

    do {
        while (ppbmFile->pos<ppbmFile->items
            && isspace(ppbmFile->buffer[ppbmFile->pos])) ppbmFile->pos++;
        if (ppbmFile->pos==ppbmFile->items) {
            if (feof(ppbmFile->pfileHandle)) {
                *preturnStatus=sEOF;
                return false;
            }
            ppbmFile->items=fread(ppbmFile->buffer,
                sizeof(char), BUFFER_SIZE, ppbmFile->pfileHandle);
            if (ferror(ppbmFile->pfileHandle)) {
                *preturnStatus=sFileErr;
                return false;
            }
            ppbmFile->pos=0;
        }
    } while (isspace(ppbmFile->buffer[ppbmFile->pos]));
    return true;
}

/* New function skip_chars (15/12/97)---skips specified number of chars. */
static bool skip_chars(struct tagFile *ppbmFile,
    enum tagStatus *preturnStatus,
    unsigned nchars) {

    unsigned skippable;

    do {
        skippable=ppbmFile->items-ppbmFile->pos;
        if (skippable>nchars) {
            ppbmFile->pos+=nchars;
            break;
        }
        nchars-=skippable;
        if (feof(ppbmFile->pfileHandle)) {
            *preturnStatus=sEOF;
            return false;
        }
        ppbmFile->items=fread(ppbmFile->buffer,
            sizeof(char), BUFFER_SIZE, ppbmFile->pfileHandle);
        if (ferror(ppbmFile->pfileHandle)) {
            *preturnStatus=sFileErr;
            return false;
        }
        ppbmFile->pos=0;
    } while (nchars);
    return true;
}

static bool find_char(struct tagFile *ppbmFile,
    enum tagStatus *preturnStatus,
    char ch) {

    do {
        while (ppbmFile->pos<ppbmFile->items
            && ppbmFile->buffer[ppbmFile->pos]!=ch) ppbmFile->pos++;
        if (ppbmFile->pos==ppbmFile->items) {
            if (feof(ppbmFile->pfileHandle)) {
                *preturnStatus=sEOF;
                return false;
            }
            ppbmFile->items=fread(ppbmFile->buffer,
                sizeof(char), BUFFER_SIZE, ppbmFile->pfileHandle);
            if (ferror(ppbmFile->pfileHandle)) {
                *preturnStatus=sFileErr;
                return false;
            }
            ppbmFile->pos=0;
        }
    } while (ppbmFile->buffer[ppbmFile->pos]!=ch);
    return true;
}

static const char *get_token(struct tagFile *ppbmFile,
    enum tagStatus *preturnStatus) {

    const char *token=read_token(ppbmFile, preturnStatus);
    while (token[0]=='#') {
        skip_line(ppbmFile, preturnStatus);
        token=read_token(ppbmFile, preturnStatus);
    }
    return token;
}

