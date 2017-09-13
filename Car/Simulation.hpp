#pragma once
#include "Car.hpp"
#include <SFML\Graphics.hpp>
#include <vector>
#include <random>


class Simulation : public sf::Drawable
{
public:
	Simulation();
	~Simulation();

	void update(sf::Time delta_time);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void createScene();

private:
	Car* m_car;
	std::vector<sf::RectangleShape> m_obs;

};

