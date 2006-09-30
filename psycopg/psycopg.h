/* psycopg.h - definitions for the psycopg python module 
 *
 * Copyright (C) 2003 Federico Di Gregorio <fog@debian.org>
 *
 * This file is part of psycopg.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef PSYCOPG_H
#define PSYCOPG_H 1

#include <Python.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Python 2.5 ssize_t compatibility */
#if PY_VERSION_HEX < 0x02050000 && !defined(PY_SSIZE_T_MIN)
typedef int Py_ssize_t;
#define PY_SSIZE_T_MAX INT_MAX
#define PY_SSIZE_T_MIN INT_MIN
#endif

/* DBAPI compliance parameters */
#define APILEVEL "2.0"
#define THREADSAFETY 2
#define PARAMSTYLE "pyformat"
    
/* C API functions */
#define psyco_errors_fill_NUM 0
#define psyco_errors_fill_RETURN void
#define psyco_errors_fill_PROTO (PyObject *dict)   
#define psyco_errors_set_NUM 1
#define psyco_errors_set_RETURN void
#define psyco_errors_set_PROTO (PyObject *type)
    
/* Total number of C API pointers */
#define PSYCOPG_API_pointers 2
    
#ifdef PSYCOPG_MODULE
    /** This section is used when compiling psycopgmodule.c & co. **/
extern psyco_errors_fill_RETURN psyco_errors_fill psyco_errors_fill_PROTO;
extern psyco_errors_set_RETURN psyco_errors_set psyco_errors_set_PROTO;

/* global excpetions */
extern PyObject *Error, *Warning, *InterfaceError, *DatabaseError,
    *InternalError, *OperationalError, *ProgrammingError,
    *IntegrityError, *DataError, *NotSupportedError;

/* python versions and compatibility stuff */
#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif
    
#else
    /** This section is used in modules that use psycopg's C API **/

static void **PSYCOPG_API;

#define psyco_errors_fill \
 (*(psyco_errors_fill_RETURN (*)psyco_errors_fill_PROTO) \
  PSYCOPG_API[psyco_errors_fill_NUM])
#define psyco_errors_set \
 (*(psyco_errors_set_RETURN (*)psyco_errors_set_PROTO) \
  PSYCOPG_API[psyco_errors_set_NUM])
    
/* Return -1 and set exception on error, 0 on success. */
static int
import_psycopg(void)
{
    PyObject *module = PyImport_ImportModule("psycopg");

    if (module != NULL) {
        PyObject *c_api_object = PyObject_GetAttrString(module, "_C_API");
        if (c_api_object == NULL) return -1;
        if (PyCObject_Check(c_api_object))
            PSYCOPG_API = (void **)PyCObject_AsVoidPtr(c_api_object);
        Py_DECREF(c_api_object);
    }
    return 0;
}

#endif

/* postgresql<->python encoding map */
extern PyObject *psycoEncodings;
    
typedef struct {
    char *pgenc;
    char *pyenc;
} encodingPair;

/* the Decimal type, used by the DECIMAL typecaster */    
extern PyObject *decimalType;

/* some utility functions */
extern void psyco_set_error(PyObject *exc, PyObject *curs,  char *msg, 
                             char *pgerror, char *pgcode);
                                  
/* Exceptions docstrings */
#define Error_doc \
"Base class for error exceptions."

#define Warning_doc \
"A database warning."

#define InterfaceError_doc \
"Error related to the database interface."

#define DatabaseError_doc \
"Error related to the database engine."

#define InternalError_doc \
"The database encountered an internal error."

#define OperationalError_doc \
"Error related to database operation (disconnect, memory allocation etc)."

#define ProgrammingError_doc \
"Error related to database programming (SQL error, table not found etc)."

#define IntegrityError_doc \
"Error related to database integrity."

#define DataError_doc \
"Error related to problems with the processed data."

#define NotSupportedError_doc \
"A not supported datbase API was called."

#ifdef __cplusplus
}
#endif

#endif /* !defined(PSYCOPG_H) */
