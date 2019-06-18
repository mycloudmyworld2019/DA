// Copyright (c) 2013 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_PANIC_H
#define DA_PANIC_H

#include <DA/base.h>
#include <cstdarg>

/**
 * @file
 * Panic handling framework.
 *
 * A 'panic' is raised by the DA SDK to signal a programming error on
 * behalf of the SDK user. A regular 'assert' is used by the SDK to
 * signal an Arista error which should be reported for correction.
 *
 * Panics occur under a number of conditions, such as an invalid
 * string format for an IP address when using ip_addr_t's constructor
 * to checks of incompatible type attributes during manager calls.
 * The panic exception passed to the exception_handler_t callback
 * describes the fault condition observed.
 *
 * During a panic call, any panic handler set with exception_handler_is
 * is called with the message, and then the panic message is also written
 * to stderr and a SIGABRT raised. The SIGABRT will also cause a stack
 * trace to be dumped.
 */

namespace DA {

class error;  // Forward declaration.

/**
 * Reports an exception from the SDK.
 * Internal errors and programming errors are reported via subclasses of
 * exception.  See DA/exception.h for possible exceptions.
 */
void panic(error const & exception) DA_SDK_NORETURN DA_SDK_PUBLIC;

/// @deprecated Obsolete, replaced with panic(error).
void panic(char const * fmt, ...)
   DA_SDK_NORETURN DA_SDK_FORMAT_STRING_CHECK(1) DA_SDK_PUBLIC;

/// @deprecated Obsolete, replaced with panic(error).
void vpanic(char const * fmt, va_list ap)
   DA_SDK_NORETURN DA_SDK_PUBLIC;

/**
 * The exception handler callback definition.
 *
 * @param exception The exception that needs to be handled.
 */
typedef void (*exception_handler_t)(error const & exception);

/**
 * Sets a custom panic handler.
 *
 * @param exception_handler_t An exception handler callback
 */
void exception_handler_is(exception_handler_t) DA_SDK_PUBLIC;

}

#endif // DA_PANIC_H
