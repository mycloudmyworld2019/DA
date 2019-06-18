// Copyright (c) 2013 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

%module(directors="1") DAsdk

// Importing this module allows us to produce better stack traces when
// a Python exception bubbles up into the C++ code.
%pythonbegin %{
try:
   import _Tac
except ImportError:
   pass
%}

%include "stdint.i"
%include "std_list.i"
%include "std_map.i"
%include "std_pair.i"
%include "std_set.i"
%include "std_string.i"
%include "std_unordered_set.i"
%include "std_vector.i"
%import "SwigUtils.i"
%include "typemaps.i"

// Change all some_class_t names to just SomeClass:
 // The <<""< uses automatic string concatination to get around the
 // conflict marker limitation.
%rename("%(command:python $SRCDIR/SwigRenamer.py --classname <<""<)s", %$isclass) "";
// Don't do anything for enum values, functions or variables.

// Cause SWIG to produce basic documentation in the generated DAsdk.py
// For functions, the docstrings describe argument type and return types.
// All constructors are listed in the __init__ function for the type.
// These docstrings are later examined by sphinx.
%feature("autodoc", "1");

// Ignored conversions:

#define SWIG_FILE_WITH_INIT
%import "DA/base.h"

// Tell swig about our custom types (copied from base.h)
typedef double seconds_t;
typedef uint16_t uint16_be_t;
typedef uint32_t uint32_be_t;
typedef uint64_t uint64_be_t;

%typemap(in) void *DAsdk_context {
   $1 = PyLong_AsVoidPtr($input);
}

// To make sure instance of DASdk types are hashable in python, this is important
// when the objects are used in sets, dicts, etc (as key).
// Three test cases added for this, one is in EthLagIntfTest.py, one in IntfTest.py
// and the third in IpAddrTest.py
%rename(__hash__) *::hash;

// generate directors for all classes that have virtual methods, except the managers
%feature("director");
%feature("nodirector") DA::acl_mgr;
%feature("nodirector") DA::agent_mgr;
%feature("nodirector") DA::aresolve_mgr;
%feature("nodirector") DA::bfd_session_mgr;
%feature("nodirector") DA::class_map_mgr;
%feature("nodirector") DA::decap_group_mgr;
%feature("nodirector") DA::directflow_mgr;
%feature("nodirector") DA::eapi_mgr;
%feature("nodirector") DA::eth_intf_mgr;
%feature("nodirector") DA::eth_lag_intf_mgr;
%feature("nodirector") DA::eth_phy_intf_mgr;
%feature("nodirector") DA::eth_phy_intf_counter_mgr;
%feature("nodirector") DA::fib_mgr;
%feature("nodirector") DA::hardware_table_mgr;
%feature("nodirector") DA::intf_mgr;
%feature("nodirector") DA::intf_counter_mgr;
%feature("nodirector") DA::ip_intf_mgr;
%feature("nodirector") DA::ip_route_mgr;
%feature("nodirector") DA::lldp_mgr;
%feature("nodirector") DA::macsec_mgr;
%feature("nodirector") DA::mac_table_mgr;
%feature("nodirector") DA::mlag_mgr;
%feature("nodirector") DA::mpls_route_mgr;
%feature("nodirector") DA::mpls_vrf_label_mgr;
%feature("nodirector") DA::neighbor_table_mgr;
%feature("nodirector") DA::nexthop_group_mgr;
%feature("nodirector") DA::policy_map_mgr;
%feature("nodirector") DA::subintf_mgr;
%feature("nodirector") DA::system_mgr;
%feature("nodirector") DA::timeout_mgr;
%feature("nodirector") DA::vrf_mgr;

%{
#include "DA/acl.h"
#include "DA/agent.h"
#include "DA/aresolve.h"
#include "DA/bfd.h"
#include "DA/event_loop.h"
#include "DA/class_map.h"
#include "DA/decap_group.h"
#include "DA/directflow.h"
#include "DA/eapi.h"
#include "DA/eth.h"
#include "DA/eth_intf.h"
#include "DA/eth_lag_intf.h"
#include "DA/eth_phy_intf.h"
#include "DA/exception.h"
#include "DA/fd.h"
#include "DA/fib.h"
#include "DA/hardware_table.h"
#include "DA/hash_mix.h"
#include "DA/intf.h"
#include "DA/ip.h"
#include "DA/ip_intf.h"
#include "DA/ip_route.h"
#include "DA/iterator.h"
#include "DA/lldp.h"
#include "DA/macsec.h"
#include "DA/mac_table.h"
#include "DA/mlag.h"
#include "DA/mpls.h"
#include "DA/mpls_route.h"
#include "DA/mpls_vrf_label.h"
#include "DA/neighbor_table.h"
#include "DA/nexthop_group.h"
#include "DA/policy_map.h"
#include "DA/sdk.h"
#include "DA/subintf.h"
#include "DA/system.h"
#include "DA/timer.h"
#include "DA/tracing.h"
#include "DA/utility.h"
#include "DA/version.h"
#include "DA/vrf.h"

using namespace DA;

/// Used to convert uncaught exceptions into Python exceptions.
class misc_error : public error {
 public:
   virtual ~misc_error() noexcept {
   }
   explicit misc_error(std::string const & msg) noexcept : error(msg) {
   }
   virtual void raise() const DA_SDK_NORETURN {
      throw *this;
   }
};

struct stop_iteration {};

#define TRANSLATE_EXCEPTION(ExceptionClass)\
   catch(ExceptionClass const & e) {\
      ExceptionClass * exc = new ExceptionClass(e);\
      PyObject * obj = SWIG_NewPointerObj(exc, SWIGTYPE_p_DA__##ExceptionClass,\
                                          SWIG_POINTER_OWN);\
      PyErr_SetObject(SWIG_Python_ExceptionType(SWIGTYPE_p_DA__##ExceptionClass), obj);\
   }

void throw_py_error(error const& err) {
   try {
      err.raise();
   }
   TRANSLATE_EXCEPTION(invalid_range_error)
   TRANSLATE_EXCEPTION(invalid_argument_error)
   TRANSLATE_EXCEPTION(no_such_interface_error)
   TRANSLATE_EXCEPTION(not_switchport_eligible_error)
   TRANSLATE_EXCEPTION(invalid_vlan_error)
   TRANSLATE_EXCEPTION(internal_vlan_error)
   TRANSLATE_EXCEPTION(unsupported_policy_feature_error)
   TRANSLATE_EXCEPTION(address_overlap_error)
   catch(error const & e) {
      misc_error * exc = new misc_error(e.msg());
      PyObject * obj = SWIG_NewPointerObj(exc, SWIGTYPE_p_DA__error,
                                          SWIG_POINTER_OWN);
      PyErr_SetObject(SWIG_Python_ExceptionType(SWIGTYPE_p_DA__error), obj);
   }
}

%}

%exceptionclass error;

%exception {
   try {
      $action
   } catch (error const & e) {
      throw_py_error(e);
      SWIG_fail;
   }
}

%typemap(throws) stop_iteration {
  (void)$1;
  SWIG_SetErrorObj(PyExc_StopIteration, SWIG_Py_Void());
  SWIG_fail;
}

%include "DA/iterator.h"

%define wrap_iterator(IteratorClass, IteratorImplClass, ReturnType)
   %template() DA::iter_base<ReturnType, IteratorImplClass>;
   %extend IteratorClass {
      // Note: __next__() is needed for Python 3.x, so we alias it to next().
      %pythoncode {
         def __iter__(self): return self
         def __next__(self): return self.next()
      }
      ReturnType next() throw(stop_iteration) {
         SWIG_PYTHON_THREAD_BEGIN_BLOCK;
         if(!*$self) {
            throw stop_iteration();
         }
         ReturnType result = $self->operator*();
         $self->operator++();
         SWIG_PYTHON_THREAD_END_BLOCK;
         return result;
      }
   }
%enddef

%define default_iterator(ManagerClass, IteratorMethod)
   %extend ManagerClass {
      %pythoncode {
         def __iter__(self): return self.IteratorMethod()
      }
   }
%enddef

// Different stl containers used directly in our exposed headers
%template() std::map<uint32_t, DA::class_map_rule_t>;
%template() std::map<uint32_t, DA::policy_map_rule_t>;
%template() std::map<uint16_t, DA::ip_addr_t>;
%template() std::set<DA::intf_id_t>;
%template() std::set<DA::ip_addr_mask_t>;
%template() std::list<uint16_t>;
%template() std::list<DA::mpls_label_t>;
%template() std::map<uint16_t, DA::nexthop_group_mpls_action_t>;
%template() std::map<uint16_t, DA::nexthop_group_entry_t>;
%template() std::pair<uint32_t, DA::acl_rule_ip_t>;
%template() std::pair<uint32_t, DA::acl_rule_eth_t>;
%template() std::unordered_set<DA::ip_addr_t>;
%template() std::map<DA::lldp_tlv_type_t, std::string>;
%template() std::list<DA::lldp_management_address_t>;
%template() std::vector<std::string>;

// For vlan_set_t
%template(_BitSet4096) std::bitset<4096>;

// Ignore the `raise' method of all exceptions.  `raise' is a Python keyword
// and also this method is used to throw the exception from C++ and is useless
// in Python.
%ignore raise;
// Ignore the `what' method of all exceptions.  This method is redundant with
// the `msg' method (it's only provided for "compatibility" with standard
// exceptions).
%ignore what;
// Python code should use Python exceptions, and not call panic().
%ignore panic;
%ignore vpanic;
%include "Includes.i"


%extend DA::error {
   std::string const & __str__() {
      return $self->msg();
   }
};

// Pythonify our iterators.
wrap_iterator(DA::acl_iter_t, DA::acl_iter_impl, DA::acl_key_t);
wrap_iterator(DA::acl_rule_ip_iter_t, DA::acl_rule_ip_iter_impl, DA::acl_rule_ip_entry_t);
wrap_iterator(DA::acl_rule_eth_iter_t, DA::acl_rule_eth_iter_impl, DA::acl_rule_eth_entry_t);
wrap_iterator(DA::agent_option_iter_t, DA::agent_option_iter_impl, std::string);
wrap_iterator(DA::agent_status_iter_t, DA::agent_status_iter_impl, std::string);
wrap_iterator(DA::bfd_session_iter_t, DA::bfd_session_iter_impl, DA::bfd_session_key_t);
wrap_iterator(DA::class_map_iter_t, DA::class_map_iter_impl, DA::class_map_key_t);
wrap_iterator(DA::fib_fec_iter_t, DA::fib_fec_iter_impl, DA::fib_fec_t);
wrap_iterator(DA::fib_route_iter_t, DA::fib_route_iter_impl, DA::fib_route_t);
wrap_iterator(DA::flow_entry_iter_t, DA::flow_entry_iter_impl, DA::flow_entry_t);
wrap_iterator(DA::eth_intf_iter_t, DA::eth_intf_iter_impl, DA::intf_id_t);
wrap_iterator(DA::eth_lag_intf_iter_t, DA::eth_lag_intf_iter_impl, DA::intf_id_t);
wrap_iterator(DA::eth_lag_intf_member_iter_t, DA::eth_lag_intf_member_iter_impl, DA::intf_id_t);
wrap_iterator(DA::eth_phy_intf_iter_t, DA::eth_phy_intf_iter_impl, DA::intf_id_t);
wrap_iterator(DA::hardware_table_iter_t, DA::hardware_table_iter_impl, DA::hardware_table_key_t);
wrap_iterator(DA::intf_iter_t, DA::intf_iter_impl, DA::intf_id_t);
wrap_iterator(DA::ip_route_iter_t, DA::ip_route_iter_impl, DA::ip_route_t);
wrap_iterator(DA::ip_route_via_iter_t, DA::ip_route_via_iter_impl, DA::ip_route_via_t);
wrap_iterator(DA::decap_group_iter_t, DA::decap_group_iter_impl, DA::decap_group_t);
wrap_iterator(DA::lldp_remote_system_iter_t, DA::lldp_remote_system_iter_impl, lldp_remote_system_t);
wrap_iterator(DA::lldp_intf_iter_t, DA::lldp_intf_iter_impl, intf_id_t);
wrap_iterator(DA::lldp_neighbor_iter_t, DA::lldp_neighbor_iter_impl, lldp_neighbor_t);
wrap_iterator(DA::mac_table_iter_t, DA::mac_table_iter_impl, DA::mac_key_t);
wrap_iterator(DA::mac_table_status_iter_t, DA::mac_table_status_iter_impl, DA::mac_key_t);
wrap_iterator(DA::mpls_route_iter_t, DA::mpls_route_iter_impl, DA::mpls_route_t);
wrap_iterator(DA::mpls_route_status_iter_t, DA::mpls_route_status_iter_impl, DA::mpls_route_key_t);
wrap_iterator(DA::mpls_route_via_iter_t, DA::mpls_route_via_iter_impl, DA::mpls_route_via_t);
wrap_iterator(DA::mpls_route_via_status_iter_t, DA::mpls_route_via_status_iter_impl, DA::mpls_route_via_t);
wrap_iterator(DA::mpls_vrf_label_iter_t, DA::mpls_vrf_label_iter_impl, DA::mpls_vrf_label_t);
wrap_iterator(DA::neighbor_table_iter_t, DA::neighbor_table_iter_impl, DA::neighbor_key_t);
wrap_iterator(DA::neighbor_table_status_iter_t, DA::neighbor_table_status_iter_impl, DA::neighbor_key_t);
wrap_iterator(DA::nexthop_group_iter_t, DA::nexthop_group_iter_impl, DA::nexthop_group_t);
wrap_iterator(DA::policy_map_iter_t, DA::policy_map_iter_impl, DA::policy_map_key_t);
wrap_iterator(DA::subintf_iter_t, DA::subintf_iter_impl, DA::intf_id_t);
wrap_iterator(DA::vrf_iter_t, DA::vrf_iter_impl, std::string);

// Make managers themselves iterable, when it makes sense:
default_iterator(DA::directflow_mgr, flow_entry_iter);
default_iterator(DA::intf_mgr, intf_iter);
default_iterator(DA::ip_route_mgr, ip_route_iter);
default_iterator(DA::mpls_route_mgr, mpls_route_iter);
default_iterator(DA::subintf_mgr, subintf_iter);
