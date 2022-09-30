#pragma once

#include "Application.h"

#include <vector>

namespace Snake {

	struct Block;

	class Game : public Application
	{
	public:
		Game(sf::RenderWindow* renderWindow);
		virtual ~Game() = default;

		virtual void Update(sf::Time ts) override;
		virtual void OnEvent(sf::Event& event) override;
	private:
		void GetSnakeDir();
		void UpdateSnake();
		void AddTail();
		void MoveBlock(Block& block, sf::Vector2i gridPos);
		void Draw();
	private:
		sf::RenderWindow* window = nullptr;

		const sf::Vector2u windowSize = { 800, 450 };

		std::vector<Block> snake;
	};

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

