#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>
#include<iostream>
using namespace std;

int main(int argc, char* argv[]) {

	//SDL + Audio init
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		cout << "SDL Error " << SDL_GetError() << endl;
		return 1;
	}

	//Mixer init
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		cout << "Mixer Error " << Mix_GetError() << endl;
		return 1;
	}

	//TTf init
	if (TTF_Init() < 0) {
		cout << "TTF Error " << TTF_GetError() << endl;
		return 1;
	}

	//window init
	SDL_Window* window = SDL_CreateWindow("Dino Mini Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	if (!window) {
		cout << "Window Error " << SDL_GetError() << endl;
		return 1;
	}

	//Renderer init
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		cout << "Renderer Error " << SDL_GetError() << endl;
		return 1;
	}

	//dino load
	SDL_Surface* dinosurface = IMG_Load("C:\\Users\\Wav Legacy\\source\\repos\\Project2\\x64\\Debug\\assets\\dino1.png");
	if (!dinosurface) {
		cout << "Dino Error " << SDL_GetError() << endl;
		return 1;
	}

	//dino Texture
	SDL_Texture* dinotexture = SDL_CreateTextureFromSurface(renderer, dinosurface);
	SDL_FreeSurface(dinosurface);

	//dino size or position

	SDL_Rect dinorect = { 100,300,200,200 };

	//obstacle load
	SDL_Surface* obstaclesurface = IMG_Load("C:\\Users\\Wav Legacy\\source\\repos\\Project2\\x64\\Debug\\assets\\obstacle.png");
	if (!obstaclesurface) {
		cout << "Obstacle Error " << SDL_GetError() << endl;
		return 1;
	}

	//obstacle texture
	SDL_Texture* obstacletexture = SDL_CreateTextureFromSurface(renderer, obstaclesurface);
	SDL_FreeSurface(obstaclesurface);

	//obstacle size or position
	SDL_Rect obstaclerect = { 500,400,100,100 };

	//jump sound load
	Mix_Chunk* jump = Mix_LoadWAV("C:\\Users\\Wav Legacy\\source\\repos\\Project2\\x64\\Debug\\assets\\jump.wav");
	if (!jump) {
		cout << "Jump Error " << Mix_GetError() << endl;
		return 1;
	}
	
	//game over sound
	Mix_Chunk* gameover = Mix_LoadWAV("C:\\Users\\Wav Legacy\\source\\repos\\Project2\\x64\\Debug\\assets\\gameover.wav");
	if (!gameover) {
		cout << "Gameover Error" << Mix_GetError() << endl;
		return 1;
	}

	//Score load
	TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 24);
	if (!font) {
		cout << "Font Error " << TTF_GetError() << endl;
		return 1;
	}
	SDL_Color white = { 255,255,255 };

	//Restart load
	TTF_Font* restart = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 28);
	if (!restart) {
		cout << "restart Error " << TTF_GetError() << endl;
		return 1;
	}
	SDL_Color Red = { 255,0,0 };

	//title font
	TTF_Font* titlefont = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 48);
	if (!restart) {
		cout << "restart Error " << TTF_GetError() << endl;
		return 1;
	}

	//Game Loop
	bool isrunning = true;

	//game menu
	bool showMenu = true;
	while (showMenu) {
		SDL_Event menuevent;
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderClear(renderer);

		// Title Text
		SDL_Surface* titleSurface = TTF_RenderText_Solid(titlefont, "Dino Run Game", Red);
		SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
		SDL_Rect titleRect = { 250, 100, titleSurface->w, titleSurface->h };
		SDL_FreeSurface(titleSurface);
		SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
		SDL_DestroyTexture(titleTexture);

		// Instruction Text
		SDL_Surface* instrSurface = TTF_RenderText_Solid(titlefont, "Press Enter to Start", Red);
		SDL_Texture* instrTexture = SDL_CreateTextureFromSurface(renderer, instrSurface);
		SDL_Rect instrRect = { 220, 220, instrSurface->w, instrSurface->h };
		SDL_FreeSurface(instrSurface);
		SDL_RenderCopy(renderer, instrTexture, NULL, &instrRect);
		SDL_DestroyTexture(instrTexture);

		SDL_RenderPresent(renderer);

		while (SDL_PollEvent(&menuevent)) {
			if (menuevent.type == SDL_QUIT) {
				isrunning = false;
				showMenu = false;
			}
			if (menuevent.type == SDL_KEYDOWN) {
				if (menuevent.key.keysym.sym == SDLK_RETURN) {
					showMenu = false; // Enter pressed, start game
				}
			}
		}

		SDL_Delay(10);
	}
	// ----- END MENU -----

	bool isjumping = false;
	bool iscollision = false;
	bool wascollision = false;
	const int groundy = 300;
	const int maxjump = 100;
	 int jumpvelocity = 0;
	const int gravity = 3;
	const int jumpstrength = -15;
	const int obstaclespeed = 5;
	int score = 0;
	char scoretext[50];

	SDL_Event event;

	while (isrunning) {
		obstaclerect.x -= obstaclespeed;

		if (obstaclerect.x + obstaclerect.w < 0) {
			obstaclerect.x = 800;
		}
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				isrunning = false;
			}

			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_SPACE && !isjumping) {
					isjumping = true;
					jumpvelocity = jumpstrength;

					Mix_PlayChannel(-1, jump, 0);
				}
			}
		}

		if (isjumping) {
			dinorect.y += jumpvelocity;
			dinorect.y += gravity;

			if (dinorect.y <= maxjump) {
				dinorect.y = maxjump;
				jumpvelocity = 0;
			}

			if (dinorect.y >= groundy) {
				dinorect.y = groundy;
				isjumping = false;
			}
		}



		if (SDL_HasIntersection(&dinorect, &obstaclerect) && dinorect.y >= groundy) {
			iscollision = true;
			if (!wascollision) {
				cout << "Collision" << endl;
				Mix_PlayChannel(-1, gameover, 0);
				wascollision = true;
				iscollision = true;
			}
		}
		else {

			score++;
		}

		//score text
		sprintf_s(scoretext, "Score = %d", score);
		SDL_Surface* scsurface = TTF_RenderText_Solid(font, scoretext, white);
		SDL_Texture* sctexture = SDL_CreateTextureFromSurface(renderer, scsurface);
		SDL_Rect screct = { 10,10,scsurface->w,scsurface->h };
		SDL_FreeSurface(scsurface);

		//game over score
		if (iscollision) {

			SDL_RenderClear(renderer);


			SDL_RenderCopy(renderer, dinotexture, NULL, &dinorect);
			SDL_RenderCopy(renderer, obstacletexture, NULL, &obstaclerect);


			SDL_Surface* oversurface = TTF_RenderText_Solid(font, "Game over! Press Enter To Exit ", white);
			SDL_Texture* overtexture = SDL_CreateTextureFromSurface(renderer, oversurface);
			SDL_Rect overrect = { 200,250,oversurface->w,oversurface->h };
			SDL_FreeSurface(oversurface);
			SDL_RenderCopy(renderer, overtexture, NULL, &overrect);
			SDL_DestroyTexture(overtexture);

			//score after collision
			sprintf_s(scoretext, "Final score = %d", score);
			SDL_Surface* finalsurface = TTF_RenderText_Solid(font, scoretext, white);
			SDL_Texture* finaltexture = SDL_CreateTextureFromSurface(renderer, finalsurface);
			SDL_Rect finalrect = { 300,300,finalsurface->w,finalsurface->h };
			SDL_FreeSurface(finalsurface);
			SDL_RenderCopy(renderer, finaltexture, NULL, &finalrect);
			SDL_DestroyTexture(finaltexture);

			SDL_RenderPresent(renderer);

			//wait for game over
			if (iscollision) {
				bool waitforexit = true;
				while (waitforexit) {
					while (SDL_PollEvent(&event)) {
						if (event.type == SDL_QUIT) {
							waitforexit = false;
						}

						if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
							waitforexit = false;
						}
					}
					SDL_Delay(100);
				}

			}
		}

		//restart game
		if (iscollision) {
			SDL_Surface* ressurface = TTF_RenderText_Solid(restart, "Press R to restart", Red);
			SDL_Texture* restexture = SDL_CreateTextureFromSurface(renderer, ressurface);
			SDL_Rect resrect = { 200,100,ressurface->w,ressurface->h };
			SDL_FreeSurface(ressurface);
			SDL_RenderCopy(renderer, restexture, NULL, &resrect);
			SDL_RenderPresent(renderer);

			bool waitforrestart = true;
			while (waitforrestart) {
				while (SDL_PollEvent(&event)) {
					if (event.type == SDL_QUIT) {
						waitforrestart = false;
						isrunning = false;
					}
					if (event.type == SDL_KEYDOWN) {
						if (event.key.keysym.sym == SDLK_r) {
							// Reset all game state
							iscollision = false;
							wascollision = false;
							isjumping = false;
							score = 0;
							dinorect.y = groundy;
							obstaclerect.x = 800;
							waitforrestart = false;
						}
						else if (event.key.keysym.sym == SDLK_RETURN) {
							waitforrestart = false;
							isrunning = false;
						}
					}
				}
				SDL_Delay(10);
			}
			SDL_DestroyTexture(restexture);
		}


			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, dinotexture, NULL, &dinorect);
			SDL_RenderCopy(renderer, obstacletexture, NULL, &obstaclerect);
			SDL_RenderCopy(renderer, sctexture, NULL, &screct);

			SDL_RenderPresent(renderer);
			SDL_DestroyTexture(sctexture);

			SDL_Delay(16);
		}

		SDL_DestroyRenderer(renderer);
		SDL_DestroyTexture(dinotexture);
		SDL_DestroyTexture(obstacletexture);
		SDL_DestroyWindow(window);
		Mix_FreeChunk(jump);
		Mix_CloseAudio();
		TTF_CloseFont(font);
		IMG_Quit();
		SDL_Quit();

		return 0;

}
