// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_INLINE_ARESOLVE_H
#define DA_INLINE_ARESOLVE_H

namespace DA {

inline aresolve_mgr * aresolve_handler::get_aresolve_mgr() const {
   return mgr_;
}

}

#endif // DA_INLINE_ARESOLVE_H
