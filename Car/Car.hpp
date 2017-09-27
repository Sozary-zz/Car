#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "Sensor.hpp"

#define BASIC_CAR_SIZE_X 100
#define BASIC_CAR_SIZE_Y 50

class Car : public sf::Drawable, public sf::Transformable
{
public:
	Car(bool draw_Sensorss = false);
	~Car();
	sf::Vector2f getOrigin();
	void update(const std::vector<sf::RectangleShape>& obstacles);
	void moveForward(sf::Time elaspded_time,double speed, float angle);
	std::vector<float> getSensorsData() const;
	bool canMove() const;


private:
	sf::RectangleShape m_shape;
	Sensors **m_sensors;

	bool m_drawings;

private:
	struct SensorCollision {
		bool collision;
		sf::RectangleShape object;
	};
	bool m_end;
	
private:
	void initSensors(Sensors * & Sensors, sf::Vector2f init_pos, float angle, bool debug);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
