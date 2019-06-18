// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef DA_EXCEPTION_H
#define DA_EXCEPTION_H

#include <exception>
#include <string>

#include <DA/base.h>

#if __cplusplus <= 199711L
#define noexcept throw()
#endif

namespace DA {

/// Base exception type for all SDK-reported exceptions.
class DA_SDK_PUBLIC error : public std::exception {
 public:
   virtual ~error() noexcept;
   explicit error(std::string const & msg) noexcept;
   std::string const & msg() const noexcept;
   virtual const char * what() const noexcept;
   virtual void raise() const DA_SDK_NORETURN = 0;  ///< Throws this exception.

 private:
   std::string msg_;
};

/**
 * Base exception for all cases where an attempt was made to configure
 * the system with completely invalid arguments -- for example, an
 * out-of-range VLAN ID (>4095).
 */
class DA_SDK_PUBLIC invalid_argument_error : public error {
  public:
   virtual ~invalid_argument_error() noexcept;
   explicit invalid_argument_error(std::string const & argument_name) noexcept;
   explicit invalid_argument_error(std::string const & argument_name,
                                    std::string const & error_details) noexcept;
   virtual void raise() const;  ///< Throws this exception.
   std::string argument_name() const noexcept;

  private:
   std::string argument_name_;
};

/// The given argument was outside of the legal range of values for
/// that argument (e.g., VLAN IDs must be between 1 and 4094).
class DA_SDK_PUBLIC invalid_range_error : public invalid_argument_error {
  public:
   virtual ~invalid_range_error() noexcept;
   explicit invalid_range_error(std::string const & argument_name,
                                uint32_t min_valid, uint32_t max_valid) noexcept;
   virtual void raise() const;  ///< Throws this exception.
   uint32_t min_valid() const noexcept;
   uint32_t max_valid() const noexcept;

  private:
   uint32_t min_valid_;
   uint32_t max_valid_;
};

/**
 * Base exception type for all cases where the configuration requested could
 * not be accepted because it's impossible or invalid.
 *
 * The configuration may be invalid because of other conflicting
 * configuration.  For instance if a VLAN ID is already used as
 * an internal VLAN on a routed port, trying to manually configure
 * this VLAN ID on a trunk port would fail until the requested VLAN
 * ID is no longer used as an internal VLAN on the routed port.
 */
class DA_SDK_PUBLIC configuration_error : public error {
 public:
   virtual ~configuration_error() noexcept;
   explicit configuration_error(std::string const & msg) noexcept;
   virtual void raise() const;  ///< Throws this exception.
};

/// The given agent has not been configured
class DA_SDK_PUBLIC unconfigured_agent_error : public configuration_error {
 public:
   virtual ~unconfigured_agent_error() noexcept;
   explicit unconfigured_agent_error(std::string const & agent_name) noexcept;
   virtual void raise() const;  ///< Throws this exception.
   std::string agent_name() const noexcept;

 private:
   std::string agent_name_;
};

/// Base class for "unsupported" errors
class DA_SDK_PUBLIC unsupported_error : public error {
 public:
   virtual ~unsupported_error() noexcept;
   explicit unsupported_error(std::string const & msg) noexcept;
   virtual void raise() const;  ///< Throws this exception.

 private:
   std::string msg_;
};

/// The class for "No scoped lock obtained" errors
class DA_SDK_PUBLIC no_scoped_lock_obtained_error : public error {
 public:
   virtual ~no_scoped_lock_obtained_error() noexcept;
   explicit no_scoped_lock_obtained_error() noexcept;
   virtual void raise() const;  ///< Throws this exception.
};

}

#include <DA/inline/exception.h>

#endif // DA_EXCEPTION_H
