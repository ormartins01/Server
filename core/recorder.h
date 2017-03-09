/*
* This file is part of TVP's fork CasparCG (www.casparcg.com).
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
*/

#pragma once

#include "monitor/monitor.h"

#include <core/parameters/parameters.h>
#include <core/video_channel.h>

#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

#include <agents.h>

namespace caspar {
	namespace core {

		class recorder : boost::noncopyable
		{
		public:

			// Static Members

			// Constructors

			// Methods
			virtual void Capture(std::shared_ptr<core::video_channel> channel, std::wstring tc_in, std::wstring tc_out, unsigned int preroll, int offset, std::wstring file_name, const core::parameters& params) {};
			virtual void Abort() {};
			
			// Properties

			virtual int index() const 
			{
				return 0;
			};
		};
	}
}