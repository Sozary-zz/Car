#include "car.hpp"
#include <iostream>
using namespace sf;
using namespace std;

Car::Car(bool draw_Sensorss) :m_drawings(draw_Sensorss)
{
	m_shape.setSize(Vector2f(100, 50));
	m_shape.setFillColor(Color::Red);

	m_sensors = new Sensors*[5];

	initSensors(m_sensors[3], Vector2f(70, 5), -126.f, true);
	initSensors(m_sensors[0], Vector2f(85, 5), -108.f, true);
	initSensors(m_sensors[1], Vector2f(85, 20), -90.f, true);
	initSensors(m_sensors[2], Vector2f(85, 35), -72.f, true);
	initSensors(m_sensors[4], Vector2f(70, 35), -54.f, true);
}

Car::~Car()
{
	for (int i = 0; i < 5; ++i)
		delete m_sensors[i];
	delete m_sensors;
}

sf::Vector2f Car::getOrigin()
{
	return Vector2f(50, 25);
}

void Car::update(const std::vector<sf::RectangleShape>& obstacles, RectangleShape a, RectangleShape b)
{
	bool hit = false;
	RectangleShape ob;
	for (const auto & x : obstacles) {

		hit = m_sensors[2]->updateCollision(x, this->getTransform());

		if (hit)
		{
			ob = x;
			break;
		}

	}
	if (hit)
		while (m_sensors[2]->updateCollision(ob, this->getTransform()) && m_sensors[2]->getSensorSize() > 10)
			m_sensors[2]->handleHit(hit);

	else
		while (!m_sensors[2]->updateCollision(ob, this->getTransform()) && m_sensors[2]->getSensorSize() < 100)
			m_sensors[2]->handleHit(hit);










}

void Car::moveForward(Time elaspded_time, double speed, float angle)
{
	this->rotate(angle);
	this->move(cos(this->getRotation()*3.14159265 / 180) * speed*elaspded_time.asSeconds(), sin(this->getRotation()*3.14159265 / 180) * speed*elaspded_time.asSeconds());
}


void Car::initSensors(Sensors * & sensors, sf::Vector2f init_pos, float angle, bool debug)
{
	sensors = new Sensors(m_shape.getPosition() + init_pos, angle, debug);
	sensors->setPosition(m_shape.getPosition() + init_pos + Vector2f(5, 5)); // + vector (half of the size of a Sensors)
	sensors->setOrigin(m_shape.getPosition() + init_pos + Vector2f(5, 5));
	sensors->setRotation(angle);
}

void Car::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(m_shape, states);
	for (int i = 0; i < 5; ++i)
		target.draw(*m_sensors[i], states);

}
