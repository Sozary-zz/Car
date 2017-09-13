#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "Simulation.hpp"

class Core
{
public:
	Core(int w, int h, const std::string& title);
	void run();

private:
	sf::RenderWindow m_window;
	const sf::Time TimePerFrame;
	
	Simulation* m_sim;



private:
	void processEvent(sf::Time);
	void update(sf::Time);
	void render();
};
