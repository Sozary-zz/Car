#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "Sensor.hpp"

class Car : public sf::Drawable, public sf::Transformable
{
public:
	Car(bool draw_Sensorss = false);
	~Car();
	sf::Vector2f getOrigin();
	void update(const std::vector<sf::RectangleShape>& obstacles, sf::RectangleShape a, sf::RectangleShape b);
	void moveForward(sf::Time elaspded_time,double speed, float angle);

private:
	sf::RectangleShape m_shape;
	Sensors **m_sensors;

	bool m_drawings;
	
private:
	void initSensors(Sensors * & Sensors, sf::Vector2f init_pos, float angle, bool debug);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
