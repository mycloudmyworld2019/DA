// Copyright (c) 2016 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_TYPES_SUBINTF_H
#define DA_TYPES_SUBINTF_H

#include <DA/hash_mix.h>
#include <DA/types/eth.h>
#include <DA/types/intf.h>
#include <DA/utility.h>
#include <sstream>

namespace DA {

/** This data structure defines a subinterface. */
class DA_SDK_PUBLIC subintf_t {
 public:
   subintf_t();
   subintf_t(intf_id_t intf_id, vlan_id_t vlan_id);

   /** Getter for 'intf_id': the interface ID of this subinterface. */
   intf_id_t intf_id() const;

   /** Getter for 'vlan_id': the VLAN tag of this subinterface. */
   vlan_id_t vlan_id() const;
   /** Setter for 'vlan_id'. */
   void vlan_id_is(vlan_id_t vlan_id);

   bool operator==(subintf_t const & other) const;
   bool operator!=(subintf_t const & other) const;
   /** The hash function for type subintf_t. */
   uint32_t hash() const;
   /** Returns a string representation of the current object's values. */
   std::string to_string() const;
   /**
    * A utility stream operator that adds a string representation of subintf_t to
    * the ostream.
    */
   friend std::ostream& operator<<(std::ostream& os, const subintf_t& obj);

 private:
   intf_id_t intf_id_;
   vlan_id_t vlan_id_;
};
}

#include <DA/inline/types/subintf.h>

#endif // DA_TYPES_SUBINTF_H
