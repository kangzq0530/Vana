/*
Copyright (C) 2008 Vana Development Team

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

#include "LuaNPC.h"
#include "Player.h"
#include "Players.h"
#include "Quests.h"
#include "Maps.h"
#include "Levels.h"
#include "InventoryPacket.h"

LuaNPC::LuaNPC(char *filename, NPC *npc) {
	lua_State *luaVm = lua_open();

	lua_pushinteger(luaVm, npc->getPlayer()->getPlayerid()); // Pushing id for reference from static functions
	lua_setglobal(luaVm, "playerid");
	lua_pushinteger(luaVm, npc->getState());
	lua_setglobal(luaVm, "state");

	lua_register(luaVm, "addText", &LuaNPCExports::addText);
	lua_register(luaVm, "addChar", &LuaNPCExports::addChar);
	lua_register(luaVm, "sendSimple", &LuaNPCExports::sendSimple);
	lua_register(luaVm, "sendYesNo", &LuaNPCExports::sendYesNo);
	lua_register(luaVm, "sendNext", &LuaNPCExports::sendNext);
	lua_register(luaVm, "sendBackNext", &LuaNPCExports::sendBackNext);
	lua_register(luaVm, "sendBackOK", &LuaNPCExports::sendBackOK);
	lua_register(luaVm, "sendOK", &LuaNPCExports::sendOK);
	lua_register(luaVm, "sendAcceptDecline", &LuaNPCExports::sendAcceptDecline);
	lua_register(luaVm, "sendGetText", &LuaNPCExports::sendGetText);
	lua_register(luaVm, "sendGetNumber", &LuaNPCExports::sendGetNumber);
	lua_register(luaVm, "sendStyle", &LuaNPCExports::sendStyle);
	lua_register(luaVm, "giveItem", &LuaNPCExports::giveItem);
	lua_register(luaVm, "giveMesos", &LuaNPCExports::giveMesos);
	lua_register(luaVm, "giveEXP", &LuaNPCExports::giveEXP);
	lua_register(luaVm, "getLevel", &LuaNPCExports::getLevel);
	lua_register(luaVm, "getGender", &LuaNPCExports::getGender);
	lua_register(luaVm, "getItemAmount", &LuaNPCExports::getItemAmount);
	lua_register(luaVm, "getMesos", &LuaNPCExports::getMesos);
	lua_register(luaVm, "getMap", &LuaNPCExports::getMap);
	lua_register(luaVm, "getHP", &LuaNPCExports::getHP);
	lua_register(luaVm, "getHair", &LuaNPCExports::getHair);
	lua_register(luaVm, "getEyes", &LuaNPCExports::getEyes);
	lua_register(luaVm, "getSelected", &LuaNPCExports::getSelected);
	lua_register(luaVm, "getNumber", &LuaNPCExports::getNumber);
	lua_register(luaVm, "getText", &LuaNPCExports::getText);
	lua_register(luaVm, "getVariable", &LuaNPCExports::getVariable);
	lua_register(luaVm, "setState", &LuaNPCExports::setState);
	lua_register(luaVm, "setStyle", &LuaNPCExports::setStyle);
	lua_register(luaVm, "setMap", &LuaNPCExports::setMap);
	lua_register(luaVm, "setHP", &LuaNPCExports::setHP);
	lua_register(luaVm, "setVariable", &LuaNPCExports::setVariable);
	lua_register(luaVm, "addQuest", &LuaNPCExports::addQuest);
	lua_register(luaVm, "endQuest", &LuaNPCExports::endQuest);
	lua_register(luaVm, "endNPC", &LuaNPCExports::end); // end() doesn't work (reserved?)

	luaL_dofile(luaVm, filename);
	lua_close(luaVm);
}

NPC * LuaNPCExports::getNPC(lua_State *luaVm) {
	lua_getglobal(luaVm, "playerid");
	return Players::players[lua_tointeger(luaVm, -1)]->getNPC();
}

int LuaNPCExports::addText(lua_State *luaVm) {
	getNPC(luaVm)->addText(lua_tostring(luaVm, -1));
	return 1;
}

int LuaNPCExports::addChar(lua_State *luaVm) {
	getNPC(luaVm)->addChar((char) lua_tointeger(luaVm, -1));
	return 1;
}

int LuaNPCExports::sendSimple(lua_State *luaVm) {
	getNPC(luaVm)->sendSimple();
	return 1;
}
	
int LuaNPCExports::sendYesNo(lua_State *luaVm) {
	getNPC(luaVm)->sendYesNo();
	return 1;
}

int LuaNPCExports::sendNext(lua_State *luaVm) {
	getNPC(luaVm)->sendNext();
	return 1;
}

int LuaNPCExports::sendBackNext(lua_State *luaVm) {
	getNPC(luaVm)->sendBackNext();
	return 1;
}

int LuaNPCExports::sendBackOK(lua_State *luaVm) {
	getNPC(luaVm)->sendBackOK();
	return 1;
}

int LuaNPCExports::sendOK(lua_State *luaVm) {
	getNPC(luaVm)->sendOK();
	return 1;
}

int LuaNPCExports::sendAcceptDecline(lua_State *luaVm) {
	getNPC(luaVm)->sendAcceptDecline();
	return 1;
}

int LuaNPCExports::sendGetText(lua_State *luaVm) {
	getNPC(luaVm)->sendGetText();
	return 1;
}

int LuaNPCExports::sendGetNumber(lua_State *luaVm) {
	getNPC(luaVm)->sendGetNumber(lua_tointeger(luaVm, -3), lua_tointeger(luaVm, -2), lua_tointeger(luaVm, -1));
	return 1;
}

int LuaNPCExports::sendStyle(lua_State *luaVm) {
	char size = (char) lua_tointeger(luaVm, -1);
	int *styles = new int[size];

	lua_pop(luaVm, 1);
	lua_pushnil(luaVm);
	while (lua_next(luaVm, -2) != 0) {
		int a = lua_tointeger(luaVm, -2)-1;
		int b = lua_tointeger(luaVm, -1);
		styles[lua_tointeger(luaVm, -2)-1] = lua_tointeger(luaVm, -1);
		lua_pop(luaVm, 1);
	}

	getNPC(luaVm)->sendStyle(styles, size);
	return 1;
}

int LuaNPCExports::giveItem(lua_State *luaVm) {
	Quests::giveItem(getNPC(luaVm)->getPlayer(), lua_tointeger(luaVm, -2), lua_tointeger(luaVm, -1));
	return 1;
}

int LuaNPCExports::giveMesos(lua_State *luaVm) {
	Quests::giveMesos(getNPC(luaVm)->getPlayer(), lua_tointeger(luaVm, -1));
	return 1;
}

int LuaNPCExports::giveEXP(lua_State *luaVm) {
	Levels::giveEXP(getNPC(luaVm)->getPlayer(), lua_tointeger(luaVm, -1), 1);
	return 1;
}

int LuaNPCExports::getLevel(lua_State *luaVm) {
	lua_pushnumber(luaVm, getNPC(luaVm)->getPlayer()->getLevel());
	return 1;
}

int LuaNPCExports::getGender(lua_State *luaVm) {
	lua_pushnumber(luaVm, getNPC(luaVm)->getPlayer()->getGender());
	return 1;
}

int LuaNPCExports::getItemAmount(lua_State *luaVm) {
	lua_pushnumber(luaVm, getNPC(luaVm)->getPlayer()->inv->getItemAmount(lua_tointeger(luaVm, -1)));
	return 1;
}

int LuaNPCExports::getMesos(lua_State *luaVm) {
	lua_pushnumber(luaVm, getNPC(luaVm)->getPlayer()->inv->getMesos());
	return 1;
}

int LuaNPCExports::getMap(lua_State *luaVm) {
	lua_pushnumber(luaVm, getNPC(luaVm)->getPlayer()->getMap());
	return 1;
}

int LuaNPCExports::getHP(lua_State *luaVm) {
	lua_pushnumber(luaVm, getNPC(luaVm)->getPlayer()->getHP());
	return 1;
}

int LuaNPCExports::getHair(lua_State *luaVm) {
	lua_pushnumber(luaVm, getNPC(luaVm)->getPlayer()->getHair());
	return 1;
}

int LuaNPCExports::getEyes(lua_State *luaVm) {
	lua_pushnumber(luaVm, getNPC(luaVm)->getPlayer()->getEyes());
	return 1;
}

int LuaNPCExports::getSelected(lua_State *luaVm) {
	lua_pushnumber(luaVm, getNPC(luaVm)->getSelected());
	return 1;
}

int LuaNPCExports::getNumber(lua_State *luaVm) {
	lua_pushnumber(luaVm, getNPC(luaVm)->getNumber());
	return 1;
}

int LuaNPCExports::getText(lua_State *luaVm) {
	lua_pushstring(luaVm, getNPC(luaVm)->getText());
	return 1;
}

int LuaNPCExports::getVariable(lua_State *luaVm) {
	lua_pushnumber(luaVm, getNPC(luaVm)->getVariable(lua_tostring(luaVm, -1)));
	return 1;
}

int LuaNPCExports::setState(lua_State *luaVm) {
	getNPC(luaVm)->setState(lua_tointeger(luaVm, -1));
	return 1;
}

int LuaNPCExports::setStyle(lua_State *luaVm) {
	int id = lua_tointeger(luaVm, -1);
	if(id/10000 == 0){
		getNPC(luaVm)->getPlayer()->setSkin((char)id);
	}
	else if(id/10000 == 2){
		getNPC(luaVm)->getPlayer()->setEyes(id);
	}
	else if(id/10000 == 3){
		getNPC(luaVm)->getPlayer()->setHair(id);
	}
	InventoryPacket::updatePlayer(getNPC(luaVm)->getPlayer());
	return 1;
}

int LuaNPCExports::setMap(lua_State *luaVm) {
	int mapid = lua_tointeger(luaVm, -1);
	if(Maps::info.find(mapid) != Maps::info.end())
		Maps::changeMap(getNPC(luaVm)->getPlayer(), mapid, 0);
	return 1;
}

int LuaNPCExports::setHP(lua_State *luaVm) {
	getNPC(luaVm)->getPlayer()->setHP(lua_tointeger(luaVm, -1));
	return 1;
}

int LuaNPCExports::setVariable(lua_State *luaVm) {
	getNPC(luaVm)->setVariable(lua_tostring(luaVm, -2), lua_tointeger(luaVm, -1));
	return 1;
}

int LuaNPCExports::addQuest(lua_State *luaVm) {
	getNPC(luaVm)->getPlayer()->quests->addQuest(lua_tointeger(luaVm, -1), getNPC(luaVm)->getNpcID());
	return 1;
}

int LuaNPCExports::endQuest(lua_State *luaVm) {
	getNPC(luaVm)->getPlayer()->quests->finishQuest(lua_tointeger(luaVm, -1), getNPC(luaVm)->getNpcID());
	return 1;
}

int LuaNPCExports::end(lua_State *luaVm) {
	getNPC(luaVm)->end();
	return 1;
}
