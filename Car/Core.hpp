#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "car.hpp"

class Core
{
public:
	Core(int w, int h, const std::string& title);
	void run();

private:
	sf::RenderWindow m_window;
	const sf::Time TimePerFrame;
	Car* m_car;
	std::vector<sf::RectangleShape> m_obs;


private:
	void processEvent();
	void update(sf::Time);
	void render();
};
