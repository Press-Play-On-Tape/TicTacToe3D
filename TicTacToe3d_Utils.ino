#include <Arduboy2.h>

void resetBoard() {

	for (uint8_t z = 0; z < 3; z++) {

		for (uint8_t y = 0; y < 3; y++) {  

			for (uint8_t x = 0; x < 3; x++) {

				board[z][y][x] = Players::None;

			}

		}

	}

	cursor[0] = 0;
	cursor[1] = 0;
	cursor[2] = 0;
    comp_cursor[0] = 255;

}

Players checkForWin() {
    
    // Check all possible 3-in-a-row combinations
    
    // 1. Check rows in each layer (9 rows total, 3 per layer) ..

    for (uint8_t z = 0; z < 3; z++) {

        for (uint8_t y = 0; y < 3; y++) {

            if (board[z][y][0] != Players::None &&
                board[z][y][0] == board[z][y][1] &&
                board[z][y][1] == board[z][y][2]) {
                return board[z][y][0];
            }

        }

    }
    

    // 2. Check columns in each layer (9 columns total, 3 per layer) ..

    for (uint8_t z = 0; z < 3; z++) {

        for (uint8_t x = 0; x < 3; x++) {

            if (board[z][0][x] != Players::None &&
                board[z][0][x] == board[z][1][x] &&
                board[z][1][x] == board[z][2][x]) {
                return board[z][0][x];
            }

        }
    }

    
    // 3. Check diagonals in each layer (6 diagonals total, 2 per layer) ..

    for (uint8_t z = 0; z < 3; z++) {

        // Diagonal top-left to bottom-right
        if (board[z][0][0] != Players::None &&
            board[z][0][0] == board[z][1][1] &&
            board[z][1][1] == board[z][2][2]) {
            return board[z][0][0];
        }

        // Diagonal top-right to bottom-left
        if (board[z][0][2] != Players::None &&
            board[z][0][2] == board[z][1][1] &&
            board[z][1][1] == board[z][2][0]) {
            return board[z][0][2];
        }
		
    }
    

    // 4. Check vertical lines through all layers (9 lines total) ..

    for (uint8_t y = 0; y < 3; y++) {

        for (uint8_t x = 0; x < 3; x++) {

            if (board[0][y][x] != Players::None &&
                board[0][y][x] == board[1][y][x] &&
                board[1][y][x] == board[2][y][x]) {
                return board[0][y][x];
            }

        }

    }
    

    // 5. Check diagonals through layers in X-Z plane (6 lines) ..

    for (uint8_t y = 0; y < 3; y++) {

        // Diagonal front-left to back-right
        if (board[0][y][0] != Players::None &&
            board[0][y][0] == board[1][y][1] &&
            board[1][y][1] == board[2][y][2]) {
            return board[0][y][0];
        }

        // Diagonal front-right to back-left
        if (board[0][y][2] != Players::None &&
            board[0][y][2] == board[1][y][1] &&
            board[1][y][1] == board[2][y][0]) {
            return board[0][y][2];
        }

    }
    

    // 6. Check diagonals through layers in Y-Z plane (6 lines) ..

    for (uint8_t x = 0; x < 3; x++) {

        // Diagonal top-front to bottom-back
        if (board[0][0][x] != Players::None &&
            board[0][0][x] == board[1][1][x] &&
            board[1][1][x] == board[2][2][x]) {
            return board[0][0][x];
        }

        // Diagonal bottom-front to top-back
        if (board[0][2][x] != Players::None &&
            board[0][2][x] == board[1][1][x] &&
            board[1][1][x] == board[2][0][x]) {
            return board[0][2][x];
        }

    }
    
    // 7. Check 4 main 3D diagonals (corner to corner through center)
    // Front-top-left to back-bottom-right ..

    if (board[0][0][0] != Players::None &&
        board[0][0][0] == board[1][1][1] &&
        board[1][1][1] == board[2][2][2]) {
        return board[0][0][0];
    }
    

    // Front-top-right to back-bottom-left ..

    if (board[0][0][2] != Players::None &&
        board[0][0][2] == board[1][1][1] &&
        board[1][1][1] == board[2][2][0]) {
        return board[0][0][2];
    }
    

    // Front-bottom-left to back-top-right ..

    if (board[0][2][0] != Players::None &&
        board[0][2][0] == board[1][1][1] &&
        board[1][1][1] == board[2][0][2]) {
        return board[0][2][0];
    }
    

    // Front-bottom-right to back-top-left ..

    if (board[0][2][2] != Players::None &&
        board[0][2][2] == board[1][1][1] &&
        board[1][1][1] == board[2][0][0]) {
        return board[0][2][2];
    }
    
    return Players::None;

}


bool checkForWinningPiece(uint8_t z_Test, uint8_t y_Test, uint8_t x_Test) {
    
    // Check all possible 3-in-a-row combinations
    
    // 1. Check rows in each layer (9 rows total, 3 per layer) ..

    for (uint8_t z = 0; z < 3; z++) {

        for (uint8_t y = 0; y < 3; y++) {

            if (board[z][y][0] != Players::None &&
                board[z][y][0] == board[z][y][1] &&
                board[z][y][1] == board[z][y][2]) {
// Serial.println("a");
                return (z_Test == z && y_Test == y);

            }

        }

    }
    

    // 2. Check columns in each layer (9 columns total, 3 per layer) ..

    for (uint8_t z = 0; z < 3; z++) {

        for (uint8_t x = 0; x < 3; x++) {

            if (board[z][0][x] != Players::None &&
                board[z][0][x] == board[z][1][x] &&
                board[z][1][x] == board[z][2][x]) {
// Serial.println("b");

                return (z_Test == z && x_Test == x);

            }

        }

    }

    
    // 3. Check diagonals in each layer (6 diagonals total, 2 per layer) ..

    for (uint8_t z = 0; z < 3; z++) {

        // Diagonal top-left to bottom-right
        if (board[z][0][0] != Players::None &&
            board[z][0][0] == board[z][1][1] &&
            board[z][1][1] == board[z][2][2]) {
// Serial.println("c");

            return (z_Test == z && y_Test == x_Test);

        }

        // Diagonal top-right to bottom-left
        if (board[z][0][2] != Players::None &&
            board[z][0][2] == board[z][1][1] &&
            board[z][1][1] == board[z][2][0]) {
// Serial.println("d");

            return (z_Test == z && ((y_Test == 0 && x_Test == 2) || (y_Test == 1 && x_Test == 1) || (y_Test == 2 && x_Test == 0)));

        }
		
    }
    

    // 4. Check vertical lines through all layers (9 lines total) ..

    for (uint8_t y = 0; y < 3; y++) {

        for (uint8_t x = 0; x < 3; x++) {

            if (board[0][y][x] != Players::None &&
                board[0][y][x] == board[1][y][x] &&
                board[1][y][x] == board[2][y][x]) {
// Serial.println("e");

                return (y_Test == y && x_Test == x);

            }

        }

    }
    

    // 5. Check diagonals through layers in X-Z plane (6 lines) ..

    for (uint8_t y = 0; y < 3; y++) {

        // Diagonal front-left to back-right
        if (board[0][y][0] != Players::None &&
            board[0][y][0] == board[1][y][1] &&
            board[1][y][1] == board[2][y][2]) {
// Serial.println("f");

            return (y_Test == y && ((z_Test == 0 && x_Test == 0) || (z_Test == 1 && x_Test == 1) || (z_Test == 2 && x_Test == 2)));

        }

        // Diagonal front-right to back-left
        if (board[0][y][2] != Players::None &&
            board[0][y][2] == board[1][y][1] &&
            board[1][y][1] == board[2][y][0]) {
// Serial.println("g");

            return (y_Test == y && ((z_Test == 0 && x_Test == 2) || (z_Test == 1 && x_Test == 1) || (z_Test == 2 && x_Test == 0)));

        }

    }
    

    // 6. Check diagonals through layers in Y-Z plane (6 lines) ..

    for (uint8_t x = 0; x < 3; x++) {

        // Diagonal top-front to bottom-back
        if (board[0][0][x] != Players::None &&
            board[0][0][x] == board[1][1][x] &&
            board[1][1][x] == board[2][2][x]) {
// Serial.println("h");

            return (x_Test == x && ((z_Test == 0 && y_Test == 0) || (z_Test == 1 && y_Test == 1) || (z_Test == 2 && y_Test == 2)));

        }

        // Diagonal bottom-front to top-back
        if (board[0][2][x] != Players::None &&
            board[0][2][x] == board[1][1][x] &&
            board[1][1][x] == board[2][0][x]) {
// Serial.println("i");

            return (x_Test == x && ((z_Test == 0 && y_Test == 2) || (z_Test == 1 && y_Test == 1) || (z_Test == 2 && y_Test == 0)));

        }

    }
    
    // 7. Check 4 main 3D diagonals (corner to corner through center)
    // Front-top-left to back-bottom-right ..

    if (board[0][0][0] != Players::None &&
        board[0][0][0] == board[1][1][1] &&
        board[1][1][1] == board[2][2][2]) {
// Serial.println("j");

        return ((z_Test == 0 && y_Test == 0 && x_Test == 0) || (z_Test == 1 && y_Test == 1 && x_Test == 1) || (z_Test == 2 && y_Test == 2 && x_Test == 2));

    }
    

    // Front-top-right to back-bottom-left ..

    if (board[0][0][2] != Players::None &&
        board[0][0][2] == board[1][1][1] &&
        board[1][1][1] == board[2][2][0]) {
// Serial.println("k");

        return ((z_Test == 0 && y_Test == 0 && x_Test == 2) || (z_Test == 1 && y_Test == 1 && x_Test == 1) || (z_Test == 2 && y_Test == 2 && x_Test == 0));

    }
    

    // Front-bottom-left to back-top-right ..

    if (board[0][2][0] != Players::None &&
        board[0][2][0] == board[1][1][1] &&
        board[1][1][1] == board[2][0][2]) {
// Serial.println("l");

        return ((z_Test == 0 && y_Test == 2 && x_Test == 0) || (z_Test == 1 && y_Test == 1 && x_Test == 1) || (z_Test == 2 && y_Test == 0 && x_Test == 2));

    }
    

    // Front-bottom-right to back-top-left ..

    if (board[0][2][2] != Players::None &&
        board[0][2][2] == board[1][1][1] &&
        board[1][1][1] == board[2][0][0]) {
// Serial.println("m");

        return ((z_Test == 0 && y_Test == 2 && x_Test == 2) || (z_Test == 1 && y_Test == 1 && x_Test == 1) || (z_Test == 2 && y_Test == 0 && x_Test == 0));

    }
    
    return Players::None;

}


void swapToPlayerMove(uint8_t x, uint8_t y, uint8_t z) {

	cursor[Constants::X_Axis] = x;
	cursor[Constants::Y_Axis] = y;
	cursor[Constants::Z_Axis] = z;

	comp_cursor[Constants::X_Axis] = x;
	comp_cursor[Constants::Y_Axis] = y;
	comp_cursor[Constants::Z_Axis] = z;

	playerOrComp = 0;

}



void computerMove() {
    

    // Strategy 1: Try to win ..

    for (uint8_t z = 0; z < 3; z++) {

        for (uint8_t y = 0; y < 3; y++) {

            for (uint8_t x = 0; x < 3; x++) {

                if (board[z][y][x] == Players::None) {

                    board[z][y][x] = Players::Crosses;

                    if (checkForWin() != Players::None) {

						swapToPlayerMove(x, y, z);
                        return; // Winning move found
                    }

					board[z][y][x] = Players::None;

                }

            }

        }

    }
    

    // Strategy 2: Block player from winning ..

    for (uint8_t z = 0; z < 3; z++) {

        for (uint8_t y = 0; y < 3; y++) {

            for (uint8_t x = 0; x < 3; x++) {

                if (board[z][y][x] == Players::None) {

                    board[z][y][x] = Players::Noughts;

                    if (checkForWin() != Players::None) {

						swapToPlayerMove(x, y, z);
                        board[z][y][x] = Players::Crosses;
                        return; // Block found

                    }

					board[z][y][x] = Players::None;

                }

            }

        }

    }
    

    // Strategy 3: Take center if available ..

    if (board[1][1][1] == Players::None) {

		swapToPlayerMove(1, 1, 1);
        board[1][1][1] = Players::Crosses;
        return;

    }
    

    // Strategy 4: Take corners ..

    uint8_t corners[8][3] = {
        {0,0,0}, {0,0,2}, {0,2,0}, {0,2,2},
        {2,0,0}, {2,0,2}, {2,2,0}, {2,2,2}
    };
    
    for (uint8_t i = 0; i < 8; i++) {

        uint8_t z = corners[i][0];
        uint8_t y = corners[i][1];
        uint8_t x = corners[i][2];

        if (board[z][y][x] == Players::None) {

			swapToPlayerMove(x, y, z);
            board[z][y][x] = Players::Crosses;
            return;
        }

    }
    

    // Strategy 5: Take any available space ..

    for (uint8_t z = 0; z < 3; z++) {

        for (uint8_t y = 0; y < 3; y++) {

            for (uint8_t x = 0; x < 3; x++) {

                if (board[z][y][x] == Players::None) {

					swapToPlayerMove(x, y, z);
                    board[z][y][x] = Players::Crosses;
                    return;

                }

            }

        }

    }

}