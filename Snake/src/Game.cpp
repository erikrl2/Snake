#include "Game.h"

#include <iostream>

namespace Snake {

	Game::Game(sf::RenderWindow* renderWindow)
		: window(renderWindow)
	{
		window->create(sf::VideoMode(windowSize.x, windowSize.y), "Snake", sf::Style::Close);
		window->setVerticalSyncEnabled(true);

		sf::Uint8 iconColor[] = { 152, 225, 112, 255 };
		window->setIcon(1, 1, iconColor);

		Block head({ 255, 255, 255 });
		head.Dir.x = 1;
		MoveBlock(head, { 5, 8 });
		snake.push_back(head);
		
		for (int i = 0; i < 5; i++)
			AddTail();
	}

	void Game::Update(sf::Time ts)
	{
		GetSnakeDir();

		static float t = 0;
		if (t >= .1f)
		{
			UpdateSnake();
			t = 0;
		}
		t += ts.asSeconds();

		Draw();
	}

	void Game::OnEvent(sf::Event& event)
	{
		if (event.type == sf::Event::Closed)
		{
			window->close();
		}
	}

	void Game::GetSnakeDir()
	{
		sf::Vector2i& dir = snake[0].Dir;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && dir.y != 1)
			dir = { 0, -1 };
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && dir.y != -1)
			dir = { 0, 1 };
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && dir.x != -1)
			dir = { 1, 0 };
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && dir.x != 1)
			dir = { -1, 0 };
	}

	void Game::UpdateSnake()
	{
		Block& head = snake[0];
		for (auto block = snake.rbegin(); block != --snake.rend(); block++)
		{
			if (*block == head)
				std::cout << "Game Over\n";

			MoveBlock(*block, (block + 1)->Pos);
		}
		MoveBlock(head, head.Pos += head.Dir);
	}

	void Game::AddTail()
	{
		Block tail = *snake.rbegin();
		MoveBlock(tail, tail.Pos - tail.Dir);
		tail.Rect.setFillColor({ 152, 225, 112 });
		snake.push_back(tail);
	}

	void Game::MoveBlock(Block& block, sf::Vector2i gridPos)
	{
		static const sf::Vector2i gridSize = { int(windowSize.x / 25.f), int(windowSize.y / 25.f) };
		if (gridPos.x < 0) gridPos.x = gridSize.x - 1;
		if (gridPos.y < 0) gridPos.y = gridSize.y - 1;
		const sf::Vector2i coord(gridPos.x % gridSize.x, gridPos.y % gridSize.y);

		block.Pos = coord;
		block.Rect.setPosition(coord.x / (float)gridSize.x * windowSize.x,
			coord.y / (float)gridSize.y * windowSize.y);
	}

	void Game::Draw()
	{
		window->clear();

		for (Block& block : snake)
			window->draw(block);

		window->display();
	}

}

int main(int argc, char** argv)
{
	Application::Launch<Snake::Game>();
	return 0;
}
