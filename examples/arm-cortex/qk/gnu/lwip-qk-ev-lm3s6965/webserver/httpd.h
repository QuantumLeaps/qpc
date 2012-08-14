/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 */

#ifndef __HTTPD_H__
#define __HTTPD_H__

#ifdef __cplusplus
extern "C" {
#endif

void httpd_init(void);

#ifdef INCLUDE_HTTPD_CGI

/*
 * Function pointer for a CGI script handler.
 *
 * This function is called each time the HTTPD server is asked for a file
 * whose name was previously registered as a CGI function using a call to
 * http_set_cgi_handlers(). The iIndex parameter provides the index of the
 * CGI within the ppcURLs array passed to http_set_cgi_handler. Parameters
 * pcParam and pcValue provide access to the parameters provided along with
 * the URI. iNumParams provides a count of the entries in the pcParam and
 * pcValue arrays. Each entry in the pcParam array contains the name of a
 * parameter with the corresponding entry in the pcValue array containing the
 * value for that parameter. Note that pcParam may contain multiple elements
 * with the same name if, for example, a multi-selection list control is used
 * in the form generating the data.
 *
 * The function should return a pointer to a character string which is the
 * path and filename of the response that is to be sent to the connected
 * browser, for example "/thanks.htm" or "/response/error.ssi".
 *
 * The maximum number of parameters that will be passed to this function via
 * iNumParams is defined by MAX_CGI_PARAMS. Any parameters in the incoming
 * HTTP request above this number will be discarded.
 *
 * Requests intended for use by this CGI mechanism must be sent using the GET
 * method (which encodes all parameters within the URI rather than in a block
 * later in the request). Attempts to use the POST method will result in the
 * request being ignored.
 *
 */
typedef char const *(*tCGIHandler)(int iIndex, int iNumParams,
                                   char const *pcParam[],
                                   char const *pcValue[]);

/*
 * Structure defining the base filename (URL) of a CGI and the associated
 * function which is to be called when that URL is requested.
 */
typedef struct {
    char const *pcCGIName;
    tCGIHandler pfnCGIHandler;
} tCGI;

void http_set_cgi_handlers(const tCGI *pCGIs, int iNumHandlers);


/* The maximum number of parameters that the CGI handler can be sent. */
#ifndef MAX_CGI_PARAMS
#define MAX_CGI_PARAMS 16
#endif

#endif

#ifdef INCLUDE_HTTPD_SSI

/*
 * Function pointer for the SSI tag handler callback.
 *
 * This function will be called each time the HTTPD server detects a tag of
 * the form <!--#name--> in a .shtml, .ssi or .shtm file where "name" appears
 * as one of the tags supplied to http_set_ssi_handler in the ppcTags array.
 * The returned string, which will replace the whole string "<!--#name-->"
 * in file sent back to the client, should be written to pointer pcInsert.
 * iInsertLen contains the size of the buffer pointed to by pcInsert. The
 * iIndex parameter provides the zero-based index of the tag as found in the
 * ppcTags array and identifies the tag that is to be replaced.
 *
 * The handler returns the number of characters written to pcInsert excluding
 * any terminating NULL or a negative number to indicate a failure (tag not
 * recognized, for example).
 *
 */
typedef int (*tSSIHandler)(int iIndex, char *pcInsert, int iInsertLen);

void http_set_ssi_handler(tSSIHandler pfnSSIHandler,
                          char const * const ppcTags[], int iNumTags);

/* The maximum length of the string comprising the tag name */
#ifndef MAX_TAG_NAME_LEN
#define MAX_TAG_NAME_LEN 8
#endif

/* The maximum string length that can be returned to replace any given tag */
#ifndef MAX_TAG_INSERT_LEN
#define MAX_TAG_INSERT_LEN 192
#endif

#endif

#ifdef __cplusplus
}
#endif

#endif /* __HTTPD_H__ */
