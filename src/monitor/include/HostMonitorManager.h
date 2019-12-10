/* -------------------------------------------------------------------------- */
/* Copyright 2002-2019, OpenNebula Project, OpenNebula Systems                */
/*                                                                            */
/* Licensed under the Apache License, Version 2.0 (the "License"); you may    */
/* not use this file except in compliance with the License. You may obtain    */
/* a copy of the License at                                                   */
/*                                                                            */
/* http://www.apache.org/licenses/LICENSE-2.0                                 */
/*                                                                            */
/* Unless required by applicable law or agreed to in writing, software        */
/* distributed under the License is distributed on an "AS IS" BASIS,          */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   */
/* See the License for the specific language governing permissions and        */
/* limitations under the License.                                             */
/* -------------------------------------------------------------------------- */

#ifndef HOST_MONITOR_MANAGER_H_
#define HOST_MONITOR_MANAGER_H_

#include "DriverManager.h"
#include "HostRPCPool.h"

#include "MonitorDriverMessages.h"
#include "MonitorDriver.h"
#include "OneMonitorDriver.h"

class Template;

/**
 *  This class controls the monitor actions and logic of OpenNebula hosts
 *
 */
class HostMonitorManager
{
public:
    HostMonitorManager(HostRPCPool * _hpool, const std::string& mad_location)
        :driver_manager(mad_location), oned_driver(this), hpool(_hpool){};

    ~HostMonitorManager() = default;

    //--------------------------------------------------------------------------
    //  Driver Interface
    //--------------------------------------------------------------------------
    /**
     *
     */
    int load_monitor_drivers(const vector<const VectorAttribute*>& mads_config)
    {
        return driver_manager.load_drivers(mads_config);
    }

    /**
     *  Start the monitor manager to process events
     */
    int start()
    {
        //TODO start timer thread
        if ( driver_manager.start() != 0 )
        {
            return -1;
        }

        oned_driver.start_driver();

        return 0;
    }

    //--------------------------------------------------------------------------
    //  Management / Monitor Interface
    //--------------------------------------------------------------------------
    /**
     *  Start the monitor agent/ or active monitor the host
     *   @param oid the host id
     */
    void start_host_monitor(int oid);

    /**
     *  Stop the monitor agent/ or stop monitor the host
     *   @param oid the host id
     */
    void stop_host_monitor(int oid);

    /**
     *  Updates the information of the given host. If it does not exist it is
     *  added to the pool
     *    @param oid host id
     *    @param xml the XML representation of the host
     */
    void update_host(int oid, const std::string &xml);

    /**
     *  Remove host from the pool
     *    @param oid host id
     */
    void delete_host(int oid);

    /**
     *  Sets the monitor information of the host. It notifies oned if needed.
     *    @param oid host id
     *    @param tmpl monitoring template
     */
    void monitor_host(int oid, Template &tmpl);

private:
    DriverManager<MonitorDriverMessages, MonitorDriver> driver_manager;

    OneMonitorDriver oned_driver;

    HostRPCPool* hpool;
};

#endif //HOST_MONITOR_MANAGER_H_
