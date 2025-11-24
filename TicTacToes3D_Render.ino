#include <Arduboy2.h>

void renderBoard() {

    switch (view) {
        
        case ViewMode::Left:

            Sprites::drawOverwrite(0, 0, Images::Background_L, 0);
            Sprites::drawOverwrite(31, Constants::Z_Axis_Offset + (cursor[Constants::Z_Axis] * Constants::Z_Axis_Height), Images::Background_M, 0);

			switch (cursor[Constants::Z_Axis]) {
			
				case 0:

					switch (cursor[Constants::X_Axis]) {
					
						case 0:
							Sprites::drawOverwrite(Constants::Cube_L_X, Constants::Cube_L_Y, Images::Cube_00_L, 0);
							break;
					
						case 1:
							Sprites::drawOverwrite(Constants::Cube_L_X, Constants::Cube_L_Y, Images::Cube_10_L, 0);
							break;
					
						case 2:
							Sprites::drawOverwrite(Constants::Cube_L_X, Constants::Cube_L_Y, Images::Cube_20_L, 0);
							break;

					}

					break;

				case 1:

					switch (cursor[Constants::X_Axis]) {
					
						case 0:
							Sprites::drawOverwrite(Constants::Cube_L_X, Constants::Cube_L_Y, Images::Cube_01_L, 0);
							break;
					
						case 1:
							Sprites::drawOverwrite(Constants::Cube_L_X, Constants::Cube_L_Y, Images::Cube_11_L, 0);
							break;
					
						case 2:
							Sprites::drawOverwrite(Constants::Cube_L_X, Constants::Cube_L_Y, Images::Cube_21_L, 0);
							break;

					}

					break;

				case 2:

					switch (cursor[Constants::X_Axis]) {
					
						case 0:
							Sprites::drawOverwrite(Constants::Cube_L_X, Constants::Cube_L_Y, Images::Cube_02_L, 0);
							break;
					
						case 1:
							Sprites::drawOverwrite(Constants::Cube_L_X, Constants::Cube_L_Y, Images::Cube_12_L, 0);
							break;
					
						case 2:
							Sprites::drawOverwrite(Constants::Cube_L_X, Constants::Cube_L_Y, Images::Cube_22_L, 0);
							break;

					}

					break;

			}


			// Render cursor ..

            renderCursor();


			// Render pieces ..

			renderPieces_L();
			renderPieces_M(0, Constants::Z_Axis_Offset, false);


            // Status

            Sprites::drawOverwrite(99, 0, Images::Status, 0);

            if (comp_cursor[0] != 255) {

                Sprites::drawOverwrite(117, -1, Images::Numbers, comp_cursor[2]);
                Sprites::drawOverwrite(121, -1, Images::Numbers, comp_cursor[1]);
                Sprites::drawOverwrite(125, -1, Images::Numbers, comp_cursor[0]);

            }

            Sprites::drawOverwrite(117, 6, Images::Numbers, cursor[2]);
            Sprites::drawOverwrite(121, 6, Images::Numbers, cursor[1]);
            Sprites::drawOverwrite(125, 6, Images::Numbers, cursor[0]);

            break;
    
        case ViewMode::Middle:

            Sprites::drawOverwrite(0, 0, Images::Background_L, 0);
            Sprites::drawOverwrite(31, Constants::Z_Axis_Offset + (cursor[Constants::Z_Axis] * Constants::Z_Axis_Height), Images::Background_M, 0);
            Sprites::drawOverwrite(95, 0, Images::Background_R, 0);


			// Render cursor ..

            renderCursor();


			// Render pieces ..

			renderPieces_L();
			renderPieces_M(0, Constants::Z_Axis_Offset, false);
			renderPieces_R();



            // Status

            {
                uint8_t x = (viewControl == ViewControl::Left ? 99 : 0);
                Sprites::drawOverwrite(x, 52, Images::Status, 0);

                if (comp_cursor[0] != 255) {

                    Sprites::drawOverwrite(x + 18, 51, Images::Numbers, comp_cursor[2]);
                    Sprites::drawOverwrite(x + 22, 51, Images::Numbers, comp_cursor[1]);
                    Sprites::drawOverwrite(x + 26, 51, Images::Numbers, comp_cursor[0]);

                }

                Sprites::drawOverwrite(x + 18, 58, Images::Numbers, cursor[2]);
                Sprites::drawOverwrite(x + 22, 58, Images::Numbers, cursor[1]);
                Sprites::drawOverwrite(x + 26, 58, Images::Numbers, cursor[0]);
            
            }

            break;

        case ViewMode::Right:

            Sprites::drawOverwrite(31, Constants::Z_Axis_Offset + (cursor[Constants::Z_Axis] * Constants::Z_Axis_Height), Images::Background_M, 0);
            Sprites::drawOverwrite(95, 0, Images::Background_R, 0);

			switch (cursor[Constants::Z_Axis]) {
			
				case 0:

					switch (cursor[Constants::Y_Axis]) {
					
						case 0:
							Sprites::drawOverwrite(Constants::Cube_R_X, Constants::Cube_R_Y, Images::Cube_00_R, 0);
							break;
					
						case 1:
							Sprites::drawOverwrite(Constants::Cube_R_X, Constants::Cube_R_Y, Images::Cube_10_R, 0);
							break;
					
						case 2:
							Sprites::drawOverwrite(Constants::Cube_R_X, Constants::Cube_R_Y, Images::Cube_20_R, 0);
							break;

					}

					break;

				case 1:

					switch (cursor[Constants::Y_Axis]) {
					
						case 0:
							Sprites::drawOverwrite(Constants::Cube_R_X, Constants::Cube_R_Y, Images::Cube_01_R, 0);
							break;
					
						case 1:
							Sprites::drawOverwrite(Constants::Cube_R_X, Constants::Cube_R_Y, Images::Cube_11_R, 0);
							break;
					
						case 2:
							Sprites::drawOverwrite(Constants::Cube_R_X, Constants::Cube_R_Y, Images::Cube_21_R, 0);
							break;

					}

					break;

				case 2:

					switch (cursor[Constants::Y_Axis]) {
					
						case 0:
							Sprites::drawOverwrite(Constants::Cube_R_X, Constants::Cube_R_Y, Images::Cube_02_R, 0);
							break;
					
						case 1:
							Sprites::drawOverwrite(Constants::Cube_R_X, Constants::Cube_R_Y, Images::Cube_12_R, 0);
							break;
					
						case 2:
							Sprites::drawOverwrite(Constants::Cube_R_X, Constants::Cube_R_Y, Images::Cube_22_R, 0);
							break;

					}

					break;

			}

            if (arduboy.frameCount % 32 < 16) {

              	Sprites::drawExternalMask(52 + Background_M_Cursor_X[cursor[Constants::X_Axis]] + Background_M_Cursor_X_YOffset[cursor[Constants::Y_Axis]], Constants::Z_Axis_Offset + (cursor[Constants::Z_Axis] * Constants::Z_Axis_Height) + Background_M_Cursor_Y[cursor[Constants::Y_Axis]]+ Background_M_Cursor_Y_XOffset[cursor[Constants::X_Axis]], Images::Background_M_Cursor, &Images::Background_M_Cursor_Mask[2], 0, 0);
              	Sprites::drawExternalMask(115 - Background_LR_Cursor_X[2 - cursor[Constants::X_Axis]], Background_LR_Cursor_Z_XOffset[2 - cursor[Constants::X_Axis]] + Background_LR_Cursor_Z[cursor[Constants::Z_Axis]], Images::Background_R_Cursor, Images::Background_R_Cursor_Mask, 0, 0);


            }


			// Render cursor ..

			renderCursor();


			// Render pieces ..

			renderPieces_M(0, Constants::Z_Axis_Offset, false);
			renderPieces_R();


            // Status
            
            Sprites::drawOverwrite(0, 0, Images::Status, 0);

            if (comp_cursor[0] != 255) {

                Sprites::drawOverwrite(18, -1, Images::Numbers, comp_cursor[2]);
                Sprites::drawOverwrite(22, -1, Images::Numbers, comp_cursor[1]);
                Sprites::drawOverwrite(26, -1, Images::Numbers, comp_cursor[0]);

            }

            Sprites::drawOverwrite(18, 6, Images::Numbers, cursor[2]);
            Sprites::drawOverwrite(22, 6, Images::Numbers, cursor[1]);
            Sprites::drawOverwrite(26, 6, Images::Numbers, cursor[0]);

            break;

    }


	switch (viewControl) {
		
		case ViewControl::Left:
			arduboy.drawFastHLine(0,63,31);
			break;
		
		case ViewControl::Middle:
			arduboy.drawFastHLine(33,63,60);
			break;
		
		case ViewControl::Right:
			arduboy.drawFastHLine(95,63,32);
			break;

	}

}


void renderPieces_L() {

	for (uint8_t z = 0; z <= 2; z++) {

		for (uint8_t y = 0; y <= 2; y++) {

			if (board[z][y][cursor[Constants::X_Axis]] == Players::Noughts) {

				Sprites::drawSelfMasked(3 + Background_LR_Cursor_X[2 - y], 6 + Background_LR_Cursor_Z_XOffset[2 - y] + Background_LR_Cursor_Z[z], Images::Nought_L, 0);
			
			}

			if (board[z][y][cursor[Constants::X_Axis]] == Players::Crosses) {

				Sprites::drawSelfMasked(3 + Background_LR_Cursor_X[2 - y], 5 + Background_LR_Cursor_Z_XOffset[2 - y] + Background_LR_Cursor_Z[z], Images::Cross_L, 0);
			
			}

		}

	}
			
}

void renderPieces_M(uint8_t xOffset, int8_t zOffset, bool checkPieces) {
    
    xOffset = xOffset + (checkPieces ? 4 : 0);

	for (uint8_t x = 0; x <= 2; x++) {

		for (uint8_t y = 0; y <= 2; y++) {

            bool winningPiece = checkForWinningPiece(cursor[Constants::Z_Axis], y, x);

			if (board[cursor[Constants::Z_Axis]][y][x] == Players::Noughts) {

                if (!checkPieces || !winningPiece || (checkPieces && (arduboy.frameCount %32 < 16) && winningPiece)) {
    				Sprites::drawSelfMasked(59 - xOffset + Background_M_Cursor_X[x] + Background_M_Cursor_X_YOffset[y], zOffset + (cursor[Constants::Z_Axis] * Constants::Z_Axis_Height) + 3 + Background_M_Cursor_Y[y]+ Background_M_Cursor_Y_XOffset[x], Images::Nought_M, 0);
                }
			
			}

			if (board[cursor[Constants::Z_Axis]][y][x] == Players::Crosses) {

                if (!checkPieces || !winningPiece || (checkPieces && (arduboy.frameCount %32 < 16) && winningPiece)) {
    				Sprites::drawSelfMasked(59 - xOffset + Background_M_Cursor_X[x] + Background_M_Cursor_X_YOffset[y], zOffset + (cursor[Constants::Z_Axis] * Constants::Z_Axis_Height) + 3 + Background_M_Cursor_Y[y]+ Background_M_Cursor_Y_XOffset[x], Images::Cross_M, 0);
                }
			
			}

		}

	}

}

void renderPieces_R() {

	for (uint8_t z = 0; z <= 2; z++) {

		for (uint8_t x = 0; x <= 2; x++) {

			if (board[z][cursor[Constants::Y_Axis]][x] == Players::Noughts) {

				Sprites::drawSelfMasked(118 - Background_LR_Cursor_X[2 - x], 6 + Background_LR_Cursor_Z_XOffset[2 - x] + Background_LR_Cursor_Z[z], Images::Nought_R, 0);
			
			}

			if (board[z][cursor[Constants::Y_Axis]][x] == Players::Crosses) {

				Sprites::drawSelfMasked(118 - Background_LR_Cursor_X[2 - x], 6 + Background_LR_Cursor_Z_XOffset[2 - x] + Background_LR_Cursor_Z[z], Images::Cross_R, 0);
			
			}

		}

	}
			
}

void renderCursor() {

	if (arduboy.frameCount % 32 < 16) {

		switch (viewControl) {

			case ViewControl::Left:
				if (view != ViewMode::Right)	Sprites::drawSelfMasked(Background_LR_Cursor_X[2 - cursor[Constants::Y_Axis]], Background_LR_Cursor_Z_XOffset[2 - cursor[Constants::Y_Axis]] + Background_LR_Cursor_Z[cursor[Constants::Z_Axis]], Images::Background_L_Cursor_Mask, 0);
				Sprites::drawExternalMask(52 + Background_M_Cursor_X[cursor[Constants::X_Axis]] + Background_M_Cursor_X_YOffset[cursor[Constants::Y_Axis]], Constants::Z_Axis_Offset + (cursor[Constants::Z_Axis] * Constants::Z_Axis_Height) + Background_M_Cursor_Y[cursor[Constants::Y_Axis]]+ Background_M_Cursor_Y_XOffset[cursor[Constants::X_Axis]], Images::Background_M_Cursor, &Images::Background_M_Cursor_Mask[2], 0, 0);
				if (view != ViewMode::Left)		Sprites::drawExternalMask(115 - Background_LR_Cursor_X[2 - cursor[Constants::X_Axis]], Background_LR_Cursor_Z_XOffset[2 - cursor[Constants::X_Axis]] + Background_LR_Cursor_Z[cursor[Constants::Z_Axis]], Images::Background_R_Cursor, &Images::Background_R_Cursor_Mask[2], 0, 0);
				break;

			case ViewControl::Middle:
				if (view != ViewMode::Right)	Sprites::drawExternalMask(Background_LR_Cursor_X[2 - cursor[Constants::Y_Axis]], Background_LR_Cursor_Z_XOffset[2 - cursor[Constants::Y_Axis]] + Background_LR_Cursor_Z[cursor[Constants::Z_Axis]], Images::Background_L_Cursor, &Images::Background_L_Cursor_Mask[2], 0, 0);
				Sprites::drawSelfMasked(52 + Background_M_Cursor_X[cursor[Constants::X_Axis]] + Background_M_Cursor_X_YOffset[cursor[Constants::Y_Axis]], Constants::Z_Axis_Offset + (cursor[Constants::Z_Axis] * Constants::Z_Axis_Height) + Background_M_Cursor_Y[cursor[Constants::Y_Axis]]+ Background_M_Cursor_Y_XOffset[cursor[Constants::X_Axis]], Images::Background_M_Cursor_Mask, 0);
				if (view != ViewMode::Left)		Sprites::drawExternalMask(115 - Background_LR_Cursor_X[2 - cursor[Constants::X_Axis]], Background_LR_Cursor_Z_XOffset[2 - cursor[Constants::X_Axis]] + Background_LR_Cursor_Z[cursor[Constants::Z_Axis]], Images::Background_R_Cursor, &Images::Background_R_Cursor_Mask[2], 0, 0);
				break;

			case ViewControl::Right:
				if (view != ViewMode::Right)	Sprites::drawExternalMask(Background_LR_Cursor_X[2 - cursor[Constants::Y_Axis]], Background_LR_Cursor_Z_XOffset[2 - cursor[Constants::Y_Axis]] + Background_LR_Cursor_Z[cursor[Constants::Z_Axis]], Images::Background_L_Cursor, &Images::Background_L_Cursor_Mask[2], 0, 0);
				Sprites::drawExternalMask(52 + Background_M_Cursor_X[cursor[Constants::X_Axis]] + Background_M_Cursor_X_YOffset[cursor[Constants::Y_Axis]], Constants::Z_Axis_Offset + (cursor[Constants::Z_Axis] * Constants::Z_Axis_Height) + Background_M_Cursor_Y[cursor[Constants::Y_Axis]]+ Background_M_Cursor_Y_XOffset[cursor[Constants::X_Axis]], Images::Background_M_Cursor, &Images::Background_M_Cursor_Mask[2], 0, 0);
				if (view != ViewMode::Left)		Sprites::drawSelfMasked(115 - Background_LR_Cursor_X[2 - cursor[Constants::X_Axis]], Background_LR_Cursor_Z_XOffset[2 - cursor[Constants::X_Axis]] + Background_LR_Cursor_Z[cursor[Constants::Z_Axis]], Images::Background_R_Cursor_Mask, 0);
				break;

		}

	}

}