#include "Game.h"

#pragma comment(lib, "x64/SDL2main")
#pragma comment(lib, "x64/SDL2")


Game::Game() {
	mTickCount = 0;
	mWindow = nullptr;
	mRenderer = nullptr;
	mIsRunning = false;
}

bool Game::Initialize() {

	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0)
	{
		SDL_Log("Unable to Initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	mWindow = SDL_CreateWindow("Game Programming C++", 100, 100, 800, 600, 0);

	if (!mWindow)
	{
		SDL_Log("Unable to Create Window: %s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (mRenderer == nullptr)
	{
		SDL_Log("Unable to Create Renderer: %s", SDL_GetError());
		return false;
	}
	
	mIsRunning = true;
	return true;
}

void Game::RunLoop() {
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();

	}
}

void Game::Shutdown() {
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::ProcessInput() {
	SDL_Event e;
	
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		default:
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);

	mPaddleDir = 0;
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
	else if (state[SDL_SCANCODE_W])
	{
		mPaddleDir = +1;
	}
	else if (state[SDL_SCANCODE_S])
	{
		mPaddleDir = -1;
	}
	else if(state[SDL_SCANCODE_R])
	{
		restartGame = true;
	}
}

void Game::UpdateGame() {

	//while (SDL_TICKS_PASSED(SDL_GetTicks(), mTickCount + 16));

	float deltaTime = (SDL_GetTicks() - mTickCount) / 1000.0f;
	mTickCount = SDL_GetTicks();

	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	if (mPaddleDir != 0)
	{
		mPaddlePoss.y -= mPaddleDir * 300.0f * deltaTime;

		if (mPaddlePoss.y < (thickness))
		{
			mPaddlePoss.y = thickness;
		}
		else if (mPaddlePoss.y > (600 - paddleLength - thickness))
		{
			mPaddlePoss.y = 600 - paddleLength - thickness;
		}

	}

	if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1;
	}
	else if (mBallPos.y >= (600 - thickness) && mBallVel.y > 0.0f)
	{
		mBallVel.y *= -1;
	}
	else if (mBallPos.x >= (800 - thickness) && mBallVel.x > 0.0f)
	{
		mBallVel.x *= -1;
	}

	float diff = mPaddlePoss.y - mBallPos.y;
	diff = diff > 0 ? diff : -diff;

	if (diff <= paddleLength && 
		mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
		mBallVel.x < 0.0f)
	{
		mBallVel.x *= -1;
	}


	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	if (mBallPos.x < 0.0f && restartGame)
	{
		mBallPos = { 400, 300};
		restartGame = false;
	}

}

void Game::GenerateOutput() {
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
	SDL_RenderClear(mRenderer);


	SDL_Rect wall{
		0,
		0,
		800,
		thickness
	}, wall2{
		0, 600 - thickness,800, thickness
	}, wall3{
		800 - thickness, 0, thickness, 600
	};

	SDL_SetRenderDrawColor(mRenderer, 0, 255, 255, 255);

	SDL_RenderFillRect(mRenderer, &wall);
	SDL_RenderFillRect(mRenderer, &wall2);
	SDL_RenderFillRect(mRenderer, &wall3);

	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};

	SDL_RenderFillRect(mRenderer, &ball);

	SDL_Rect paddle{
		0,
		static_cast<int>(mPaddlePoss.y - thickness / 2),
		thickness,
		paddleLength
	};

	SDL_RenderFillRect(mRenderer, &paddle);

	SDL_RenderPresent(mRenderer);

}

int main(int argc, char** argv) {
	Game* g = new Game();
	bool success = g->Initialize();
	if (success)
		g->RunLoop();
	g->Shutdown();

	return 0;
}

