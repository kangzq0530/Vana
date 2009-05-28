/*
Copyright (C) 2008-2009 Vana Development Team

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
#include "Levels.h"
#include "GameConstants.h"
#include "LevelsPacket.h"
#include "PacketReader.h"
#include "Player.h"
#include "PlayersPacket.h"
#include "Randomizer.h"
#include "Skills.h"
#include <boost/lexical_cast.hpp>
#include <string>

using std::string;

uint32_t Levels::exps[Stats::PlayerLevels] = {15, 34, 57, 92, 135, 372, 560, 840, 1242, 1716, 2360, 3216, 4200,
	5460, 7050, 8840, 11040, 13716, 16680, 20216, 24402, 28980, 34320, 40512, 47216, 54900,
	63666, 73080, 83720, 95700, 108480, 122760, 138666, 155540, 174216, 194832, 216600, 240500,
	266682, 294216, 324240, 356916, 391160, 428280, 468450, 510420, 555680, 604416, 655200,
	709716, 748608, 789631, 832902, 878545, 926689, 977471, 1031036, 1087536, 1147132, 1209994,
	1276301, 1346242, 1420016, 1497832, 1579913, 1666492, 1757815, 1854143, 1955750, 2062925,
	2175973, 2295216, 2420993, 2553663, 2693603, 2841212, 2996910, 3161140, 3334370, 3517093,
	3709829, 3913127, 4127566, 4353756, 4592341, 4844001, 5109452, 5389449, 5684790, 5996316,
	6324914, 6671519, 7037118, 7422752, 7829518, 8258575, 8711144, 9188514, 9692044, 10223168,
	10783397, 11374327, 11997640, 12655110, 13348610, 14080113, 14851703, 15665576, 16524049,
	17429566, 18384706, 19392187, 20454878, 21575805, 22758159, 24005306, 25320796, 26708375,
	28171993, 29715818, 31344244, 33061908, 34873700, 36784778, 38800583, 40926854, 43169645,
	45535341, 48030677, 50662758, 53439077, 56367538, 59456479, 62714694, 66151459, 69776558,
	73600313, 77633610, 81887931, 86375389, 91108760, 96101520, 101367883, 106992842, 112782213,
	118962678, 125481832, 132358236, 139611467, 147262175, 155332142, 163844343, 172823012,
	182293713, 192283408, 202820538, 213935103, 225658746, 238024845, 251068606, 264827165,
	279339639, 294647508, 310794191, 327825712, 345790561, 364739883, 384727628, 405810702,
	428049128, 451506220, 476248760, 502347192, 529875818, 558913012, 589541445, 621848316,
	655925603, 691870326, 729784819, 769777027, 811960808, 856456260, 903390063, 952895838,
	1005114529, 1060194805, 1118293480, 1179575962, 1244216724, 1312399800, 1384319309,
	1460180007, 1540197871, 1624600714, 1713628833, 1807535693, 1906558648, 2011069705, 2121276324};

void Levels::giveExp(Player *player, uint32_t exp, bool inChat, bool white) {
	if (player->getLevel() >= Stats::PlayerLevels) // Do not give EXP to characters of max level or over
		return;
	uint32_t cexp = player->getExp() + exp;
	if (exp != 0)
		LevelsPacket::showEXP(player, exp, white, inChat);
	uint8_t level = player->getLevel();
	if (cexp >= exps[level - 1]) {
		int16_t apgain = 0;
		int16_t spgain = 0;
		int16_t hpgain = 0;
		int16_t mpgain = 0;
		uint8_t levelsgained = 0;
		uint8_t levelsmax = ChannelServer::Instance()->getMaxMultiLevel();
		int16_t job = player->getJob() / 100;
		int16_t intt = player->getInt() / 10;
		int16_t x = 0; // X value for Improving *P Increase skills, cached, only needs to be set once
		while (cexp >= exps[level - 1] && levelsgained < levelsmax) {
			cexp -= exps[player->getLevel() - 1];
			level++;
			levelsgained++;
			apgain += 5;
			switch (job) {
				case 0: // Beginner
					hpgain += levelHp(Stats::BaseHp::Beginner);
					mpgain += levelMp(Stats::BaseMp::Beginner, intt);
					break;
				case 1: // Warrior
					if (levelsgained == 1 && player->getSkills()->getSkillLevel(Jobs::Swordsman::ImprovedMaxHpIncrease) > 0)
						x = getX(player, Jobs::Swordsman::ImprovedMaxHpIncrease);
					hpgain += levelHp(Stats::BaseHp::Warrior, x);
					mpgain += levelMp(Stats::BaseMp::Warrior, intt);
					break;
				case 2: // Magician
					if (levelsgained == 1 && player->getSkills()->getSkillLevel(Jobs::Magician::ImprovedMaxMpIncrease) > 0)
						x = getX(player, Jobs::Magician::ImprovedMaxMpIncrease);
					hpgain += levelHp(Stats::BaseHp::Magician);
					mpgain += levelMp(Stats::BaseMp::Magician, 2 * x + intt);
					break;
				case 3: // Bowman
					hpgain += levelHp(Stats::BaseHp::Bowman);
					mpgain += levelMp(Stats::BaseMp::Bowman, intt);
					break;
				case 4: // Thief
					hpgain += levelHp(Stats::BaseHp::Thief);
					mpgain += levelMp(Stats::BaseMp::Thief, intt);
					break;
				case 5: // Pirate
					if (levelsgained == 1 && player->getSkills()->getSkillLevel(Jobs::Infighter::ImproveMaxHp) > 0)
						x = getX(player, Jobs::Infighter::ImproveMaxHp);
					hpgain += levelHp(Stats::BaseHp::Pirate, x);
					mpgain += levelMp(Stats::BaseMp::Pirate, intt);
					break;
				default: // GM
					hpgain += Stats::BaseHp::Gm;
					mpgain += Stats::BaseMp::Gm;
			}
			if (player->getJob() > 0)
				spgain += 3;
			if (level >= Stats::PlayerLevels) { // Do not let people level past the level 200 cap
				cexp = 0;
				break;
			}
		}

		if (cexp >= exps[level - 1]) { // If the desired number of level ups have passed and they're still above, set it to where it should be
			cexp = exps[level - 1] - 1;
		}

		if (levelsgained) { // Check if the player has leveled up at all, it is possible that the user hasn't leveled up if multi-level limit is 0
			player->modifyRMHp(hpgain);
			player->modifyRMMp(mpgain);
			player->setLevel(level);
			player->setAp(player->getAp() + apgain);
			player->setSp(player->getSp() + spgain);
			// Let hyperbody remain on if on during a level up, as it should
			if (player->getActiveBuffs()->hasHyperBody()) {
				int32_t skillid = player->getActiveBuffs()->getHyperBody();
				uint8_t hblevel = player->getActiveBuffs()->getActiveSkillLevel(skillid);
				player->setHyperBody(Skills::skills[skillid][hblevel].x, Skills::skills[skillid][hblevel].y);
			}
			else {
				player->setMHp(player->getRMHp());
				player->setMMp(player->getRMMp());
			}
			player->setHp(player->getMHp());
			player->setMp(player->getMMp());
			player->setLevelDate();
			if (player->getLevel() == Stats::PlayerLevels && !player->isGm()) {
				string message;
				message = "[Congrats] ";
				message += player->getName();
				message += " has reached Level ";
				message += boost::lexical_cast<string>(Stats::PlayerLevels);
				message += "! Congratulate ";
				message += player->getName();
				message += " on such an amazing achievement!";
				PlayersPacket::showMessageWorld(message, 6);
			}
		}
	}
	player->setExp(cexp);
}

void Levels::addStat(Player *player, PacketReader &packet) {
	packet.skipBytes(4);
	int32_t type = packet.get<int32_t>();
	if (player->getAp() == 0) {
		// hacking
		return;
	}
	LevelsPacket::statOK(player);
	addStat(player, type);
}

void Levels::addStatMulti(Player *player, PacketReader &packet) {
	packet.skipBytes(4);
	uint32_t amount = packet.get<uint32_t>();

	LevelsPacket::statOK(player);

	for (uint32_t i = 0; i < amount; i++) {
		int32_t type = packet.get<int32_t>();
		int32_t value = packet.get<int32_t>();

		if (value < 0 || player->getAp() < value) {
			//hacking
			return;
		}

		addStat(player, type, static_cast<int16_t>(value)); // Prefer a single cast to countless casts/modification down the line
	}
}

void Levels::addStat(Player *player, int32_t type, int16_t mod, bool isreset) {
	int16_t maxstat = ChannelServer::Instance()->getMaxStats();
	bool issubtract = mod < 0;
	switch (type) {
		case Stats::Str:
			if (player->getStr() >= maxstat)
				return;
			player->setStr(player->getStr() + mod);
			break;
		case Stats::Dex:
			if (player->getDex() >= maxstat)
				return;
			player->setDex(player->getDex() + mod);
			break;
		case Stats::Int:
			if (player->getInt() >= maxstat)
				return;
			player->setInt(player->getInt() + mod);
			break;
		case Stats::Luk:
			if (player->getLuk() >= maxstat)
				return;
			player->setLuk(player->getLuk() + mod);
			break;
		case Stats::MaxHp:
		case Stats::MaxMp: {
			if (type == Stats::MaxHp && player->getRMHp() >= Stats::MaxMaxHp)
				return;
			if (type == Stats::MaxMp && player->getRMMp() >= Stats::MaxMaxMp)
				return;
			if (issubtract && player->getHpMpAp() == 0) {
				// Hacking
				return;
			}
			int16_t job = player->getJob() / 100;
			int16_t hpgain = 0;
			int16_t mpgain = 0;
			int16_t y = 0;
			switch (job) {
				case 0: // Beginner
					hpgain = apResetHp(isreset, issubtract, Stats::BaseHp::BeginnerAp);
					mpgain = apResetMp(isreset, issubtract, Stats::BaseMp::BeginnerAp);
					break;
				case 1: // Warrior
					if (player->getSkills()->getSkillLevel(Jobs::Swordsman::ImprovedMaxHpIncrease) > 0)
						y = getY(player, Jobs::Swordsman::ImprovedMaxHpIncrease);
					hpgain = apResetHp(isreset, issubtract, Stats::BaseHp::WarriorAp, y);
					mpgain = apResetMp(isreset, issubtract, Stats::BaseMp::WarriorAp);
					break;
				case 2: // Magician
					if (player->getSkills()->getSkillLevel(Jobs::Magician::ImprovedMaxMpIncrease) > 0)
						y = getY(player, Jobs::Magician::ImprovedMaxMpIncrease);
					hpgain = apResetHp(isreset, issubtract, Stats::BaseHp::MagicianAp);
					mpgain = apResetMp(isreset, issubtract, Stats::BaseMp::MagicianAp, 2 * y);
					break;
				case 3: // Bowman
					hpgain = apResetHp(isreset, issubtract, Stats::BaseHp::BowmanAp);
					mpgain = apResetMp(isreset, issubtract, Stats::BaseMp::BowmanAp);
					break;
				case 4: // Thief
					hpgain = apResetHp(isreset, issubtract, Stats::BaseHp::ThiefAp);
					mpgain = apResetMp(isreset, issubtract, Stats::BaseMp::ThiefAp);
					break;
				case 5: // Pirate
					if (player->getSkills()->getSkillLevel(Jobs::Infighter::ImproveMaxHp) > 0)
						y = getY(player, Jobs::Infighter::ImproveMaxHp);
					hpgain = apResetHp(isreset, issubtract, Stats::BaseHp::PirateAp, y);
					mpgain = apResetMp(isreset, issubtract, Stats::BaseMp::PirateAp);
					break;
				default: // GM
					hpgain = apResetHp(isreset, issubtract, Stats::BaseHp::GmAp);
					mpgain = apResetMp(isreset, issubtract, Stats::BaseMp::GmAp);
					break;
			}
			player->setHpMpAp(player->getHpMpAp() + mod);
			switch (type) {
				case Stats::MaxHp: player->modifyRMHp(hpgain); break;
				case Stats::MaxMp: player->modifyRMMp(mpgain); break;
			}
			if (player->getActiveBuffs()->hasHyperBody()) {
				int32_t skillid = player->getActiveBuffs()->getHyperBody();
				uint8_t hblevel = player->getActiveBuffs()->getActiveSkillLevel(skillid);
				player->setHyperBody(Skills::skills[skillid][hblevel].x, Skills::skills[skillid][hblevel].y);
			}
			else {
				player->setMHp(player->getRMHp());
				player->setMMp(player->getRMMp());
			}
			player->setHp(player->getHp());
			player->setMp(player->getMp());
			break;
		}
		default:
			// Hacking, one assumes
			break;
	}
	if (!isreset)
		player->setAp(player->getAp() - mod);
}

int16_t Levels::randHp() {
	return Randomizer::Instance()->randShort(Stats::BaseHp::Variation); // Max HP range per class (e.g. Beginner is 8-12)
}

int16_t Levels::randMp() {
	return Randomizer::Instance()->randShort(Stats::BaseMp::Variation); // Max MP range per class (e.g. Beginner is 6-8)
}

int16_t Levels::getX(Player *player, int32_t skillid) {
	return Skills::skills[skillid][player->getSkills()->getSkillLevel(skillid)].x;
}

int16_t Levels::getY(Player *player, int32_t skillid) {
	return Skills::skills[skillid][player->getSkills()->getSkillLevel(skillid)].y;
}

int16_t Levels::apResetHp(bool isreset, bool issubtract, int16_t val, int16_t sval) {
	return (isreset ? (issubtract ? -(sval + val + Stats::BaseHp::Variation) : val) : levelHp(val, sval));
}

int16_t Levels::apResetMp(bool isreset, bool issubtract, int16_t val, int16_t sval) {
	return (isreset ? (issubtract ? -(sval + val + Stats::BaseMp::Variation) : val) : levelMp(val, sval));
}

int16_t Levels::levelHp(int16_t val, int16_t bonus) {
	return randHp() + val + bonus;
}

int16_t Levels::levelMp(int16_t val, int16_t bonus) {
	return randMp() + val + bonus;
}