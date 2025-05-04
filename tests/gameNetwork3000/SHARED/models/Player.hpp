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
	std::vector<Worm_Segment> body;
	float rad = 1;
public:
	static boost::uuids::uuid generatorUUID() {
		static boost::uuids::random_generator generator;
		return generator();
	}
	Player(const std::string& name = "some random amogus123 who lives in street Freddy Fasber123",
		std::optional<sf::IpAddress> ip = std::optional<sf::IpAddress>(sf::IpAddress::LocalHost),
		unsigned short port = 222 * 3,
		boost::uuids::uuid id = generatorUUID(),
		float rad = 1) : name(name), id(id), ip(ip), port(port), rad(rad) {}
	
};
