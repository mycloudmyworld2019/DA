/**
 * @file
 * The version module. It provides DASdk version information.
 *
 * This module provides access to DASdk version information, including
 * version major number, version minor number and version patch number.
 *
 * To access the version information, application will need to include
 * "DA/version.h", then it can access it directly, as following:
 *   DA::version_major
 *   DA::version_minor
 *   DA::version_patch
 *   DA::version
 *
 *   Note: DA::version is of type "char *" while version_major, version_minor
 *   and version_patch are of type "uint8_t".
 */

// Copyright (c) 2013 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DASDK_VERSION_H
#define DASDK_VERSION_H

#include <DA/base.h>

namespace DA {

// Version string, at least of the form "X.Y.Z", but possibly followed by a
// dash and more characters (e.g. "X.Y.Z-rc1").
extern const char * const version DA_SDK_PUBLIC;

// Major version number (i.e. the "X" in "X.Y.Z")
extern const uint8_t version_major DA_SDK_PUBLIC;

// Minor version number (i.e. the "Y" in "X.Y.Z")
extern const uint8_t version_minor DA_SDK_PUBLIC;

// Patch version number (i.e. the "Z" in "X.Y.Z", regardless of any trailing
// characters followed by the dash optional)
extern const uint8_t version_patch DA_SDK_PUBLIC;

// Arista project name used to build the DA SDK.
extern const char * const build_project DA_SDK_PUBLIC;

// User that built the DA SDK.
extern const char * const build_user DA_SDK_PUBLIC;

// Path under which the DA SDK was built.
extern const char * const build_path DA_SDK_PUBLIC;

// Source control system revision number at which the DA SDK was built.
extern const uint32_t build_cl DA_SDK_PUBLIC;

// UNIX timestamp at which the DA SDK was built.
extern const uint64_t build_timestamp DA_SDK_PUBLIC;

}

#endif // DASDK_VERSION_H
