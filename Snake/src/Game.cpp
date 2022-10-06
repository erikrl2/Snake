#include "Game.h"

#include <random>
#include <format>

#ifdef NDEBUG
#include <Windows.h>
#define MAIN() wWinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ PWSTR arguments, _In_ int commandShow)
#else
#define MAIN() main(int argc, char** argv)
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
		SpawnApple();
	}

	void Game::Update(sf::Time ts)
	{
		if (playing)
		{
			static float t = 0;
			if (t >= snakeUpdateTime)
			{
				UpdateSnake();

				t = 0;
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
		int bufferSize = (int)dirBuffer.size();
		if (bufferSize >= 2)
			return;

		sf::Keyboard::Key key = event.key.code;
		bool canTurnY = snakeDir.y == 0 || bufferSize > 0;
		bool canTurnX = snakeDir.x == 0 || bufferSize > 0;

		if (key == sf::Keyboard::W && canTurnY)
			dirBuffer.push({ 0, -1 });
		else if (key == sf::Keyboard::S && canTurnY)
			dirBuffer.push({ 0, 1 });
		else if (key == sf::Keyboard::D && canTurnX)
			dirBuffer.push({ 1, 0 });
		else if (key == sf::Keyboard::A && canTurnX)
			dirBuffer.push({ -1, 0 });
		else
			return;

		if (!playing)
			Restart();
	}

	void Game::UpdateSnake()
	{
		UpdateSnakeDir();

		Block& head = *snake.begin();
		Block& tailEnd = *snake.rbegin();

		for (auto block = snake.rbegin(); block != snake.rend() - 1; block++)
		{
			if (head.Pos == block->Pos)
				return Reset();

			SetBlockPos(*block, (block + 1)->Pos);
		}

		SetBlockPos(head, head.Pos + snakeDir);

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

		SetBlockPos(apple, { distX(engn), distY(engn) });
	}

	void Game::Reset()
	{
		std::queue<sf::Vector2i> temp;
		dirBuffer.swap(temp);

		playing = false;
		snakeDir = {};
	}

	void Game::Restart()
	{
		snake.clear();

		UpdateSnakeDir();

		Block head({ 255, 255, 255 });
		SetBlockPos(head, { 3, 9 });
		snake.push_back(head);

		for (int i = 0; i < 3; i++)
		{
			Block tail({ 152, 225, 112 });
			SetBlockPos(tail, snake[i].Pos - snakeDir);
			snake.push_back(tail);
		}

		playing = true;
	}

	void Game::UpdateSnakeDir()
	{
		if (!dirBuffer.empty())
		{
			snakeDir = dirBuffer.front();
			dirBuffer.pop();
		}
	}

	void Game::SetBlockPos(Block& block, sf::Vector2i gridPos)
	{
		if (gridPos.x < 0) gridPos.x = gridSize.x - 1;
		if (gridPos.y < 0) gridPos.y = gridSize.y - 1;
		sf::Vector2i coord(gridPos.x % gridSize.x, gridPos.y % gridSize.y);

		block.Pos = coord;
		block.Rect.setPosition(coord.x / (float)gridSize.x * windowSize.x,
			coord.y / (float)gridSize.y * windowSize.y);
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

int MAIN()
{
	Application::Launch<Snake::Game>();
	return 0;
}
