/*
 * Copyright 2004-2016 The NSClient++ Authors - https://nsclient.org
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <boost/tuple/tuple.hpp>

#include <nscapi/nscapi_protobuf.hpp>
#include <client/command_line_parser.hpp>
#include <nscapi/nscapi_targets.hpp>
#include <nscapi/nscapi_protobuf_types.hpp>
#include <nscapi/nscapi_helper_singleton.hpp>
#include <nscapi/macros.hpp>
#include <socket/client.hpp>

#include <nrpe/packet.hpp>
#include <nrpe/client/nrpe_client_protocol.hpp>

namespace po = boost::program_options;
namespace sh = nscapi::settings_helper;

#include "../modules/NRPEClient/nrpe_client.hpp"
#include "../modules/NRPEClient/nrpe_handler.hpp"

class check_nrpe {
private:
	client::configuration client_;

public:
	check_nrpe();
	void query(const Plugin::QueryRequestMessage &request, Plugin::QueryResponseMessage &response);
};