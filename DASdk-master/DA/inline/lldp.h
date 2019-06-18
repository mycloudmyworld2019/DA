// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_INLINE_LLDP_H
#define DA_INLINE_LLDP_H

namespace DA {

inline lldp_mgr * lldp_handler::get_lldp_mgr() const {
   return mgr_;
}

}

#endif // DA_INLINE_LLDP_H
