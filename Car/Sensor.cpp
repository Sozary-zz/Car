#include "Sensor.hpp"
#include <iostream>

using namespace std;
using namespace sf;

Sensors::Sensors(const sf::Vector2f & position, const float angle, bool debug) :m_debug(debug),
m_link(LinesStrip, 2), m_range(100)
{

	m_shape.setSize(Vector2f(10, 10));
	m_shape.setPosition(position);

	m_link[0].position = position + Vector2f(5, 5);
	m_link[1].position = position + Vector2f(0, m_range) + Vector2f(5, 5);

	m_target.setRadius(5.f);
	m_target.setPosition(position + Vector2f(0, m_range));
}

bool Sensors::updateCollision(const sf::RectangleShape & obstacle, const sf::Transform& transformations)
{
	auto  x = transformations * this->getTransform();

	return x.transformRect(m_target.getGlobalBounds()).intersects(obstacle.getGlobalBounds());
	/*
			if (m_range > 10)
			{
				cout << "k" ;
				m_target.setPosition(m_shape.getPosition() + Vector2f(0, --m_range));
				return true;
			}
	}
	else {
			if (m_range < 100)
			{
				cout << "l";
				m_target.setPosition(m_shape.getPosition() + Vector2f(0, ++m_range));
			}



		//
	}*/
	//if (!x.transformRect(m_target.getGlobalBounds()).intersects(obstacle.getGlobalBounds()) && m_range < 100)
	//	
	// HERE ALPHA MIGHT BE REMOVED
	/*while (
		(x.transformRect(m_target.getGlobalBounds()).intersects(obstacle.getGlobalBounds()) ||
			x.transformRect(m_link.getBounds()).intersects(obstacle.getGlobalBounds())) // ALPHA
		&& m_range > 10)
	{
		if (!disp)
		{
			cout << m_range << endl;
			disp = true;
		}


		m_target.setPosition(m_shape.getPosition() + Vector2f(0, --m_range));
		m_link[1].position = m_shape.getPosition() + Vector2f(0, m_range) + Vector2f(5, 5);


	}
	cout << "=> "<<m_range << endl;
	while (!
		(x.transformRect(m_target.getGlobalBounds()).intersects(obstacle.getGlobalBounds()) ||
			x.transformRect(m_link.getBounds()).intersects(obstacle.getGlobalBounds())) // ALPHA
		&& m_range < 100)
	{

		m_target.setPosition(m_shape.getPosition() + Vector2f(0, ++m_range));
		m_link[1].position = m_shape.getPosition() + Vector2f(0, m_range) + Vector2f(5, 5);
	}*/



}

void Sensors::handleHit(bool hit)
{
	if (hit)
	{

		if (m_range > 10) {
			m_target.setPosition(m_shape.getPosition() + Vector2f(0, --m_range));
			m_link[1].position = m_shape.getPosition() + Vector2f(0, m_range) + Vector2f(5, 5);
		}
	}
	else
		if (m_range < 100) {
			m_target.setPosition(m_shape.getPosition() + Vector2f(0, ++m_range));
			m_link[1].position = m_shape.getPosition() + Vector2f(0, m_range) + Vector2f(5, 5);
		}
}

int Sensors::getSensorSize() const
{
	return m_range;
}


Sensors::~Sensors()
{
}

void Sensors::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	if (m_debug)
	{

		target.draw(m_link, states);
		target.draw(m_target, states);
	}

	target.draw(m_shape, states);

}


