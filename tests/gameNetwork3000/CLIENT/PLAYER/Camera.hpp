#pragma once
#include <SFML/Graphics.hpp>
#include <MATH/math.hpp>
#include <vector>
#include <memory>

class Camera : public sf::View {
	std::shared_ptr<TransformableObj> targetObject = nullptr;
	float speed = 5;
	float _zoom = 1;
	float k = 0.4;
public:
	Camera(const sf::RenderWindow& window) : View(window.getView()) {}
	void update(sf::RenderWindow& windowsIdk, float dT) {
		if (targetObject == nullptr)
			return;
		sf::Vector2f coords = targetObject->_getPosition();
		sf::Vector2f currentView = this->getCenter();
		float dist = getDistance(currentView, coords);
		sf::Vector2f normVec = getNormalizedDirection(currentView, coords, dist);
		sf::Vector2f step = normVec * speed * dist * dT;

		float idk = _zoom - targetObject->_getDimentinalK();
		if (abs(idk) > 0.00001) {
			_zoom = targetObject->_getDimentinalK();
			//this->zoom(_zoom);
			sf::Vector2f targetFraction(targetObject->_getTargetFraction());
			targetFraction.x /= windowsIdk.getSize().x * k;
			targetFraction.y /= windowsIdk.getSize().y * k;
			float zoom = std::max(targetFraction.x, targetFraction.y);
			this->zoom(zoom);
		}

		this->move(step);
		windowsIdk.setView(*this);
	}
	void setTargetObject(const std::shared_ptr<TransformableObj> from) { targetObject = from; }
	std::shared_ptr<TransformableObj> getTargetObject() { return targetObject; }
};