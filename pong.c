#include <stdio.h>
#include <SDL2/SDL.h>
//TO DO:
// handle collision for botton of the ball 
// improve ai
// handle collision for top/ botton of paddle
// create score card
// create title screen 
// make game harder

typedef struct {
	//struct for ball
	int x;
	int y;
	int width;
	int height;
}Paddle;

typedef struct {
	// struct for pabbles
	int x, y;
	int dx, dy;
	int width;
	int	height;
}Ball;

typedef struct {
	// struct containing all game info
	
	SDL_Window *window;
	SDL_Renderer *rend;
	
	int gameWidth;
	int gameHeight;
	int playerSpeed;
	int aiSpeed;
	
	Paddle player, ai;
	Ball ball;
	
}GameState;

void loadGame(GameState *game);
void processEvents(int *done, GameState *game);
void processMovement(GameState *game);
void moveBall(GameState *game);
void moveAi(GameState *game);
void drawScreen(GameState *game);
void movePaddle(GameState *game);
void movePaddleAi(GameState *game);
void moveBall(GameState *game);
void quitGame(GameState *game);

int main(int argc, char *argv[]) {
	GameState game;
	SDL_Init(SDL_INIT_VIDEO);
	
	loadGame(&game);
	
	int done = 0;
	while(!done) {
		processEvents(&done, &game);
		moveBall(&game);
		moveAi(&game);
		drawScreen(&game);
	}
	
	quitGame(&game);
	return 1;
}

void loadGame(GameState *game) {
	// handles creating window, renderer and game pieces
	game -> gameWidth = 640;
	game -> gameHeight = 480;
	game -> playerSpeed = 10;
	game -> aiSpeed = 10;
	
	game -> window = SDL_CreateWindow("Pong!", SDL_WINDOWPOS_CENTERED, 
						SDL_WINDOWPOS_CENTERED, game -> gameWidth, game -> gameHeight, 0);
	game -> rend = SDL_CreateRenderer(game -> window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	game -> player = (Paddle) {20, 190, 10, 100};
	game -> ai = (Paddle) {610, 190, 10, 100};
	game -> ball = (Ball) {315, 235, -3, 5, 10, 10};
}

void processEvents(int *done, GameState *game) {
	// EVENT HANDLING FUNCTION
	SDL_Event event;
	
	processMovement(game);
	
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_WINDOWEVENT_CLOSE : 
				*done = 1;
				break;
			
			case SDL_QUIT :
				*done = 1;
				break;
			
			case SDL_KEYDOWN : {
				switch(event.key.keysym.sym) {
					case SDLK_ESCAPE:
						*done = 1;
						break;
				}
			}
		}
	}
}

void processMovement(GameState *game) {
	// helper function for process events
	//process player movement and collision 
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	if(keystate[SDL_SCANCODE_UP]) {
		if((game -> player.y - game -> playerSpeed) <= 0) {
			game -> player.y = 0;
		}
		else {
			game -> player.y -= game -> playerSpeed;
		}
	}
	if(keystate[SDL_SCANCODE_DOWN]) {
		if((game -> player.y + game -> player.height + game -> playerSpeed) 
			>= game -> gameHeight) {
			game -> player.y = game -> gameHeight - game -> player.height;
		}
		else {
			game -> player.y += game -> playerSpeed;
		}
	}
}

void moveBall(GameState *game) {
	// moves ball (x,y) by dx and dy
	game -> ball.x += game -> ball.dx;
	game -> ball.y += game -> ball.dy;
	
	// collision handling: walls
	if(game -> ball.y <= 0 || game -> ball.y >= game -> gameHeight) {
		game -> ball.dy *= -1;
	}
	// collision handling: player
	if(game-> ball.y >= game-> player.y && game-> ball.y <= (game-> player.y + game-> player.height)
		&& game-> ball.x <= (game-> player.x + game-> player.width) && game-> ball.x >= game-> player.x) {
			game-> ball.dx *= -1;
	}
	// collision handling: ai
	if(game-> ball.y >= game-> ai.y && game-> ball.y <= (game-> ai.y + game-> ai.height)
		&& (game-> ball.x + game-> ball.width) >= game-> ai.x && game-> ball.x <= (game-> ai.x + game-> ai.width)){
		game-> ball.dx *= -1;
	}
	
}

void moveAi(GameState *game) {
	// handles moving ai in responce to the ball
	int ballCenter = game-> ball.height / 2;
	int aiCenter = game-> ai.height / 2;
	
	
	if((game-> ball.y + ballCenter) < (game-> ai.y + ballCenter)) {
		if((game -> ai.y - game -> aiSpeed) <= 0) {
			game-> ai.y = 0;
		}
		else {
			game -> ai.y -= game -> aiSpeed;
		}
	}
	else if((game-> ball.y + ballCenter) > (game-> ai.y + ballCenter)) {
		if((game -> ai.y + game -> ai.height + game -> aiSpeed) >= game -> gameHeight) {
			game -> ai.y = game -> gameHeight - game -> ai.height;
		}
		else {
			game -> ai.y += game -> aiSpeed;
		}
	}
}

void drawScreen(GameState *game) {
	// draw screen function
	SDL_SetRenderDrawColor(game -> rend, 10, 10, 10, 255);
	SDL_RenderClear(game -> rend);

	SDL_SetRenderDrawColor(game -> rend, 255, 255, 255, 255);
	
	SDL_Rect playerRect = {game -> player.x, game -> player.y, game -> player.width, game -> player.height};
	SDL_RenderFillRect(game -> rend, &playerRect);
	
	SDL_Rect aiRect = {game -> ai.x, game -> ai.y, game -> ai.width, game -> ai.height};
	SDL_RenderFillRect(game -> rend, &aiRect);
	
	SDL_Rect ballRect = {game -> ball.x, game -> ball.y, game -> ball.width, game -> ball.height};
	SDL_RenderFillRect(game -> rend, &ballRect);
	
	SDL_RenderPresent(game -> rend);
	SDL_Delay(10);
	
}

void quitGame(GameState *game) {
	SDL_DestroyRenderer(game -> rend);
	SDL_DestroyWindow(game -> window);
	SDL_Quit();
}



