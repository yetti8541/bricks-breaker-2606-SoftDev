#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	Reset();
}
Game::~Game()
{

}

void Game::Reset()
{
	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::CursorVisible(false);

	gameWon = false; 
	gameLost = false; 

	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'O';
	ball.color = ConsoleColor::Cyan;
	ResetBall();

	 // Clear vector for reset. 
	bricks.clear();
	brickHits.clear();


	// TODO #2 - Add this brick and 4 more bricks to the vector
	for (int i = 0; i < 5; i++)
	{
		Box newBrick;

		newBrick.width = 10;
		newBrick.height = 2;
		newBrick.x_position = 3 + i * 15;//Space bricks out across the top of the window.
		newBrick.y_position = 5;
		newBrick.doubleThick = true;
		newBrick.color = ConsoleColor::DarkGreen;

		bricks.push_back(newBrick);//Add new brick to vector.
		brickHits.push_back(0); //Initialize hit count for each brick to 0.
	}
}

void Game::ResetBall()
{
	ball.x_position = paddle.x_position + paddle.width / 2;
	ball.y_position = paddle.y_position - 1;
	ball.x_velocity = rand() % 2 ? 1 : -1;
	ball.y_velocity = -1;
	ball.moving = false;
}

bool Game::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
		return false;

	if (GetAsyncKeyState('R') & 0x1)
		Reset();

	if (gameWon || gameLost)	
		return true;

	if (GetAsyncKeyState(VK_RIGHT) && paddle.x_position < WINDOW_WIDTH - paddle.width)
		paddle.x_position += 2;

	if (GetAsyncKeyState(VK_LEFT) && paddle.x_position > 0)
		paddle.x_position -= 2;

	if (GetAsyncKeyState(VK_SPACE) & 0x1)
		ball.moving = !ball.moving;

	

	ball.Update();
	CheckCollision();

	return true;
}

//  All rendering, including text, should occur in the Render function
void Game::Render() const
{
	Console::Lock(true);
	Console::Clear();
	
	paddle.Draw();
	ball.Draw();

	// TODO #3 - Update render to render all bricks
	for (int i = 0; i < bricks.size(); i++)
	{
		bricks[i].Draw();
	}

	if (gameWon)
	{
		Console::WordWrap(
			WINDOW_WIDTH / 2 - 16,
			WINDOW_HEIGHT / 2,
			40,
			"You win! Press R to play again."
		);
	}

	if (gameLost)
	{
		Console::WordWrap(
			WINDOW_WIDTH / 2 - 16,
			WINDOW_HEIGHT / 2,
			40,
			"You lose. Press R to play again."
		);
	}

	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	for (int i = 0; i < bricks.size(); i++)//Loop through all bricks to check for collision.
	{
		if (bricks[i].Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			ball.y_velocity *= -1; // Bounce the ball.

			brickHits[i]++;//Increment hit count for this brick.

			if (brickHits[i] == 1) // Change the brick color after each hit so damage is visible.

			{
				bricks[i].color = ConsoleColor::DarkYellow;
			}
			else if (brickHits[i] == 2)
			{
				bricks[i].color = ConsoleColor::DarkRed;
			}

			// TODO #5 - If the ball hits the same brick 3 times, remove it from the vector
			if (brickHits[i] >= 3) //If hit count reaches 3, remove the brick from the vector.
			{
				bricks.erase(bricks.begin() + i); //Remove the brick from the vector.
				brickHits.erase(brickHits.begin() + i); 
			}

			break;
		}
	}

	// TODO #6 - If no bricks remain, pause ball and display victory text in Render
	if (bricks.empty()) //If there are no bricks left, the player wins.
	{
		gameWon = true;
		ball.moving = false;
	}

	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position)) // If the ball hits the paddle, bounce it.
	{
		ball.y_velocity *= -1; // Bounce the ball.
	}

	// TODO #7 - If ball touches bottom of window, pause ball and display defeat text in Render
	if (ball.y_position >= WINDOW_HEIGHT - 1) //If the ball touches the bottom of the window, the player loses.
	{
		gameLost = true;
		ball.moving = false;
	}
}