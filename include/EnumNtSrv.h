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

#include <list>
#include <string>

namespace services_helper {

	void init();
	std::string get_service_classification(const std::string &name);

	struct service_info {
		std::string name;
		std::string displayname;
		service_info(std::string name, std::string displayname) 
			: name(name)
			, displayname(displayname)
			, pid(0)
			, state(0)
			, start_type(0)
			, error_control(0)
			, type(0)
			, delayed(false)
			, triggers(0)
			, classification_(get_service_classification(name))
		{}
		service_info(const service_info &other)
			: name(other.name), displayname(other.displayname)
			, pid(other.pid), state(other.state), start_type(other.start_type), error_control(other.error_control), type(other.type), delayed(other.delayed), triggers(other.triggers)
			, binary_path(other.binary_path)
			, classification_(other.classification_) {}

		DWORD pid;
		DWORD state;
		DWORD start_type;
		DWORD error_control;
		DWORD type;
		bool delayed;
		int triggers;

		std::string binary_path;
		std::string classification_;

		std::string get_state_s() const;
		std::string get_legacy_state_s() const;
		std::string get_classification() const {
			return classification_;
		}
		std::string get_start_type_s() const;
		long long get_state_i() const { return state; }
		long long get_start_type_i() const { return start_type; }
		std::string get_type() const;
		std::string get_name() const { return name; }
		std::string get_desc() const { return displayname; }
		long long get_pid() const { return pid; }
		long long get_delayed() const { return delayed ? 1 : 0; }
		long long get_is_trigger() const { return triggers > 0 ? 1 : 0; }
		long long get_triggers() const { return triggers; }

		static long long parse_start_type(const std::string &s);
		static long long parse_state(const std::string &s);
	};

	DWORD parse_service_type(const std::string str = "service");
	DWORD parse_service_state(const std::string str = "all");
	std::list<service_info> enum_services(const std::string computer, DWORD dwServiceType, DWORD dwServiceState);
	service_info get_service_info(const std::string computer, const std::string service);
}
