#include "Game.h"

#include <random>
#include <format>

#ifdef NDEBUG
#include <Windows.h>
#endif

namespace Snake {

	Game::Game(sf::RenderWindow* renderWindow)
		: window(renderWindow)
	{
		window->create(sf::VideoMode(windowSize.x, windowSize.y), "Snake", sf::Style::Close);
		window->setVerticalSyncEnabled(true);
		window->setKeyRepeatEnabled(false);

		sf::Uint8 iconColor[] = { 152, 225, 112, 255 };
		window->setIcon(1, 1, iconColor);

		font.loadFromFile("C:/Windows/Fonts/arial.ttf");
		fpsCount.setFont(font);
		fpsCount.setCharacterSize(14);

		Restart();
	}

	static bool turning = false;

	void Game::Update(sf::Time ts)
	{
		if (playing)
		{
			static float t = 0;
			if (t >= .1f)
			{
				t = 0;
				turning = false;

				UpdateSnake();
			}
			t += ts.asSeconds();
		}

		fpsCount.setString(std::format("{:.0f} FPS", 1.f / ts.asSeconds()));

		Draw();
	}

	void Game::OnEvent(sf::Event& event)
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window->close();
			break;
		case sf::Event::KeyPressed:
			OnKeyPressed(event);
			break;
		}
	}

	void Game::OnKeyPressed(sf::Event& event)
	{
		if (playing)
		{
			if (turning)
				return;
			turning = true;

			sf::Keyboard::Key key = event.key.code;

			bool b1 = snakeDir.x != 1 && snakeDir.x != -1;
			bool b2 = snakeDir.y != 1 && snakeDir.y != -1;

			if (key == sf::Keyboard::W && b2)
				snakeDir = { 0, -1 };
			else if (key == sf::Keyboard::S && b2)
				snakeDir = { 0, 1 };
			else if (key == sf::Keyboard::D && b1)
				snakeDir = { 1, 0 };
			else if (key == sf::Keyboard::A && b1)
				snakeDir = { -1, 0 };
			else
				turning = false;
		}
		else
		{
			Restart();
		}
	}

	void Game::UpdateSnake()
	{
		Block& head = *snake.begin();
		Block tailEnd = *snake.rbegin();

		for (auto block = snake.rbegin(); block != snake.rend() - 1; block++)
		{
			if (head.Pos == block->Pos)
			{
				playing = false;
				return;
			}

			MoveBlock(*block, (block + 1)->Pos);
		}

		MoveBlock(head, head.Pos += snakeDir);

		if (head.Pos == apple.Pos)
		{
			snake.push_back(tailEnd);
			SpawnApple();
		}
	}

	void Game::SpawnApple()
	{
		static std::random_device rd;
		static std::default_random_engine engn(rd());
		static std::uniform_int_distribution<int> distX(0, gridSize.x);
		static std::uniform_int_distribution<int> distY(0, gridSize.y);

		MoveBlock(apple, { distX(engn), distY(engn) });
	}

	void Game::MoveBlock(Block& block, sf::Vector2i gridPos)
	{
		if (gridPos.x < 0) gridPos.x = gridSize.x - 1;
		if (gridPos.y < 0) gridPos.y = gridSize.y - 1;
		const sf::Vector2i coord(gridPos.x % gridSize.x, gridPos.y % gridSize.y);

		block.Pos = coord;
		block.Rect.setPosition(coord.x / (float)gridSize.x * windowSize.x,
			coord.y / (float)gridSize.y * windowSize.y);
	}

	void Game::Restart()
	{
		snake.clear();

		Block head({ 255, 255, 255 });
		MoveBlock(head, { 3, 9 });
		snake.push_back(head);

		for (int i = 0; i < 3; i++)
		{
			Block tail({ 152, 225, 112 });
			MoveBlock(tail, snake[i].Pos - snakeDir);
			snake.push_back(tail);
		}

		SpawnApple();

		playing = true;
	}

	void Game::Draw()
	{
		window->clear();

		window->draw(apple);

		for (auto block = snake.rbegin(); block != snake.rend(); block++)
			window->draw(*block);

		window->draw(fpsCount);

		window->display();
	}

}

#ifdef NDEBUG
int wWinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ PWSTR arguments, _In_ int commandShow)
{
	Application::Launch<Snake::Game>();
	return 0;
}
#else
int main(int argc, char** argv)
{
	Application::Launch<Snake::Game>();
	return 0;
}
#endif
