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

#include "OneMonitorDriver.h"

#include "HostMonitorManager.h"
#include "NebulaLog.h"

HostMonitorManager * OneMonitorDriver::hm = nullptr;

OneMonitorDriver::OneMonitorDriver(HostMonitorManager * _hm)
{
    hm = _hm;

    register_action(OpenNebulaMessages::UNDEFINED,
            &OneMonitorDriver::_undefined);

    register_action(OpenNebulaMessages::UPDATE_HOST,
            &OneMonitorDriver::_update_host);

    register_action(OpenNebulaMessages::DEL_HOST,
            &OneMonitorDriver::_del_host);

    register_action(OpenNebulaMessages::START_MONITOR,
            &OneMonitorDriver::_start_monitor);

    register_action(OpenNebulaMessages::STOP_MONITOR,
            &OneMonitorDriver::_stop_monitor);
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

void OneMonitorDriver::host_state(int oid, const std::string& state)
{
    Message<OpenNebulaMessages> oned_msg;

    oned_msg.type(OpenNebulaMessages::HOST_STATE);
    oned_msg.oid(oid);
    oned_msg.payload(state);

    write2one(oned_msg);
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

void OneMonitorDriver::_undefined(message_t msg)
{
    NebulaLog::info("MON", "Received UNDEFINED msg: " + msg->payload());
}

void OneMonitorDriver::_update_host(message_t msg)
{
    hm->update_host(msg->oid(), msg->payload());
}

void OneMonitorDriver::_del_host(message_t msg)
{
    hm->delete_host(msg->oid());
}

void OneMonitorDriver::_start_monitor(message_t msg)
{
    hm->start_host_monitor(msg->oid());
}

void OneMonitorDriver::_stop_monitor(message_t msg)
{
    hm->stop_host_monitor(msg->oid());
}
