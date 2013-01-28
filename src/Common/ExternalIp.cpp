/*
Copyright (C) 2008-2013 Vana Development Team

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
#include "ExternalIp.h"
#include "PacketCreator.h"
#include "PacketReader.h"
#include <stdexcept>

ExternalIp::ExternalIp(const std::string &addr, uint32_t ipv4SubnetMask) :
	Ip(addr, Ip::Type::Ipv4),
	m_ipv4SubnetMask(ipv4SubnetMask)
{
}

ExternalIp::ExternalIp(uint32_t ipv4, uint32_t ipv4SubnetMask) :
	Ip(ipv4),
	m_ipv4SubnetMask()
{
}

bool ExternalIp::tryMatchIpToSubnet(const Ip &test, Ip &result) const {
	if (test.m_type != m_type) throw std::invalid_argument("IP type must match the external IP type");

	if (m_type == Ip::Type::Ipv4) {
		if ((m_ipv4 & m_ipv4SubnetMask) == (test.m_ipv4 & m_ipv4SubnetMask)) {
			result = Ip(m_ipv4);
			return true;
		}
		return false;
	}

	return false;
}

void ExternalIp::write(PacketCreator &packet) const {
	packet.addClass<Ip::Type>(m_type);
	if (m_type == Ip::Type::Ipv4) {
		packet.add<uint32_t>(m_ipv4);
		packet.add<uint32_t>(m_ipv4SubnetMask);
	}
}

void ExternalIp::read(PacketReader &packet) {
	m_type = packet.getClass<Ip::Type>();
	if (m_type == Ip::Type::Ipv4) {
		m_ipv4 = packet.get<uint32_t>();
		m_ipv4SubnetMask = packet.get<uint32_t>();
	}
}