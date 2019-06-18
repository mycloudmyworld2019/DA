// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#include "DA/mlag.h"
#include "impl.h"

namespace DA {

//
// mlag_mgr implementation
//

class mlag_mgr_impl : public mlag_mgr {
 public:
   mlag_mgr_impl() {
   }

   bool enabled() const {
      return false;
   }

   void enabled_is(bool enabled) {
   }

   ip_addr_t peer_address() const {
      return ip_addr_t();
   }

   mlag_state_t state() const {
      return MLAG_STATE_DISABLED;
   }

};

DEFINE_STUB_MGR_CTOR(mlag_mgr)

mlag_handler::mlag_handler(mlag_mgr *mgr) : 
      base_handler<mlag_mgr, mlag_handler>(mgr) {
}

void mlag_handler::on_mlag_state(mlag_state_t state) {
}

} // end DA namespace
