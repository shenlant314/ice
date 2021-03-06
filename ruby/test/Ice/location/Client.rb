#!/usr/bin/env ruby
# **********************************************************************
#
# Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
#
# This copy of Ice is licensed to you under the terms described in the
# ICE_LICENSE file included in this distribution.
#
# **********************************************************************

require "Ice"
Ice::loadSlice("'-I#{Ice.getSliceDir()}' Test.ice")
require './AllTests'

class Client < ::TestHelper
    def run(args)
        properties = self.createTestProperties(args:args)
        properties.setProperty("Ice.Default.Locator", "locator:#{self.getTestEndpoint(properties:properties)}")
        self.init(properties:properties) do |communicator|
            allTests(self, communicator)
        end
    end
end
