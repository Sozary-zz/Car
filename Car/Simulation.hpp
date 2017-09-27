#pragma once
#include "Car.hpp"
#include <SFML\Graphics.hpp>
#include <vector>
#include <random>
#include "Genetic.hpp"

#define SIMULATION_TIME 10.f
#define SPEED 1000.f


class Simulation : public sf::Drawable, protected Genetic
{
public:
	Simulation();
	void begin();
	~Simulation();

	void update(sf::Time delta_time);
	float getMaxFitness() const;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void createScene();
	void createPopulation();
	double computeFitness( Car* c) ;
	void clearPopulation();

private:
	std::vector<Car*> m_cars;
	std::vector<sf::RectangleShape> m_obs;
	sf::RectangleShape m_spawn_point;
	sf::RectangleShape m_end;
	sf::Clock m_clock;

	bool m_end_simulation;
	float m_max_fit;

	float m_base_dist;
};

