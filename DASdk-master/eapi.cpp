// Copyright (c) 2017 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#include "DA/eapi.h"
#include "impl.h"

namespace DA {

class eapi_mgr_impl : public eapi_mgr {
 public:
   eapi_mgr_impl() {
   }

   eapi_response_t run_show_cmd(std::string const &) const {
      return eapi_response_t();
   }

   eapi_response_t run_config_cmds(std::vector<std::string> const &) const {
      return eapi_response_t();
   }

};

DEFINE_STUB_MGR_CTOR(eapi_mgr)

}  // end namespace DA
