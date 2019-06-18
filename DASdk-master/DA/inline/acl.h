// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_INLINE_ACL_H
#define DA_INLINE_ACL_H

namespace DA {


inline acl_mgr * acl_handler::get_acl_mgr() const {
   return mgr_;
}

}  // end namespace DA

#endif // DA_INLINE_ACL_H
