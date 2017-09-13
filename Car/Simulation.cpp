#include "Simulation.hpp"
#include <iostream>
using namespace std;
using namespace sf;


Simulation::Simulation()
{
	createScene();

	m_car = new Car(true);
	m_car->setPosition(0, 0);
	m_car->setOrigin(m_car->getOrigin());

	m_car->move(100, 200);
}


Simulation::~Simulation()
{
	delete m_car;
}

void Simulation::update(sf::Time delta_time)
{

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<>dis(-45, 45);
	uniform_int_distribution<>speed(80, 500);

	//	m_car->moveForward(delta_time, speed(gen), dis(gen));
	m_car->moveForward(delta_time, 100, 0);
	m_car->update(m_obs, m_obs.back(), m_obs[3]);
}

void Simulation::draw(sf::RenderTarget & target, sf::RenderStates states) const
{

	for (auto x : m_obs)
		target.draw(x);

	target.draw(*m_car);

}

void Simulation::createScene()
{
	RectangleShape a, b, c, d, tmp;
	a.setSize({ 5,580 });
	b.setSize({ 5,580 });
	c.setSize({ 780,5 });
	d.setSize({ 780,5 });

	a.setPosition({ 10,10 });
	c.setPosition({ 10,10 });
	b.setPosition({ 785,10 });
	d.setPosition({ 10,585 });

	a.setFillColor(Color::Red);
	b.setFillColor(Color::Blue);
	c.setFillColor(Color::Cyan);
	d.setFillColor(Color::Yellow);

	tmp.setFillColor(Color::Green);
	tmp.setSize({ 200,200 });
	tmp.setPosition({ 300,200 });
	m_obs.push_back(a);
	
	m_obs.push_back(c);
	m_obs.push_back(d);
	m_obs.push_back(tmp);
	m_obs.push_back(b);
}
