#pragma once

#include "Application.h"

#include <vector>

namespace Snake {

	struct Block
	{
		sf::RectangleShape Rect;
		sf::Vector2i Pos;

		Block(sf::Color color)
		{
			Rect.setFillColor(color);
			Rect.setSize(sf::Vector2f(25, 25));
		}

		operator const sf::Drawable& () { return Rect; }
	};

	class Game : public Application
	{
	public:
		Game(sf::RenderWindow* renderWindow);
		virtual ~Game() = default;

		virtual void Update(sf::Time ts) override;
		virtual void OnEvent(sf::Event& event) override;
	private:
		void OnKeyPressed(sf::Event& event);
		void UpdateSnake();
		void SpawnApple();
		void MoveBlock(Block& block, sf::Vector2i gridPos);
		void Restart();
		void Draw();
	private:
		sf::RenderWindow* window = nullptr;

		const sf::Vector2u windowSize = { 800, 450 };
		const sf::Vector2f cellSize = { 25, 25 };
		const sf::Vector2i gridSize = { int(windowSize.x / cellSize.x), int(windowSize.y / cellSize.y) };

		sf::Font font;
		sf::Text fpsCount;

		std::vector<Block> snake;
		sf::Vector2i snakeDir = { 1, 0 };

		Block apple = { sf::Color(240, 58, 23) };

		bool playing = true;
	};

}

