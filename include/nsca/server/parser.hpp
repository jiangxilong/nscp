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
#include <boost/noncopyable.hpp>

#include <nsca/nsca_packet.hpp>
//#include <cryptopp/cryptopp.hpp>

#include "handler.hpp"

namespace nsca {
	namespace server {
		class parser : public boost::noncopyable {
			unsigned int payload_length_;
			unsigned int packet_length_;

			std::string buffer_;
			boost::shared_ptr<nsca::server::handler> handler_;
		public:
			parser(unsigned int payload_length)
				: payload_length_(payload_length)
				, packet_length_(nsca::length::get_packet_length(payload_length)) {}

			template <typename InputIterator>
			boost::tuple<bool, InputIterator> digest(InputIterator begin, InputIterator end) {
				int count = packet_length_ - buffer_.size();
				for (; count > 0 && begin != end; ++begin, --count)
					buffer_.push_back(*begin);
				return boost::make_tuple(buffer_.size() >= packet_length_, begin);
			}

			void decrypt(nscp::encryption::engine &encryption) {
				encryption.decrypt_buffer(buffer_);
			}
			nsca::packet parse() {
				nsca::packet packet(payload_length_);
				packet.parse_data(buffer_.c_str(), buffer_.size());
				buffer_.clear();
				return packet;
			}
			std::string get_buffer() const {
				return buffer_;
			}
			std::string::size_type size() {
				return buffer_.size();
			}
			void reset() {
				buffer_.clear();
			}
		};
	}// namespace server
} // namespace nsca