/* ------------------------------------------------------------------------- 
 * sim_support.h
 * Functions used by the FMU simulations fmusim_me and fmusim_cs.
 * Copyright QTronic GmbH. All rights reserved.
 * -------------------------------------------------------------------------*/

// Used 7z options, version 4.57:
// -x   Extracts files from an archive with their full paths in the current dir, or in an output dir if specified
// -aoa Overwrite All existing files without prompt
// -o   Specifies a destination directory where files are to be extracted
#define UNZIP_CMD "7z x -aoa -o"
#define XML_FILE  "modelDescription.xml"
#define RESULT_FILE "result.csv"
#define BUFSIZE 4096
#ifdef _WIN64
#define DLL_DIR   "binaries\\win64\\"
#else
#define DLL_DIR   "binaries\\win32\\"
#endif
#define RESOURCES_DIR "resources\\"

// return codes of the 7z command line tool
#define SEVEN_ZIP_NO_ERROR 0 // success
#define SEVEN_ZIP_WARNING 1  // e.g., one or more files were locked during zip
#define SEVEN_ZIP_ERROR 2
#define SEVEN_ZIP_COMMAND_LINE_ERROR 7
#define SEVEN_ZIP_OUT_OF_MEMORY 8
#define SEVEN_ZIP_STOPPED_BY_USER 255

void fmuLogger(fmi2Component c, fmi2String instanceName, fmi2Status status, fmi2String category, fmi2String message, ...);
int unzip(const char *zipPath, const char *outPath);
void parseArguments(int argc, char *argv[], const char **fmuFileName, double *tEnd, double *h,
                    int *loggingOn, char *csv_separator, int *nCategories, char **logCategories[]);
void loadFMU(const char *fmuFileName);
void deleteUnzippedFiles();
void outputRow(FMU *fmu, fmi2Component c, double time, FILE* file, char separator, fmi2Boolean header);
int error(const char *message);
void printHelp(const char *fmusim);
char *getTempResourcesLocation(); // caller has to free the result
