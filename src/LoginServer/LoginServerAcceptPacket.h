/*
Copyright (C) 2008-2010 Vana Development Team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#pragma once

#include "Types.h"
#include <string>

using std::string;

class LoginServerAcceptConnection;
class World;

namespace LoginServerAcceptPacket {
	void connect(LoginServerAcceptConnection *player, World *world);
	void noMoreWorld(LoginServerAcceptConnection *player);
	void connectChannel(LoginServerAcceptConnection *player, int8_t worldid, uint32_t ip, uint16_t port);
	void newPlayer(LoginServerAcceptConnection *player, uint16_t channel, int32_t charid);
	void removeCharacter(LoginServerAcceptConnection *player, int32_t charid);
};
