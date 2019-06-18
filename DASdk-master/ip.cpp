// Copyright (c) 2013 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#include <cstring>

#include "DA/ip.h"

namespace std {

size_t hash<DA::ip_addr_t>::operator() (DA::ip_addr_t const & addr) const {
   return 0;  // TODO: no op impl.
}

}  // end namespace std
