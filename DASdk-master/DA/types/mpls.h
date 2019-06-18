// Copyright (c) 2017 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_TYPES_MPLS_H
#define DA_TYPES_MPLS_H

#include <DA/base.h>
#include <DA/hash_mix.h>
#include <DA/utility.h>
#include <sstream>

namespace DA {

/** The action to take for an MPLS route. */
enum mpls_action_t {
   MPLS_ACTION_NULL,
   MPLS_ACTION_PUSH,
   MPLS_ACTION_POP,
   MPLS_ACTION_SWAP,
   MPLS_ACTION_FORWARD,
};
/** Appends a string representation of enum mpls_action_t value to the ostream. */
std::ostream& operator<<(std::ostream& os, const mpls_action_t & enum_val);

/** Whether to use the pipe or uniform TTL inheritance mode. */
enum mpls_ttl_mode_t {
   MPLS_TTLMODE_NULL,
   /** pipe" mode, do not inherit TTL from inner IP. */
   MPLS_TTLMODE_PIPE,
   /** uniform" mode, inherit TTL from inner IP. */
   MPLS_TTLMODE_UNIFORM,
};
/** Appends a string representation of enum mpls_ttl_mode_t value to the ostream. */
std::ostream& operator<<(std::ostream& os, const mpls_ttl_mode_t & enum_val);

/**
 * The inner payload type.
 *  Used in a similar way to ethertype, as MPLS does not provide this
 *  in the packet.
 */
enum mpls_payload_type_t {
   MPLS_PAYLOAD_TYPE_NULL,
   MPLS_PAYLOAD_TYPE_MPLS,
   MPLS_PAYLOAD_TYPE_IPV4,
   MPLS_PAYLOAD_TYPE_IPV6,
   MPLS_PAYLOAD_TYPE_GUESS,
};
/**
 * Appends a string representation of enum mpls_payload_type_t value to the
 * ostream.
 */
std::ostream& operator<<(std::ostream& os, const mpls_payload_type_t & enum_val);

/**
 * An MPLS label, per RFC 3032. Note: label id 0 (default value) is the explicit
 * null label. LSRs receiving this label are expected to pop it and perform
 * ultimate IP orwarding. MPLS label numbers are 20 bits in size, covering the
 * range 0..1048575.
 */
class DA_SDK_PUBLIC mpls_label_t {
 public:
   explicit mpls_label_t(uint32_t label);

    mpls_label_t();
   /** Getter for 'label': key for the route that this via is attached to. */
   uint32_t label() const;
   /** Setter for 'label'. */
   void label_is(uint32_t label);

   enum  {
      MIN,
      /** 1048575. */
      MAX = 0xfffff,
   };
   bool operator!() const;
   bool operator==(mpls_label_t const & other) const;
   bool operator!=(mpls_label_t const & other) const;
   bool operator<(mpls_label_t const & other) const;
   /** The hash function for type mpls_label_t. */
   uint32_t hash() const;
   /** Returns a string representation of the current object's values. */
   std::string to_string() const;
   /**
    * A utility stream operator that adds a string representation of mpls_label_t
    * to the ostream.
    */
   friend std::ostream& operator<<(std::ostream& os, const mpls_label_t& obj);

 private:
   uint32_t label_;
   friend struct MplsLabelHelper;
};
}

#include <DA/inline/types/mpls.h>

#endif // DA_TYPES_MPLS_H
