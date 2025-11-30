#include <Arduboy2.h>
#include <FixedPoints.h>
#include <FixedPointsCommon.h>
#include "TicTacToe3D_Constants.h"
#include "TicTacToe3D_Images.h"
#include "TicTacToe3D_Enums.h"

#define DEBUG_BREAK    asm volatile("break\n");

Arduboy2 arduboy;

GameState gameState = GameState::Splash;

uint8_t cursor[3] = {2, 0, 0};				// z, y, x
uint8_t comp_cursor[3] = {255, 0, 0};		// z, y, x
uint8_t board[3][3][3] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};		// z, y, x

uint16_t bCount = 0;
uint16_t aCount = 0;
uint8_t playerOrComp = 0;

int8_t Background_M_Cursor_X[3] = 				{ 0, 8, 16 };
int8_t Background_M_Cursor_Y[3] = 				{ 16, 12, 8 };
int8_t Background_M_Cursor_Y_XOffset[3] = 		{ 0, -4, -8 };
int8_t Background_M_Cursor_X_YOffset[3] = 		{ 0, -8, -16 };

using Fixed = SQ15x16;

bool xRotate = false;
bool yRotate = false;
int8_t xRotateDirection = 0;
int8_t yRotateDirection = 0;

// Rotation angle
Fixed rotationAngle = 0;
const Fixed ROTATION_SPEED = 0.08; 				// radians per frame
const Fixed ROTATION_SPEED_NEG = -0.08; 		// radians per frame
const int8_t ROTATION_DIRECTION = -1; 			// 1 for forward, -1 for reverse

// Isometric projection constants
const Fixed COS_ISO = 0.866;    				// cos(30°)
const Fixed SIN_ISO = 0.5;      				// sin(30°)
Fixed gridSize = 26;
Fixed layerSpacing = 21;


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Setup
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void setup() {

	arduboy.boot();
	arduboy.flashlight(); 
	arduboy.audio.begin();

	arduboy.initRandomSeed();
	arduboy.setFrameRate(30);

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Main loop
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void loop() {

	if (!(arduboy.nextFrame())) return;
	arduboy.clear();
	arduboy.pollButtons();

	switch (gameState) {

		case GameState::Title:
			showTitle();
			break;

		case GameState::Game:
		case GameState::GameOver:
			playGame();
			break;

		case GameState::GameOver_View_1:
		case GameState::GameOver_View_2:
		case GameState::GameOver_View_3:
			showResults();
			break;

	}

    arduboy.display(true);

}

void showTitle() {

	Sprites::drawOverwrite(0, 0, Images::Title, 0);

	if (arduboy.justPressed(A_BUTTON)) {

		resetBoard();

		// board[0][0][0] = 1;
		// board[0][0][1] = 2;
		// board[0][0][2] = 1;

		// board[1][1][0] = 1;
		// board[1][1][1] = 2;
		// board[2][0][1] = 1;

		// board[2][2][0] = 2;
		// board[2][2][1] = 2;
				
		// playerOrComp = random(0, 2);	
		playerOrComp = 0;
		gameState = GameState::Game;
		prevGameState = GameState::Game;
		aCount = 20;

	}

}

void playGame() {

	Fixed centerY = layerSpacing - Fixed(15);

	// Sprites::drawOverwrite(-5, 0, Images::Side, 0);
	// Sprites::drawOverwrite(5 + 128 - 23, 0, Images::Side2, 0);

	if (xRotate) {

	  	rotateGridX(xRotateDirection); // Call with 1 for forward, -1 for reverse
		drawGridLayer_X(Fixed(-15), gridSize, centerY);                          		// Bottom layer
		drawGridLayer_X(centerY, gridSize, centerY);            						// Middle layer - fill bottom
		drawGridLayer_X((layerSpacing * Fixed(2)) - Fixed(15), gridSize, centerY);      // Top layer 


	}
	else if (yRotate) {

		rotateGridY(yRotateDirection); // Call with 1 for clockwise, -1 for counter-clockwise
		drawGridLayer_Y(Fixed(-15), gridSize);
		drawGridLayer_Y(centerY, gridSize);
		drawGridLayer_Y((layerSpacing * Fixed(2)) - Fixed(15), gridSize);

	}
	else {
	
		renderBoard();

	}

	// renderStatus();


	switch (bCount) {

		case 0 ... 8:
			
			if (arduboy.justPressed(LEFT_BUTTON) && cursor[Constants::X_Axis] > 0) 		{ cursor[Constants::X_Axis]--; }
			if (arduboy.justPressed(RIGHT_BUTTON) && cursor[Constants::X_Axis] < 2) 	{ cursor[Constants::X_Axis]++; }
			if (arduboy.justPressed(UP_BUTTON) && cursor[Constants::Y_Axis] < 2) 		{ cursor[Constants::Y_Axis]++; }
			if (arduboy.justPressed(DOWN_BUTTON) && cursor[Constants::Y_Axis] > 0) 		{ cursor[Constants::Y_Axis]--; }

			break;

		default:

			if (arduboy.justPressed(UP_BUTTON)) {

				xRotate = true;
				xRotateDirection = 1;
				rotationAngle = 0;

			}
			
			if (arduboy.justPressed(DOWN_BUTTON)) {

				xRotate = true;
				xRotateDirection = -1;
				rotationAngle = 0;

			}

			if (arduboy.justPressed(LEFT_BUTTON)) {

				yRotate = true;
				yRotateDirection = 1;
				rotationAngle = 0;

			}

			if (arduboy.justPressed(RIGHT_BUTTON)) {

				yRotate = true;
				yRotateDirection = -1;
				rotationAngle = 0;
			
			}

			break;

	}

	if (arduboy.pressed(A_BUTTON)) {

		aCount++;

	}

	if (arduboy.pressed(B_BUTTON)) {

		bCount++;

	}

	if (arduboy.justReleased(B_BUTTON)) {

		if (bCount < 8) {

			cursor[Constants::Z_Axis]++; 
			if (cursor[Constants::Z_Axis] > 2) cursor[Constants::Z_Axis] = 0; 

		}

		bCount = 0; 

	}


	// Have a turn ..

	switch (gameState) {

		case GameState::Game:

			if (playerOrComp == 0) {

				if (arduboy.justReleased(A_BUTTON) && (aCount < 10) && board[cursor[Constants::Z_Axis]][cursor[Constants::Y_Axis]][cursor[Constants::X_Axis]] == Players::None) {

					board[cursor[Constants::Z_Axis]][cursor[Constants::Y_Axis]][cursor[Constants::X_Axis]] = Players::Noughts;
					playerOrComp = 1;
					aCount = 0;

				}

			}
			else {
			
				computerMove();

			}


			// Check for win ..

			if (checkForWin() != Players::None) {
			
				gameState = GameState::GameOver;

			}

			break;

		case GameState::GameOver:

			switch (checkForWin()) {

				case Players::Noughts:
					Sprites::drawExternalMask(24, 18, Images::GameOver_YouWon, Images::GameOver_YouWon_Mask, 0, 0);
					break;

				case Players::Crosses:
					Sprites::drawExternalMask(24, 18, Images::GameOver_YouLost, Images::GameOver_YouLost_Mask, 0, 0);
					break;

				case Players::Draw:
					Sprites::drawExternalMask(24, 18, Images::GameOver_Draw, Images::GameOver_Draw_Mask, 0, 0);
					break;

			}

			if (arduboy.justPressed(A_BUTTON)) {

				gameState = GameState::GameOver_View_1;
				prevGameState = GameState::Title;
				bCount = 0;

			}

			break;

	}
    

	if (arduboy.notPressed(A_BUTTON)) aCount = 0;
	if (arduboy.notPressed(B_BUTTON)) bCount = 0;


}

void showResults() {

	uint8_t xOffset = 0;

	switch (prevGameState) {

		case GameState::Game:
			Sprites::drawOverwrite(83, 44, Images::StackInstructions, 0);
			xOffset = 10;
			break;

	}

	switch (checkForWin()) {
	
		case Players::Noughts:
			Sprites::drawOverwrite(96, 51, Images::GameOver_YouWon_Small, 0);
			break;
				
		case Players::Crosses:
			Sprites::drawOverwrite(96, 51, Images::GameOver_YouLost_Small, 0);
			break;
			
	}

	switch (gameState) {
		
		case GameState::GameOver_View_1:

			cursor[Constants::Z_Axis] = 2;
			Sprites::drawOverwrite(38, 40, Images::Background, 0);
			renderPieces(xOffset, 2, true);
			break;
	
		case GameState::GameOver_View_2:

			cursor[Constants::Z_Axis] = 2;
			Sprites::drawOverwrite(38, 40, Images::Background, 0);
			renderPieces(xOffset, 2, true);
			cursor[Constants::Z_Axis] = 1;
			Sprites::drawExternalMask(38, 18, Images::Background, Images::Background_Mask, 0, 0);
			renderPieces(xOffset, 1, true);
			break;
	
		case GameState::GameOver_View_3:

			cursor[Constants::Z_Axis] = 2;
			Sprites::drawOverwrite(38, 40,  Images::Background, 0);
			renderPieces(xOffset, 2, true);
			cursor[Constants::Z_Axis] = 1;
			Sprites::drawExternalMask(38, 18, Images::Background, Images::Background_Mask, 0, 0);
			renderPieces(xOffset, 1, true);
			cursor[Constants::Z_Axis] = 0;
			Sprites::drawExternalMask(38, -4, Images::Background, Images::Background_Mask, 0, 0);
			renderPieces(xOffset, 0, true);
			break;
	
	}

	switch (prevGameState) {

		case GameState::Title:

			bCount++;

			if (bCount % 48 == 0) {

				bCount = 0;
				gameState = static_cast<GameState>(static_cast<uint8_t>(gameState) + 1);

				if (gameState == GameState::EndState) { gameState = GameState::GameOver_View_1; }

			}

			if (arduboy.justPressed(A_BUTTON)) {

				gameState = GameState::Title;

			}

			break;

		case GameState::Game:

			if (arduboy.pressed(A_BUTTON) && arduboy.justPressed(UP_BUTTON)) {

				switch (gameState) {
				
					case GameState::GameOver_View_1:	gameState = GameState::GameOver_View_2;		break;
					case GameState::GameOver_View_2:	gameState = GameState::GameOver_View_3;		break;

				}

			}

			if (arduboy.pressed(A_BUTTON) && arduboy.justPressed(DOWN_BUTTON)) {

				switch (gameState) {
				
					case GameState::GameOver_View_2:	gameState = GameState::GameOver_View_1;		break;
					case GameState::GameOver_View_3:	gameState = GameState::GameOver_View_2;		break;

				}

			}

			if (arduboy.justReleased(A_BUTTON)) {

				gameState = GameState::Game;
			
			}

			break;

	}

}

void splashScreen() {

	Sprites::drawOverwrite(32, 17, Images::PPOT, 0);

	uint8_t y = 17; // Default pixel position 1 is hidden in the top line of the image
	switch (arduboy.getFrameCount(48)) {

		case 12 ... 23:
			y = 30; // Move pixel down to position 2
			[[fallthrough]]

		case 0 ... 11:
			Sprites::drawOverwrite(91, 25, Images::PPOT_Arrow, 0); // Flash 'Play' arrow
			break;

		case 24 ... 35:
			y = 31; // Move pixel down to position 3
			break;

		default: // 36 ... 47:
			y = 32; // Move pixel down to position 4
			break;

	}

	arduboy.drawPixel(52, y, WHITE); // Falling pixel represents the tape spooling
	if (y % 2 == 0) { // On even steps of pixel movement, update the spindle image
		Sprites::drawOverwrite(45, 28, Images::PPOT_Spindle, 0);
	}

}