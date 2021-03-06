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

#include "realtime_data.hpp"

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>

#include <strEx.h>

#include <nscapi/nscapi_helper_singleton.hpp>
#include <nscapi/macros.hpp>

void runtime_data::touch(boost::posix_time::ptime now) {
	BOOST_FOREACH(file_container &fc, files) {
		if (boost::filesystem::exists(fc.file)) {
			fc.size = boost::filesystem::file_size(fc.file);
		} else {
			fc.size = 0;
		}
	}
}

bool runtime_data::has_changed(transient_data_type) const {
	BOOST_FOREACH(const file_container &fc, files) {
		if (boost::filesystem::exists(fc.file) &&  fc.size != boost::filesystem::file_size(fc.file))
			return true;
	}
	return false;
}

void runtime_data::add_file(const boost::filesystem::path &path) {
	try {
		file_container fc;
		if (boost::filesystem::exists(path)) {
			fc.file = path;
			fc.size = boost::filesystem::file_size(fc.file);
		} else {
			fc.file = path;
			fc.size = 0;
		}
		files.push_back(fc);
	} catch (std::exception &e) {
		NSC_LOG_ERROR("Failed to add " + path.string() + ": " + utf8::utf8_from_native(e.what()));
	}
}

modern_filter::match_result runtime_data::process_item(filter_type &filter, transient_data_type) {
	modern_filter::match_result ret;
	BOOST_FOREACH(file_container &c, files) {
		boost::uintmax_t sz = boost::filesystem::file_size(c.file);
		if (sz == c.size)
			continue;
		std::ifstream file(c.file.string().c_str());
		if (file.is_open()) {
			std::string line;
			if (sz > c.size)
				file.seekg(c.size);
			while (file.good()) {
				std::getline(file, line, '\n');
				if (!line.empty()) {
					std::list<std::string> chunks = strEx::s::splitEx(line, utf8::cvt<std::string>(column_split));
					boost::shared_ptr<logfile_filter::filter_obj> record(new logfile_filter::filter_obj(c.file.string(), line, chunks));
					ret.append(filter.match(record));
				}
			}
			file.close();
		} else {
			NSC_LOG_ERROR("Failed to open file: " + c.file.string());
		}
	}
	return ret;
}

void runtime_data::set_split(std::string line, std::string column) {
	if (column.empty())
		column_split = "\t";
	else
		column_split = column;
	strEx::s::replace(column_split, "\\t", "\t");
	strEx::s::replace(column_split, "\\n", "\n");
	std::size_t len = column_split.size();
	if (len == 0)
		column_split = " ";
	if (len > 2 && column_split[0] == '\"' && column_split[len - 1] == '\"')
		column_split = column_split.substr(1, len - 2);


	if (line.empty())
		line = "\n";
	else
		line_split = line;
	strEx::s::replace(line_split, "\\t", "\t");
	strEx::s::replace(line_split, "\\n", "\n");
	len = line_split.size();
	if (len == 0)
		line_split = " ";
	if (len > 2 && line_split[0] == '\"' && line_split[len - 1] == '\"')
		line_split = line_split.substr(1, len - 2);
}