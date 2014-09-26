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

#pragma once

#include <core/video_channel.h>

#include "clk_command_processor.h"

namespace caspar { namespace protocol { namespace CLK {

/**
 * Add the CLK command handlers to a command processor.
 *
 * @param processor The command processor to add the command handlers to.
 * @param channel   The channel to play the flash graphics on.
 */
void add_command_handlers(
	clk_command_processor& processor, const safe_ptr<core::video_channel>& channel);

}}}
