/*
    Copyright (C) 2014-2015 Islog

    This file is part of Leosac.

    Leosac is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Leosac is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "GetLocalConfigVersion.hpp"
#include "core/kernel.hpp"
#include "tools/gettid.hpp"
#include "tools/log.hpp"

Leosac::Tasks::GetLocalConfigVersion::GetLocalConfigVersion(Kernel &k)
    : kernel_(k)
{
    INFO("Creating GetLocalConfigVersion task. Guid = " << get_guid());
}

bool Leosac::Tasks::GetLocalConfigVersion::do_run()
{
    config_version_ = kernel_.config_manager().config_version();
    return true;
}
