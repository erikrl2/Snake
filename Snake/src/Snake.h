#pragma once

#include <SFML/Graphics.hpp>

namespace Snake {

	struct Block
	{
		sf::RectangleShape Rect;
		sf::Vector2i Pos;
		sf::Vector2i Dir;

		Block(sf::Color color)
		{
			Rect.setFillColor(color);
			Rect.setSize(sf::Vector2f(25, 25));
		}

		operator const sf::Drawable& () { return Rect; }
		bool operator == (const Block& other) { return Pos == other.Pos; }
		bool operator != (const Block& other) { return !(*this == other); }
	};
}
