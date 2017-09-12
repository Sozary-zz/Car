#include "core.hpp"
#define GAME_FPS 60.f


using namespace std;
using namespace sf;

Core::Core(int w, int h, const string& title) :
	m_window(VideoMode(w, h), title, Style::Close), TimePerFrame(seconds(1.f / (float)GAME_FPS))
{
	m_car = new Car(true);
	m_car->setPosition(0, 0);
	m_car->setOrigin(m_car->getOrigin());

	m_car->move(200, 200);

}


void Core::run()
{
	Clock clock;

	Time timeSinceLastUpdate;
	timeSinceLastUpdate = Time::Zero;

	while (m_window.isOpen())
	{

		processEvent();
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			processEvent();
			update(TimePerFrame);
		}
		render();
	}
}

void Core::processEvent()
{

	Event event;

	while (m_window.pollEvent(event))
	{
		switch (event.type)
		{
		case Event::MouseMoved:

			break;
		case Event::KeyReleased:
			break;
		case Event::KeyPressed:
			switch (event.key.code)
			{

			case Keyboard::Left:
				m_car->rotate(2);
				break;
			case Keyboard::Right:
				m_car->rotate(-2);
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
	m_car->update(m_obs);
}


void Core::render()
{
	m_window.clear();

	for (auto x : m_obs)
		m_window.draw(x);

	m_window.draw(*m_car);
	m_window.display();
}
