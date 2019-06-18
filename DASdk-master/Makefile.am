lib_LTLIBRARIES = libDA.la

AM_CXXFLAGS = -fvisibility=hidden -std=gnu++14
AM_CXXFLAGS += -Wextra -Wall -Wnon-virtual-dtor -Werror -Wno-unused-parameter -Wno-error=unused-value
AM_LDFLAGS = -Wl,--no-undefined

PUBLISHED_HEADER_FILES := $(wildcard DA/*.h) $(wildcard DA/inline/*.h) $(wildcard DA/types/*.h) $(wildcard DA/inline/types/*.h)
nobase_include_HEADERS = $(PUBLISHED_HEADER_FILES)

EXTRA_DIST = MaybePrintProfilesAndExit.cpp $(wildcard examples/*) $(wildcard examples/test/*)
EXTRA_DIST += GetAgentProcessName.cpp

libDA_la_SOURCES =
libDA_la_SOURCES += agent.cpp
libDA_la_SOURCES += acl.cpp
libDA_la_SOURCES += aresolve.cpp
libDA_la_SOURCES += aresolve_types.cpp
libDA_la_SOURCES += class_map.cpp
libDA_la_SOURCES += decap_group.cpp
libDA_la_SOURCES += directflow.cpp
libDA_la_SOURCES += directflow_types.cpp
libDA_la_SOURCES += eapi.cpp
libDA_la_SOURCES += eth_types.cpp
libDA_la_SOURCES += eth_intf.cpp
libDA_la_SOURCES += eth_lag_intf.cpp
libDA_la_SOURCES += eth_phy_intf.cpp
libDA_la_SOURCES += event_loop.cpp
libDA_la_SOURCES += exception.cpp
libDA_la_SOURCES += fd.cpp
libDA_la_SOURCES += hardware_table.cpp
libDA_la_SOURCES += intf.cpp
libDA_la_SOURCES += intf_types.cpp
libDA_la_SOURCES += impl.cpp impl.h
libDA_la_SOURCES += ip.cpp
libDA_la_SOURCES += ip_types.cpp
libDA_la_SOURCES += ip_intf.cpp
libDA_la_SOURCES += ip_route.cpp
libDA_la_SOURCES += fib.cpp
libDA_la_SOURCES += iterator.cpp
libDA_la_SOURCES += macsec.cpp
libDA_la_SOURCES += mac_table.cpp
libDA_la_SOURCES += mlag.cpp
libDA_la_SOURCES += mpls.cpp
libDA_la_SOURCES += mpls_route.cpp
libDA_la_SOURCES += mpls_vrf_label.cpp
libDA_la_SOURCES += nexthop_group.cpp
libDA_la_SOURCES += panic.cpp
libDA_la_SOURCES += neighbor_table.cpp
libDA_la_SOURCES += policy_map.cpp
libDA_la_SOURCES += policy_map_types.cpp
libDA_la_SOURCES += sdk.cpp
libDA_la_SOURCES += subintf.cpp
libDA_la_SOURCES += system.cpp
libDA_la_SOURCES += timer.cpp
libDA_la_SOURCES += tracing.cpp
libDA_la_SOURCES += version.cpp
libDA_la_SOURCES += vrf.cpp
libDA_la_SOURCES += bfd.cpp
libDA_la_SOURCES += lldp.cpp

libDA_la_LDFLAGS = -version-info $(SDK_LIBTOOL_VERSION) -lrt
libDA_la_LDFLAGS += -soname libDA.so.$(SDK_VERSION_MAJOR).$(SDK_VERSION_MINOR)


noinst_PROGRAMS = HelloWorld
HelloWorld_SOURCES = examples/HelloWorld.cpp
HelloWorld_LDADD = libDA.la

# --------- #
# SWIG libs #
# --------- #

if USE_PYTHON
SWIG_FILES := DASdk.i SwigUtils.i Includes.i SwigRenamer.py swig.patch
python_PYTHON = $(srcdir)/DAsdk.py
pyexec_LTLIBRARIES = _DAsdk.la
_DAsdk_la_SOURCES = $(srcdir)/DAsdk_wrap.cpp $(srcdir)/DAsdk_wrap.h
_DAsdk_la_LDFLAGS = -avoid-version -module
# Swig generated code produces some warnings, depending on the combination of
# Python version and GCC version, so we turn the problematic ones off here as
# we build with -Werror:
_DAsdk_la_CXXFLAGS = $(AM_CXXFLAGS) -Wno-strict-aliasing -Wno-missing-field-initializers
_DAsdk_la_CXXFLAGS += -Wno-error=maybe-uninitialized
_DAsdk_la_CPPFLAGS = -I/usr/include/python$(PYTHON_VERSION)
_DAsdk_la_LIBADD = libDA.la

$(srcdir)/DAsdk_wrap.cpp: $(SWIG_FILES) $(nobase_include_HEADERS) $(srcdir)/DAsdk_wrap.cpp
	SRCDIR=$(srcdir) prefix [stubs-swig] $(SWIG) -threads -c++ -python -dirvtable -O -I$(srcdir) -o $@-t $(srcdir)/DASdk.i
	patch --batch --no-backup-if-mismatch -p0 <$(srcdir)/swig.patch
	 mv $@-t $@

$(srcdir)/DAsdk_wrap.h: $(srcdir)/DAsdk_wrap.cpp

$(srcdir)/DAsdk.py: $(srcdir)/DAsdk_wrap.cpp

BUILT_SOURCES = $(_DAsdk_la_SOURCES)
MAINTAINERCLEANFILES = $(BUILT_SOURCES)
endif

ACLOCAL_AMFLAGS = -I autogen
EXTRA_DIST += bootstrap build.sh $(SWIG_FILES)
EXTRA_DIST += LICENSE
EXTRA_DIST += ltmain.patch

dist-hook:
	chmod -R u+w $(distdir)
