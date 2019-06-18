// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_INLINE_IP_INTF_H
#define DA_INLINE_IP_INTF_H

namespace DA {

inline ip_intf_mgr * ip_intf_handler::get_ip_intf_mgr() const {
   return mgr_;
}

}

#endif // DA_INLINE_IP_INTF_H
