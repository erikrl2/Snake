#pragma once

#include "Application.h"

#include <vector>
#include <queue>

namespace Snake {

	inline constexpr float cellSize = 25;

	struct Block
	{
		sf::RectangleShape Rect;
		sf::Vector2i Pos;

		Block(sf::Color color)
		{
			Rect.setFillColor(color);
			Rect.setSize(sf::Vector2f(cellSize - 2, cellSize - 2));
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
		void Reset();
		void Restart();
		void UpdateSnakeDir();
		void SetBlockPos(Block& block, sf::Vector2i gridPos);
		void Draw();
	private:
		sf::RenderWindow* window = nullptr;

		const sf::Vector2u windowSize = { 800, 450 };
		const sf::Vector2i gridSize = { int(windowSize.x / cellSize), int(windowSize.y / cellSize) };

		sf::Font font;
		sf::Text fpsCount;

		std::vector<Block> snake;
		sf::Vector2i snakeDir = { 1, 0 };
		float snakeUpdateTime = .08f;

		std::queue<sf::Vector2i> dirBuffer;

		Block apple = { sf::Color(240, 58, 23) };

		bool playing = true;
	};

}

