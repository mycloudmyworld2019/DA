#!/usr/bin/env python
# Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
# Arista Networks, Inc. Confidential and Proprietary.

import DAsdk
import sys


class MyIntfHandler( DAsdk.AgentHandler, DAsdk.IntfHandler ):
   def __init__( self, intfMgr, agentMgr ):
      DAsdk.AgentHandler.__init__( self, agentMgr )
      DAsdk.IntfHandler.__init__( self, intfMgr )  # pylint: disable-msg=W0233
      self.intfMgr_ = intfMgr
      self.numIntfChanges_ = 0
      print "Constructed"

   # Callback provided by AgentHandler when all state is synchronized
   def on_initialized( self ):
      print "We are initialized!"
      self.watch_all_intfs( True )  # pylint: disable-msg=E1101

   # Callback provided by IntfHandler when an interface's configuration changes
   def on_oper_status( self, intfId, operState ):
      intfState = 'up' if operState == DAsdk.INTF_OPER_UP else 'down'
      print "The state of", intfId.to_string(), "is now", intfState
      self.numIntfChanges_ += 1
      print "   We have seen", self.numIntfChanges_, "state changes"
      self.intfMgr_.description_is(intfId,
                                   "Changed state [%d]" % self.numIntfChanges_)


sdk = DAsdk.Sdk()
mta = MyIntfHandler( sdk.get_intf_mgr(), sdk.get_agent_mgr() )
sdk.main_loop( sys.argv )
