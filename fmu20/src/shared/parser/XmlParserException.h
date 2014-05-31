/*
 * Copyright QTronic GmbH. All rights reserved.
 */

/* ---------------------------------------------------------------------------*
 * XmlParserException.h
 * Exception used in parsing model description of a FMI 2.0 model.
 *
 * Author: Adrian Tirea
 * ---------------------------------------------------------------------------*/

#ifndef XML_PARSER_EXCEPTION_H
#define XML_PARSER_EXCEPTION_H

#include "exception"

static char* strallocprintf(const char *format, va_list argp);

// message passed in constructor is freed in destructor.
class XmlParserException : public std::exception {
public:
    char *message;
public:
    XmlParserException(const char *format, ...) {
    va_list argp;
    va_start(argp, format);
    message = strallocprintf(format, argp);
    va_end(argp);
    }
    ~XmlParserException() {
        if (message) free(message);
    }

    virtual const char *what() const throw() {
        return message;
    }
};

static char *strallocprintf(const char *format, va_list argp) {
    int n;
    char *result;
    n = _vscprintf(format, argp);
    result = (char *)malloc((n + 1) * sizeof(char));
    vsprintf(result, format, argp);
    return result;
}

#endif // XML_PARSER_EXCEPTION_H