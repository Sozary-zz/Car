#include "car.hpp"
#include <iostream>
using namespace sf;
using namespace std;

Car::Car(bool draw_Sensorss) :m_drawings(draw_Sensorss),m_end(false)
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

void Car::update(const std::vector<sf::RectangleShape>& obstacles)
{

	bool hit = false;
	RectangleShape ob;

	SensorCollision collisions[5];

	for (const auto & x : obstacles)
		if (this->getTransform().transformRect(m_shape.getGlobalBounds()).intersects(x.getGlobalBounds()))
			m_end = true;

	for (int i = 0; i < 5; ++i) {
		collisions[i].collision = false;
		for (const auto & x : obstacles) {
			collisions[i].collision = m_sensors[2]->updateCollision(x, this->getTransform());
			if (collisions[i].collision)
			{
				collisions[i].object = x;
				break;
			}

		}
	}

	for (int i = 0; i < 5; ++i) {
		if (collisions[i].collision)
			while (m_sensors[i]->updateCollision(collisions[i].object, this->getTransform()) && m_sensors[i]->getSensorSize() > 10)
				m_sensors[i]->handleHit(collisions[i].collision);

		else
			while (!m_sensors[i]->updateCollision(collisions[i].object, this->getTransform()) && m_sensors[i]->getSensorSize() < 100)
				m_sensors[i]->handleHit(collisions[i].collision);

	}
}

void Car::moveForward(Time elaspded_time, double speed, float angle)
{
	this->rotate(angle);
	this->move(cos(this->getRotation()*3.14159265 / 180) * speed*elaspded_time.asSeconds(), sin(this->getRotation()*3.14159265 / 180) * speed*elaspded_time.asSeconds());
}

std::vector<float> Car::getSensorsData() const
{
	vector<float> res;
	
	for (int i = 0; i < 5; ++i)
		res.push_back(m_sensors[i]->getSensorSize());

	return res;
}

bool Car::canMove() const
{
	return !m_end;
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
