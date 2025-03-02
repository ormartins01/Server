/*
* Copyright 2013 Sveriges Television AB http://casparcg.com/
*
* This file is part of CasparCG (www.casparcg.com).
*
* CasparCG is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* CasparCG is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with CasparCG. If not, see <http://www.gnu.org/licenses/>.
*
* Author: Helge Norberg, helge.norberg@svt.se
*/

#include "../stdafx.h"

#include "base64.h"

#include <vector>
#include <algorithm>

#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/remove_whitespace.hpp>
#include <boost/range/join.hpp>
#include <boost/range/adaptor/sliced.hpp>

#include "../exception/exceptions.h"
#include "assert.h"

namespace caspar {

std::string to_base64(const char* data, uint32_t length)
{
	using namespace boost::archive::iterators;

	// From http://www.webbiscuit.co.uk/2012/04/02/base64-encoder-and-boost/
		
	typedef
		insert_linebreaks<         // insert line breaks every 76 characters
			base64_from_binary<    // convert binary values to base64 characters
				transform_width<   // retrieve 6 bit integers from a sequence of 8 bit bytes
					const unsigned char *,
					6,
					8
				>
			>,
			76
		>
        base64_iterator; // compose all the above operations in to a new iterator
	std::vector<char> bytes;
	bytes.resize(length);
	std::memcpy(bytes.data(), data, length);

	int padding = 0;

	while (bytes.size() % 3 != 0)
	{
		++padding;
		bytes.push_back(0x00);
	}

	std::string result(
			base64_iterator(bytes.data()),
			base64_iterator(bytes.data() + length));
	result.insert(result.end(), padding, '=');

	CASPAR_VERIFY((result.length() - result.length() / 77) % 4 == 0);

	return std::move(result);
}

std::vector<unsigned char> from_base64(const std::string& data)
{
	if (data.length() % 4 != 0)
	{
		auto length = std::count_if(
				data.begin(),
				data.end(),
				[] (char c) { return !std::isspace(c); });

		if (length % 4 != 0)
			BOOST_THROW_EXCEPTION(caspar_exception() << msg_info(
					"The length of a base64 sequence must be a multiple of 4"));
	}

	int padding = 0;
	std::string zero_padding;

	// binary_from_base64 does not support padding characters so we have to
	// append base64 0 -> 'A' and then remove it after decoding
	if (data.length() >= 2)
	{
		if (data[data.length() - 1] == '=')
		{
			++padding;
			zero_padding += 'A';
		}

		if (data[data.length() - 2] == '=')
		{
			++padding;
			zero_padding += 'A';
		}
	}

	if (padding > 0)
	{
		auto concatenated = boost::join(
				data | boost::adaptors::sliced(0, data.length() - padding),
				boost::make_iterator_range(zero_padding.cbegin(), zero_padding.cend()));

		// From https://svn.boost.org/trac/boost/ticket/5624
		typedef boost::archive::iterators::transform_width<
				boost::archive::iterators::binary_from_base64<
					boost::archive::iterators::remove_whitespace<decltype(concatenated.begin())>
				>,
				8,
				6
			> base64_iterator;

		std::vector<unsigned char> result(base64_iterator(concatenated.begin()), base64_iterator(concatenated.end()));

		result.resize(result.size() - padding);

		return std::move(result);
	}
	else
	{
		// From https://svn.boost.org/trac/boost/ticket/5624
		typedef boost::archive::iterators::transform_width<
				boost::archive::iterators::binary_from_base64<
					boost::archive::iterators::remove_whitespace<std::string::const_iterator>
				>,
				8,
				6
			> base64_iterator;
		std::vector<unsigned char> result(base64_iterator(data.begin()), base64_iterator(data.end()));

		return std::move(result);
	}
}

}
