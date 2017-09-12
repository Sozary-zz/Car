#pragma once
#include <SFML\Graphics.hpp>
#include <vector>

class Sensors: public sf::Drawable, public sf::Transformable
{
public:
	Sensors(const sf::Vector2f& position, const float angle, bool debug = false);
	void updateCollision(const sf::RectangleShape& obstacle, const sf::Transform& transformations);
	~Sensors();

private:
	sf::RectangleShape m_shape;
	sf::CircleShape m_target;
	sf::VertexArray m_link;
	bool m_debug;

	double m_range;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

