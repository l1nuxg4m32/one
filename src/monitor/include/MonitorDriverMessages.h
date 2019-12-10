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

#ifndef MONITOR_DRIVER_MESSAGES_H_
#define MONITOR_DRIVER_MESSAGES_H_

#include "Driver.h"
#include "UDPStream.h"

/**
 * Messages between the Monitor daemon and drivers
 */
enum class MonitorDriverMessages : unsigned short int
{
    UNDEFINED    = 0,
    INIT         = 1,
    FINALIZE     = 2,
    MONITOR_VM   = 3,
    MONITOR_HOST = 4,
    SYSTEM_HOST  = 5,
    STATE_VM     = 6,
    START_MONITOR= 7,
    STOP_MONITOR = 8
};

typedef Driver<MonitorDriverMessages> driver_t;

typedef UDPStream<MonitorDriverMessages> udp_streamer_t;

#endif /*MONITOR_DRIVER_MESSAGES_H_*/
