// Copyright (c) 2016 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_TYPES_VRF_H
#define DA_TYPES_VRF_H

#include <DA/hash_mix.h>
#include <DA/utility.h>
#include <sstream>

namespace DA {

/** VRF state. */
enum vrf_state_t {
   /** VRF does not exist. */
   VRF_NULL,
   /** VRF is being initialized. */
   VRF_INITIALIZING,
   /** VRF is active. */
   VRF_ACTIVE,
   /** VRF is being deleted. */
   VRF_DELETING,
};
/** Appends a string representation of enum vrf_state_t value to the ostream. */
std::ostream& operator<<(std::ostream& os, const vrf_state_t & enum_val);

/** @deprecated. This data structure is used to describe a VRF on a switch. */
class DA_SDK_PUBLIC vrf_t {
 public:
   vrf_t();
   vrf_t(std::string name, vrf_state_t state, uint64_t rd);

   /** Getter for 'name': the name of the VRF. */
   std::string name() const;

   /** Getter for 'state': the state of this VRF. */
   vrf_state_t state() const;

   /** Getter for 'rd': the route distinguisher of the VRF. */
   uint64_t rd() const;

   bool operator==(vrf_t const & other) const;
   bool operator!=(vrf_t const & other) const;
   bool operator<(vrf_t const & other) const;
   /** The hash function for type vrf_t. */
   uint32_t hash() const;
   /** Returns a string representation of the current object's values. */
   std::string to_string() const;
   /**
    * A utility stream operator that adds a string representation of vrf_t to the
    * ostream.
    */
   friend std::ostream& operator<<(std::ostream& os, const vrf_t& obj);

 private:
   std::string name_;
   vrf_state_t state_;
   uint64_t rd_;
};
}

#include <DA/inline/types/vrf.h>

#endif // DA_TYPES_VRF_H
