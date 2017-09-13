#include "core.hpp"
#define GAME_FPS 60.f
#include <iostream>

using namespace std;
using namespace sf;

Core::Core(int w, int h, const string& title) :
m_window(VideoMode(w, h), title, Style::Close, ContextSettings{ 0,0,8 }), TimePerFrame(seconds(1.f / (float)GAME_FPS))
{

	m_sim = new Simulation();
	
}


void Core::run()
{
	Clock clock;

	Time timeSinceLastUpdate;
	timeSinceLastUpdate = Time::Zero;

	while (m_window.isOpen())
	{

		processEvent(TimePerFrame);
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			processEvent(TimePerFrame);
			update(TimePerFrame);
		}
		render();
	}
}

void Core::processEvent(Time delta)
{


	Event event;

	while (m_window.pollEvent(event))
	{
		switch (event.type)
		{
		case Event::KeyReleased:
			break;
		case Event::KeyPressed:
			switch (event.key.code)
			{

			case Keyboard::Left:
				break;

			case Keyboard::Up:

				
				break;
			case Keyboard::Right:
			
				break;

			case Keyboard::Escape:
				m_window.close();
				break;
			}
			break;

		case Event::Closed:
			m_window.close();
			break;
		}
	}

}

void Core::update(Time deltaTime)
{
	m_sim->update(deltaTime);
}


void Core::render()
{
	m_window.clear();

	m_window.draw(*m_sim);
	m_window.display();
}
