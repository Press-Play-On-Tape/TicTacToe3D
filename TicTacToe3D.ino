#include <Arduboy2.h>
#include "TicTacToe3D_Constants.h"
#include "TicTacToe3D_Images.h"
#include "TicTacToe3D_Enums.h"

#define DEBUG_BREAK    asm volatile("break\n");

Arduboy2 arduboy;

GameState gameState = GameState::Title;
GameState prevGameState = GameState::Title;
ViewMode view = ViewMode::Left;
ViewControl viewControl = ViewControl::Left;

uint8_t cursor[3] = {0, 0, 0};		// z, y, x
uint8_t comp_cursor[3] = {255, 0, 0};		// z, y, x
uint8_t board[3][3][3] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};		// z, y, x

uint16_t bCount = 0;
uint16_t aCount = 0;
uint8_t playerOrComp = 0;

int8_t Background_LR_Cursor_X[3] = 				{ 0, 10, 20 };
int8_t Background_LR_Cursor_Z[3] = 				{ 0, 12, 24 };
int8_t Background_LR_Cursor_Z_XOffset[3] = 		{ -1, 4, 9 };
int8_t Background_M_Cursor_X[3] = 				{ 0, 10, 20 };
int8_t Background_M_Cursor_Y[3] = 				{ 20, 15, 10 };
int8_t Background_M_Cursor_Y_XOffset[3] = 		{ 0, -5, -10 };
int8_t Background_M_Cursor_X_YOffset[3] = 		{ 0, -10, -20 };


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
				
		playerOrComp = random(0, 2);	
		gameState = GameState::Game;
		prevGameState = GameState::Game;

	}

}

void playGame() {

	renderBoard();

	switch (bCount) {

		case 0 ... 8:

			switch (viewControl) {

				case ViewControl::Left:
				
					if (arduboy.justPressed(LEFT_BUTTON) && cursor[Constants::Y_Axis] < 2) 		{ cursor[Constants::Y_Axis]++; }
					if (arduboy.justPressed(RIGHT_BUTTON) && cursor[Constants::Y_Axis] > 0) 	{ cursor[Constants::Y_Axis]--; }
					if (arduboy.justPressed(UP_BUTTON) && cursor[Constants::Z_Axis] > 0) 		{ cursor[Constants::Z_Axis]--; }
					if (arduboy.justPressed(DOWN_BUTTON) && cursor[Constants::Z_Axis] < 2) 		{ cursor[Constants::Z_Axis]++; }
					break;

				case ViewControl::Middle:
				
					if (arduboy.justPressed(LEFT_BUTTON) && cursor[Constants::X_Axis] > 0) 		{ cursor[Constants::X_Axis]--; }
					if (arduboy.justPressed(RIGHT_BUTTON) && cursor[Constants::X_Axis] < 2) 	{ cursor[Constants::X_Axis]++; }
					if (arduboy.justPressed(UP_BUTTON) && cursor[Constants::Y_Axis] < 2) 		{ cursor[Constants::Y_Axis]++; }
					if (arduboy.justPressed(DOWN_BUTTON) && cursor[Constants::Y_Axis] > 0) 		{ cursor[Constants::Y_Axis]--; }
					break;

				case ViewControl::Right:
				
					if (arduboy.justPressed(LEFT_BUTTON) && cursor[Constants::X_Axis] > 0) 		{ cursor[Constants::X_Axis]--; }
					if (arduboy.justPressed(RIGHT_BUTTON) && cursor[Constants::X_Axis] < 2) 	{ cursor[Constants::X_Axis]++; }
					if (arduboy.justPressed(UP_BUTTON) && cursor[Constants::Z_Axis] > 0) 		{ cursor[Constants::Z_Axis]--; }
					if (arduboy.justPressed(DOWN_BUTTON) && cursor[Constants::Z_Axis] < 2) 		{ cursor[Constants::Z_Axis]++; }
					break;

			};

			break;

		default:

			if (arduboy.justPressed(LEFT_BUTTON)) {

				view = view == ViewMode::Left ? view : static_cast<ViewMode>((3 + static_cast<uint8_t>(view) - 1) % 3);

				switch (view) {
				
					case ViewMode::Left:
						if (viewControl == ViewControl::Right)	viewControl = ViewControl::Middle;
						break;

					case ViewMode::Right:
						if (viewControl == ViewControl::Left)	viewControl = ViewControl::Middle;
						break;

					default: break;

				}

			}

			if (arduboy.justPressed(RIGHT_BUTTON)) {

				view = view == ViewMode::Right ? view : static_cast<ViewMode>((static_cast<uint8_t>(view) + 1) % 3);

				switch (view) {
				
					case ViewMode::Left:
						if (viewControl == ViewControl::Right)	viewControl = ViewControl::Middle;
						break;

					case ViewMode::Right:
						if (viewControl == ViewControl::Left)	viewControl = ViewControl::Middle;
						break;

					default: break;

				}
				
			}


	}

	if (arduboy.pressed(A_BUTTON)) {

		aCount++;

		if (aCount > 48) {
			gameState = static_cast<GameState>(static_cast<uint8_t>(GameState::GameOver_View_1) + 2 - static_cast<uint8_t>(cursor[Constants::Z_Axis]));
			aCount = 0;
			viewControl = ViewControl::Middle;
		}

	}

	if (arduboy.pressed(B_BUTTON)) {

		bCount++;

	}

	if (arduboy.justReleased(B_BUTTON)) {

		if (bCount < 8) {

			viewControl = viewControl == ViewControl::Right ? ViewControl::Left : static_cast<ViewControl>((static_cast<uint8_t>(viewControl) + 1) % 3);

			switch (view) {
			
				case ViewMode::Left:
					if (viewControl == ViewControl::Right) { viewControl = ViewControl::Left;}
					break;
			
				case ViewMode::Right:
					if (viewControl == ViewControl::Left) { viewControl = ViewControl::Middle;}
					break;

				default: break;

			}

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
			Sprites::drawOverwrite(27 - xOffset, 34, Images::Background_M, 0);
			renderPieces_M(xOffset, 8, true);
			break;
	
		case GameState::GameOver_View_2:

			cursor[Constants::Z_Axis] = 2;
			Sprites::drawOverwrite(27 - xOffset, 34, Images::Background_M, 0);
			renderPieces_M(xOffset, 8, true);
			cursor[Constants::Z_Axis] = 1;
			Sprites::drawExternalMask(27 - xOffset, 16, Images::Background_M, Images::Background_M_Mask, 0, 0);
			renderPieces_M(xOffset, 3, true);
			break;
	
		case GameState::GameOver_View_3:

			cursor[Constants::Z_Axis] = 2;
			Sprites::drawOverwrite(27 - xOffset, 34, Images::Background_M, 0);
			renderPieces_M(xOffset, 8, true);
			cursor[Constants::Z_Axis] = 1;
			Sprites::drawExternalMask(27 - xOffset, 16, Images::Background_M, Images::Background_M_Mask, 0, 0);
			renderPieces_M(xOffset, 3, true);
			cursor[Constants::Z_Axis] = 0;
			Sprites::drawExternalMask(27 - xOffset, -2, Images::Background_M, Images::Background_M_Mask, 0, 0);
			renderPieces_M(xOffset, -2, true);
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


