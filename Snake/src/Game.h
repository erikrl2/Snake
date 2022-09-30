#pragma once

#include "Application.h"

#include "Snake.h"

#include <vector>

namespace Snake {

	class Game : public Application
	{
	public:
		Game(sf::RenderWindow* renderWindow);
		virtual ~Game() = default;

		virtual void Update(sf::Time ts) override;
		virtual void OnEvent(sf::Event& event) override;
	private:
		void GetDir();
		void UpdateSnake();
		void AddTail();
		void MoveBlock(Block& block, sf::Vector2i gridPos);
	private:
		sf::RenderWindow* window = nullptr;

		const sf::Vector2u windowSize = { 800, 450 };

		std::vector<Block> snake;
	};

}

