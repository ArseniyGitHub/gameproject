#pragma once

class GameObject {
private:
	
public:
	virtual void update() = 0;
	virtual void draw() const = 0;
	enum Type {
		gameObject,
		entity,
        staticObject,
		// ---------
		count
	};
	Type type;
	
};