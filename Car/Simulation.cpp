#include "Simulation.hpp"
#define L m_obs.back()
#include <iostream>
using namespace std;
using namespace sf;


Simulation::Simulation() :
	Genetic(POPULATION_SIZE), m_end_simulation(false), m_max_fit(0.f)
{
	createScene();
	createPopulation();
}

void Simulation::begin()
{
	m_clock.restart();
}

Simulation::~Simulation()
{
	for (auto&x : m_cars)
		delete x;
}

void Simulation::update(sf::Time delta_time)
{
	int blocked_count = 0;


	if (m_clock.getElapsedTime().asSeconds() <= SIMULATION_TIME && !m_end_simulation) {
		blocked_count = 0;
		for (int i = 0; i < POPULATION_SIZE; ++i) {
			if (m_cars[i]->canMove()) {
				m_cars[i]->update(m_obs);
				try {
					vector<float> speed_and_rotation = m_population[i]->network->setInputs(m_cars[i]->getSensorsData());
					m_cars[i]->moveForward(delta_time, speed_and_rotation[0] * SPEED, speed_and_rotation[1]);
					m_population[i]->fitness = this->computeFitness(m_cars[i]);
					if (m_population[i]->fitness > m_max_fit)
						m_max_fit = m_population[i]->fitness;
				}
				catch (string e) {
					cout << e << endl;
				}
			}
			else
				blocked_count++;
		}
		evolve();
		if (blocked_count == POPULATION_SIZE)
			m_end_simulation = true;
	}
	else {

		clearPopulation();
		createPopulation();

		m_clock.restart();
		m_end_simulation = false;
	}

}

float Simulation::getMaxFitness() const
{
	return m_max_fit;
}

void Simulation::draw(sf::RenderTarget & target, sf::RenderStates states) const
{

	for (auto x : m_obs)
		target.draw(x);

	for (auto&x : m_cars)
		target.draw(*x);

	target.draw(m_spawn_point);
	target.draw(m_end);

	if (DRAW_NEURAL_NETWORK)
		target.draw(*m_population.front()->network);
}

void Simulation::createScene()
{
#pragma region Border
	RectangleShape a, b, c, d;
	a.setSize({ 5,620 });
	b.setSize({ 5,625 });
	c.setSize({ 1175,5 });
	d.setSize({ 1175,5 });

	a.setPosition({ 10,10 });
	c.setPosition({ 10,10 });
	b.setPosition({ 1185,10 });
	d.setPosition({ 10,630 });

	a.setFillColor(Color::Red);
	b.setFillColor(Color::Blue);
	c.setFillColor(Color::Cyan);
	d.setFillColor(Color::Yellow);

	m_obs.push_back(a);
	m_obs.push_back(c);
	m_obs.push_back(d);
	m_obs.push_back(b);
#pragma endregion
#pragma region Circuit
	m_obs.push_back(RectangleShape());
	L.setFillColor(Color::Red);
	L.setOutlineThickness(3.f);
	L.setOutlineColor(Color::Magenta);
	L.setPosition({ 20,20 });
	L.setSize({ 400,20 });

	m_obs.push_back(RectangleShape());
	L.setFillColor(Color::Red);
	L.setOutlineThickness(3.f);
	L.setOutlineColor(Color::Magenta);
	L.setPosition(400, 20);
	L.setSize({ 20,600 });

	m_obs.push_back(RectangleShape());
	L.setFillColor(Color::Red);
	L.setOutlineThickness(3.f);
	L.setOutlineColor(Color::Magenta);
	L.setPosition(200, 200);
	L.setSize({ 20,420 });

	m_obs.push_back(RectangleShape());
	L.setFillColor(Color::Red);
	L.setOutlineThickness(3.f);
	L.setOutlineColor(Color::Magenta);
	L.setPosition(20, 200);
	L.setSize({ 200,20 });
#pragma endregion

	m_spawn_point.setFillColor(Color::Yellow);
	m_spawn_point.setOutlineThickness(3.f);
	m_spawn_point.setOutlineColor(Color::Cyan);
	m_spawn_point.setPosition({ 70,120 });
	m_spawn_point.setSize({ 5,5 });

	m_end.setFillColor(Color::Yellow);
	m_end.setOutlineThickness(3.f);
	m_end.setOutlineColor(Color::Red);
	m_end.setPosition({ 300,600 });
	m_end.setSize({ 5,5 });

	m_base_dist = NeuralNetwork::getDistance(m_spawn_point.getPosition(), m_end.getPosition());
}

void Simulation::createPopulation()
{
	for (int i = 0; i < POPULATION_SIZE; ++i) {
		m_cars.push_back(new Car);
		m_cars.back()->setScale({ .8f,.8f });
		m_cars.back()->setOrigin(m_cars.back()->getPosition() + Vector2f(.8f*BASIC_CAR_SIZE_X, .8f*BASIC_CAR_SIZE_Y)*.5f);
		m_cars.back()->setPosition(m_spawn_point.getPosition());

		m_population.push_back(new Chromosom);
		m_population.back()->fitness = 0;
		m_population.back()->network = new NeuralNetwork({ 0,0,0,0,0 }, 2, 4, 2, 800, 600, DRAW_NEURAL_NETWORK);
		m_population.back()->network->move({ 350,50 });
	}


}

double Simulation::computeFitness(Car * c)
{
	return m_base_dist - NeuralNetwork::getDistance(c->getPosition(), m_end.getPosition());
}

void Simulation::clearPopulation()
{
	for (auto x : m_cars)
		delete x;

	m_cars.clear();
}
