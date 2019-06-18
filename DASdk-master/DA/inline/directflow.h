// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_INLINE_DIRECTFLOW_H
#define DA_INLINE_DIRECTFLOW_H

#include <DA/panic.h>
#include <DA/exception.h>

namespace DA {

inline directflow_mgr * flow_handler::get_directflow_mgr() const {
   return mgr_;
}

}

#endif // DA_INLINE_DIRECTFLOW_H
