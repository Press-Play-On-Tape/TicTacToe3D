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
uint8_t gameOverCounter = 0;


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

		case GameState::Splash:
			showSplashScreen();
			break;

		case GameState::Title:
			showTitle();
			break;

		case GameState::Instructions:
			showInstructions();
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
		gameState = GameState::Instructions;
		aCount = 20;
		gameOverCounter = Constants::GameOverCounter_NoAction;

	}

}

void showInstructions() {

	Sprites::drawOverwrite(6, 8, Images::Instructions, 0);

	if (arduboy.justPressed(A_BUTTON)) {

		gameState = GameState::Game;

	}

}

void playGame() {

	Fixed centerY = layerSpacing - Fixed(15);

	if (xRotate) {

	  	rotateGridX(xRotateDirection); 
		drawGridLayer_X(Fixed(-15), gridSize, centerY);                          		// Bottom layer
		drawGridLayer_X(centerY, gridSize, centerY);            						// Middle layer - fill bottom
		drawGridLayer_X((layerSpacing * Fixed(2)) - Fixed(15), gridSize, centerY);      // Top layer 


	}
	else if (yRotate) {

		rotateGridY(yRotateDirection); 
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
				gameOverCounter = 0;

			}

			break;

		case GameState::GameOver:

			switch (checkForWin()) {

				case Players::Noughts:

					if (gameOverCounter != Constants::GameOverCounter_NoAction) {
						
						for (uint8_t i = 0; i < 15; i++) {

							uint8_t c = pgm_read_byte(&GameOver::YouWon_Letters[i]);
							uint8_t x = pgm_read_byte(&GameOver::YouWon_X[i]);
							int8_t y = pgm_read_byte(&GameOver::YouWon_Y[(gameOverCounter * 15) + i]);

							const uint8_t* letterPtr = (const uint8_t*)pgm_read_ptr(&Images::Letters[c]);
							const uint8_t* maskPtr = (const uint8_t*)pgm_read_ptr(&Images::Letters_Mask[c]);


							Sprites::drawExternalMask(x, y, letterPtr, maskPtr, 0, 0);
						}

						if (arduboy.frameCount % 2 == 0 && gameOverCounter < 54) gameOverCounter++;

					}			

					break;

				case Players::Crosses:

					if (gameOverCounter != Constants::GameOverCounter_NoAction) {
						
						for (uint8_t i = 0; i < 16; i++) {

							uint8_t c = pgm_read_byte(&GameOver::YouLost_Letters[i]);
							uint8_t x = pgm_read_byte(&GameOver::YouLost_X[i]);
							int8_t y = pgm_read_byte(&GameOver::YouLost_Y[(gameOverCounter * 16) + i]);

							const uint8_t* letterPtr = (const uint8_t*)pgm_read_ptr(&Images::Letters[c]);
							const uint8_t* maskPtr = (const uint8_t*)pgm_read_ptr(&Images::Letters_Mask[c]);

							Sprites::drawExternalMask(x, y, letterPtr, maskPtr, 0, 0);

						}

						if (arduboy.frameCount % 2 == 0 && gameOverCounter < 54) gameOverCounter++;

					}				
	
					break;

				case Players::Draw:

					if (gameOverCounter != Constants::GameOverCounter_NoAction) {
						
						for (uint8_t i = 0; i < 13; i++) {

							uint8_t c = pgm_read_byte(&GameOver::Draw_Letters[i]);
							uint8_t x = pgm_read_byte(&GameOver::Draw_X[i]);
							int8_t y = pgm_read_byte(&GameOver::Draw_Y[(gameOverCounter * 13) + i]);

							const uint8_t* letterPtr = (const uint8_t*)pgm_read_ptr(&Images::Letters[c]);
							const uint8_t* maskPtr = (const uint8_t*)pgm_read_ptr(&Images::Letters_Mask[c]);

							Sprites::drawExternalMask(x, y, letterPtr, maskPtr, 0, 0);

						}

						if (arduboy.frameCount % 2 == 0 && gameOverCounter < 44) gameOverCounter++;

					}				

					break;

			}

			if (arduboy.justPressed(A_BUTTON)) {

				gameState = GameState::GameOver_View_1;
				bCount = 0;

			}

			break;

	}
    

	if (arduboy.notPressed(A_BUTTON)) aCount = 0;
	if (arduboy.notPressed(B_BUTTON)) bCount = 0;


}

void showResults() {

	uint8_t xOffset = 0;

	switch (checkForWin()) {
	
		case Players::Noughts:
			Sprites::drawOverwrite(96, 51, Images::GameOver_YouWon_Small, 0);
			break;
				
		case Players::Crosses:
			Sprites::drawOverwrite(96, 51, Images::GameOver_YouLost_Small, 0);
			break;
				
		case Players::Draw:
			Sprites::drawOverwrite(96, 51, Images::GameOver_Draw_Small, 0);
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


	// Cycle graphics ..

	bCount++;

	if (bCount % 48 == 0) {

		bCount = 0;
		gameState = static_cast<GameState>(static_cast<uint8_t>(gameState) + 1);

		if (gameState == GameState::EndState) { gameState = GameState::GameOver_View_1; }

	}

	if (arduboy.justPressed(A_BUTTON)) {

		gameState = GameState::Title;

	}

}

void showSplashScreen() {

	Sprites::drawOverwrite(32, 17, Images::PPOT, 0);

	uint8_t y = 17; // Default pixel position 1 is hidden in the top line of the image

	switch (arduboy.frameCount % 32) {

		case 8 ... 15:
			y = 30; // Move pixel down to position 2
			[[fallthrough]]

		case 0 ... 7:
			Sprites::drawOverwrite(91, 25, Images::PPOT_Arrow, 0); // Flash 'Play' arrow
			break;

		case 16 ... 24:
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

	if (arduboy.justPressed(A_BUTTON)) {
		gameState = GameState::Title;
	}

	//Sprites::drawExternalMask(0, 0, Images::Letters[3], Images::Letters_Mask[3], 0,  0);


// Sprites::drawExternalMask(24,18,Images::Letters[6], Images::Letters_Mask[6], 0, 0);		Sprites::drawExternalMask(33,18,Images::Letters[0], Images::Letters_Mask[0], 0, 0);		Sprites::drawExternalMask(42,18,Images::Letters[12], Images::Letters_Mask[12], 0, 0);		Sprites::drawExternalMask(54,18,Images::Letters[4], Images::Letters_Mask[4], 0, 0);		Sprites::drawExternalMask(67,18,Images::Letters[14], Images::Letters_Mask[14], 0, 0);		Sprites::drawExternalMask(76,18,Images::Letters[21], Images::Letters_Mask[21], 0, 0);		Sprites::drawExternalMask(85,18,Images::Letters[4], Images::Letters_Mask[4], 0, 0);		Sprites::drawExternalMask(94,18,Images::Letters[17], Images::Letters_Mask[17], 0, 0);


// Sprites::drawExternalMask(28,32,Images::Letters[24], Images::Letters_Mask[24], 0, 0);		Sprites::drawExternalMask(38,32,Images::Letters[14], Images::Letters_Mask[14], 0, 0);		Sprites::drawExternalMask(47,32,Images::Letters[20], Images::Letters_Mask[20], 0, 0);		Sprites::drawExternalMask(60,32,Images::Letters[22], Images::Letters_Mask[22], 0, 0);		Sprites::drawExternalMask(72,32,Images::Letters[14], Images::Letters_Mask[14], 0, 0);		Sprites::drawExternalMask(81,32,Images::Letters[13], Images::Letters_Mask[13], 0, 0);

// Sprites::drawExternalMask(95,32,Images::Letters[26], Images::Letters_Mask[26], 0, 0);



	// if (z =< 99) {
		
	// for (uint8_t i = 0; i < 15; i++) {
	// 	uint8_t c = pgm_read_byte(&YChars[(z * 21) + (i * 3)]);
	// 	uint8_t x = pgm_read_byte(&YChars[(z * 21) + (i * 3) + 1]);
	// 	int8_t y = pgm_read_byte(&YChars[(z * 21) + (i * 3) + 2]);




    // const uint8_t* letterPtr = (const uint8_t*)pgm_read_ptr(&Images::Letters[c]);
    // const uint8_t* maskPtr = (const uint8_t*)pgm_read_ptr(&Images::Letters_Mask[c]);


	// 	Sprites::drawExternalMask(x, y, letterPtr, maskPtr, 0, 0);
	// }

	// if (z < 98) z++;

	// }
}