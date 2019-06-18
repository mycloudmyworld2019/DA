// Copyright (c) 2015 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef INLINE_VRF_H
#define INLINE_VRF_H

namespace DA {

inline vrf_mgr *vrf_handler::get_vrf_mgr() const {
   return mgr_;
}

}  // end namespace DA

#endif // INLINE_VRF_H
