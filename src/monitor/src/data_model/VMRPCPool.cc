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

#include "VMRPCPool.h"
#include "OneDB.h"

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

int VMRPCPool::update_monitoring(const VMMonitoringTemplate& monitoring)
{
    if (monitor_expiration <= 0)
    {
        return 0;
    }

    auto sql_xml = db->escape_str(monitoring.to_xml());

    if (sql_xml == 0)
    {
        NebulaLog::log("VMP", Log::WARNING,
                "Could not transform VM monitoring to XML");

        return -1;
    }

    if (ObjectXML::validate_xml(sql_xml) != 0)
    {
        NebulaLog::log("VMP", Log::WARNING,
                "Could not transform VM monitoring to XML" + string(sql_xml));

        db->free_str(sql_xml);
        return -1;
    }

    ostringstream oss;

    oss << "REPLACE INTO " << one_db::vm_monitor_table
        << " (" << one_db::vm_monitor_db_names << ") VALUES ("
        << monitoring.oid() << ","
        << monitoring.timestamp() << ","
        << "'" << sql_xml << "')";

    db->free_str(sql_xml);

    return db->exec_local_wr(oss);
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

int VMRPCPool::clean_expired_monitoring()
{
    if (monitor_expiration == 0)
    {
        return 0;
    }

    time_t max_mon_time = time(nullptr) - monitor_expiration;

    ostringstream   oss;
    oss << "DELETE FROM " << one_db::vm_monitor_table
        << " WHERE last_poll < " << max_mon_time;

    return db->exec_local_wr(oss);
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

void VMRPCPool::clean_all_monitoring()
{
    ostringstream   oss;

    oss << "DELETE FROM " << one_db::vm_monitor_table;

    db->exec_local_wr(oss);
}