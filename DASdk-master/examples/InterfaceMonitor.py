#!/usr/bin/env python
# Copyright (c) 2016 Arista Networks, Inc.  All rights reserved.
# Arista Networks, Inc. Confidential and Proprietary.


import DAsdk
import re
import sys
import time

# This is a simple example agent which reacts to changes to an
# interface's operational status. It keeps track of total number of
# status changes, along with the last changed interface and last
# change time. Additionally, when an interface's status changes, this
# agent will update the interface's description with a timestamp of
# when the interface changed.


class InterfaceMonitor(DAsdk.AgentHandler, DAsdk.IntfHandler):
   def __init__(self, intfMgr, agentMgr):
      DAsdk.AgentHandler.__init__(self, agentMgr)
      DAsdk.IntfHandler.__init__(self, intfMgr)
      self.tracer = DAsdk.Tracer("DASdkInterfaceMonitor")
      self.intfMgr_ = intfMgr
      self.agentMgr_ = agentMgr

      # Keep track of the total number of state changes. This number
      # gets reset whenever the agent is restarted.
      self.numIntfChanges_ = 0

      self.tracer.trace0("Constructed")

   def on_initialized(self):
      """ Callback provided by AgentHandler when all state is synchronized """
      self.tracer.trace0("We are initialized!")
      self.agentMgr_.status_set("Total intf changes", "0")
      self.watch_all_intfs(True)

   def on_oper_status(self, intfId, operState):
      """ Callback provided by IntfHandler when an interface's
      configuration changes """
      self.numIntfChanges_ += 1
      intfState = 'up' if operState == DAsdk.INTF_OPER_UP else 'down'
      lastChangeTime = re.sub( ' +', ' ', time.ctime() )
      self.tracer.trace5("The state of " + intfId.to_string() +
                         " is now " + intfState)

      # Update the interface's description with the latest change timestamp
      self.intfMgr_.description_is(intfId,
                                   "Last status change at " + lastChangeTime)

      # Update this agent's status with new statistics:
      self.agentMgr_.status_set("Total intf changes", str(self.numIntfChanges_))
      self.agentMgr_.status_set("Last change of " + intfId.to_string(), intfState)
      self.agentMgr_.status_set("Last change time of " + intfId.to_string(),
                                lastChangeTime)


if __name__ == "__main__":
   sdk = DAsdk.Sdk()
   _ = InterfaceMonitor(sdk.get_intf_mgr(), sdk.get_agent_mgr())
   sdk.main_loop(sys.argv)
