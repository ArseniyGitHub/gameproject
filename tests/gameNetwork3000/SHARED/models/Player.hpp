#pragma once
#include <SFML/Network.hpp>
#include "../MATH/math.hpp"
#include "Segment.hpp"
#include <vector>
#include <memory>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

class Player {
	std::string name;
	boost::uuids::uuid id;
	std::optional<sf::IpAddress> ip;
	unsigned short port;
	
public:
	float rad = 1;
	std::vector<Worm_Segment> body;
	static boost::uuids::uuid generatorUUID() {
		static boost::uuids::random_generator generator;
		return generator();
	}
	Player(const std::string& name = "some random amogus123 who lives in street Freddy Fasber123",
		std::optional<sf::IpAddress> ip = std::optional<sf::IpAddress>(sf::IpAddress::LocalHost),
		unsigned short port = 222 * 3,
		boost::uuids::uuid id = generatorUUID(),
		float rad = 1) : name(name), id(id), ip(ip), port(port), rad(rad) {}
	const std::string& getName() const { return name; }
	const boost::uuids::uuid& getUUID() const { return id; }
	void setUUID(const boost::uuids::uuid& id) { this->id = id; }
	void setName(const std::string& str) { name = str; }
	const std::optional<sf::IpAddress>& getIP() const { return ip; }
	void setIP(const sf::IpAddress& nip) { ip = nip; }
	const unsigned short getPort() const { return port; }
	void setPort(const unsigned short port) { this->port = port; }
};
