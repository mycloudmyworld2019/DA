// Copyright (c) 2016 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_TYPES_HARDWARE_TABLE_H
#define DA_TYPES_HARDWARE_TABLE_H

#include <DA/hash_mix.h>
#include <DA/utility.h>
#include <sstream>

namespace DA {

/** The unique identifier for a hardware table. */
class DA_SDK_PUBLIC hardware_table_key_t {
 public:
   hardware_table_key_t();
   hardware_table_key_t(std::string const & table_name,
                        std::string const & feature, std::string const & chip);
   hardware_table_key_t(std::string const & table_name,
                        std::string const & feature);
   explicit hardware_table_key_t(std::string const & table_name);

   /**
    * Getter for 'table_name': the name of the hardware resource. For example, this
    * value could be 'TCAM', 'LEM', 'ECMP', 'LPM' etc.
    */
   std::string table_name() const;

   /**
    * Getter for 'feature': the name of the feature using the table, such as
    * 'Multicast', 'IPv6' or 'MPLS'.
    */
   std::string feature() const;

   /**
    * Getter for 'chip': which chip this table is located on. If empty, then this
    * feature's usage is identical across chips.
    */
   std::string chip() const;

   bool operator==(hardware_table_key_t const & other) const;
   bool operator!=(hardware_table_key_t const & other) const;
   bool operator<(hardware_table_key_t const & other) const;
   /** The hash function for type hardware_table_key_t. */
   uint32_t hash() const;
   /** Returns a string representation of the current object's values. */
   std::string to_string() const;
   /**
    * A utility stream operator that adds a string representation of
    * hardware_table_key_t to the ostream.
    */
   friend std::ostream& operator<<(std::ostream& os,
                                   const hardware_table_key_t& obj);

 private:
   std::string table_name_;
   std::string feature_;
   std::string chip_;
};

/** The compound high watermark statistics. */
class DA_SDK_PUBLIC hardware_table_high_watermark_t {
 public:
   hardware_table_high_watermark_t();
   hardware_table_high_watermark_t(uint32_t max_entries, time_t timestamp);

   /** Getter for 'max_entries': the maximum number of entries used so far. */
   uint32_t max_entries() const;

   /** Getter for 'timestamp': the last time when high watermark reached. */
   time_t timestamp() const;

   bool operator==(hardware_table_high_watermark_t const & other) const;
   bool operator!=(hardware_table_high_watermark_t const & other) const;
   bool operator<(hardware_table_high_watermark_t const & other) const;
   /** The hash function for type hardware_table_high_watermark_t. */
   uint32_t hash() const;
   /** Returns a string representation of the current object's values. */
   std::string to_string() const;
   /**
    * A utility stream operator that adds a string representation of
    * hardware_table_high_watermark_t to the ostream.
    */
   friend std::ostream& operator<<(std::ostream& os,
                                   const hardware_table_high_watermark_t& obj);

 private:
   uint32_t max_entries_;
   time_t timestamp_;
};

/** The usage statistics for a given table (or partition of a table). */
class DA_SDK_PUBLIC hardware_table_usage_t {
 public:
   hardware_table_usage_t();
   hardware_table_usage_t(uint32_t used_entries, uint32_t free_entries,
                          uint32_t committed_entries);

   /**
    * Getter for 'used_entries': number of entries used by this feature on this
    * table.
    */
   uint32_t used_entries() const;

   /**
    * Getter for 'free_entries': the number of free entries available for this
    * feature to use. For features that share a table, if a feature increases its
    * used or committed entries, all other features' free entries will decrease.
    */
   uint32_t free_entries() const;

   /**
    * Getter for 'committed_entries': the number of entries currently allocated for
    * this feature. For tables that have no explicit partitions, this value will be
    * 0.
    */
   uint32_t committed_entries() const;

   bool operator==(hardware_table_usage_t const & other) const;
   bool operator!=(hardware_table_usage_t const & other) const;
   bool operator<(hardware_table_usage_t const & other) const;
   /** The hash function for type hardware_table_usage_t. */
   uint32_t hash() const;
   /** Returns a string representation of the current object's values. */
   std::string to_string() const;
   /**
    * A utility stream operator that adds a string representation of
    * hardware_table_usage_t to the ostream.
    */
   friend std::ostream& operator<<(std::ostream& os,
                                   const hardware_table_usage_t& obj);

 private:
   uint32_t used_entries_;
   uint32_t free_entries_;
   uint32_t committed_entries_;
};

/** The hardware table entry for a given table key. */
class DA_SDK_PUBLIC hardware_table_entry_t {
 public:
   hardware_table_entry_t();
   hardware_table_entry_t(hardware_table_usage_t const & usage,
                          uint32_t max_entries,
                          hardware_table_high_watermark_t const & high_watermark);

   /** Getter for 'usage': the compound usage statistics. */
   hardware_table_usage_t usage() const;

   /**
    * Getter for 'max_entries': the theoretical maximum number of entries
    * available.
    */
   uint32_t max_entries() const;

   /** Getter for 'high_watermark': the compound high watermark statistics. */
   hardware_table_high_watermark_t high_watermark() const;

   bool operator==(hardware_table_entry_t const & other) const;
   bool operator!=(hardware_table_entry_t const & other) const;
   bool operator<(hardware_table_entry_t const & other) const;
   /** The hash function for type hardware_table_entry_t. */
   uint32_t hash() const;
   /** Returns a string representation of the current object's values. */
   std::string to_string() const;
   /**
    * A utility stream operator that adds a string representation of
    * hardware_table_entry_t to the ostream.
    */
   friend std::ostream& operator<<(std::ostream& os,
                                   const hardware_table_entry_t& obj);

 private:
   hardware_table_usage_t usage_;
   uint32_t max_entries_;
   hardware_table_high_watermark_t high_watermark_;
};
}

#include <DA/inline/types/hardware_table.h>

#endif // DA_TYPES_HARDWARE_TABLE_H
