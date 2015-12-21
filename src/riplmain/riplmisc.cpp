#include "riplmisc.h"

#include "Error.h"
#include "ripldbug.h"
#include "validate.h"
#include <fstream>

using namespace std;
using namespace ripl;

vector<string> riplParseResponseFile(const string& fileName)
{
    ifstream stream(fileName, ios::in);
    RIPL_REQUIRE(stream, error::IOError);

    vector<string> args;
    while (!stream.eof())
    {
        string arg;
        stream >> arg;
        args.emplace_back(arg);
    }

    return args;
}

/* Return 'true' if specified file exists. */
bool riplFileExists(const char *pfileName) {

    FILE *pfile;

    RIPL_VALIDATE(pfileName)

    pfile=fopen(pfileName, "r");
    if (pfile) {
        fclose(pfile);
        return true;
    }
    return false;
}

/* Round double value to nearest integer. */
int riplRound(double val) {

    int ival=val+1;

    if (val-ival+1<ival-val) return ival-1;
    return ival;
}

#ifndef HAVE_STRLWR
/* Convert specified zero-terminated string to lower-case. */
char *strlwr(char *s) {

    char *ptr=s;

    while (*ptr) {
        if (isupper(*ptr)) *ptr=(char)tolower(*ptr);
        ptr++;
    }
    return s;
}
#endif

#ifndef HAVE_STRUPR
/* Convert specified zero-terminated string to upper-case. */
char *strupr(char *s) {

    char *ptr=s;

    while (*ptr) {
        if (islower(*ptr)) *ptr=(char)toupper(*ptr);
        ptr++;
    }
    return s;
}
#endif

#ifndef HAVE_STRDUP
/* Duplicate specified string. */
char *strdup(const char *s) {

    char *new_s=(char *)riplCalloc(strlen(s)+1, sizeof(char));
    
    return new_s ? strcpy(new_s, s):NULL;
}
#endif

