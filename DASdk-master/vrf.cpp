// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#include <DA/vrf.h>
#include <impl.h>

namespace DA {

/// The manager for Vrf
class vrf_mgr_impl : public vrf_mgr {
 public:
   ~vrf_mgr_impl() {
   }

   vrf_iter_t vrf_iter() const {
      vrf_iter_t * nop = 0;
      return *nop;
   }

   bool exists(std::string const & vrf_name) const {
      return false;
   }

   vrf_state_t state(std::string const & vrf_name) const {
      return VRF_NULL;
   }

   uint64_t rd(std::string const & vrf_name) const {
      return 0;
   }

   vrf_t vrf(std::string const & vrf_name) const {
      return vrf_t();
   }

   int socket_at(int domain, int type, int protocol, std::string const & vrf_name) {
      return 0;
   }

};

DEFINE_STUB_MGR_CTOR(vrf_mgr)

vrf_handler::vrf_handler(vrf_mgr *mgr) : base_handler(mgr) {
}

void vrf_handler::watch_all_vrfs(bool) {}

void vrf_handler::watch_vrf(std::string const & vrf_name, bool) {}

void vrf_handler::on_vrf_state(std::string vrf_name, vrf_state_t vrf_state) {
}

} // end namespace DA
