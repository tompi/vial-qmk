#include "matrix.h"
#include "quantum.h"
#include "rgblight.h"
#include "encoder.h"

#define COL_SHIFTER ((uint16_t)1)

#define ENC_ROW 3
#define ENC_A_COL 2
#define ENC_B_COL 4
#define ENC_BUTTON_COL 0

static bool colABPressed   = false;
static uint16_t turns = 0;
static bool last_turn_clockwise;

void turned(bool clockwise) {
	if (clockwise != last_turn_clockwise) {
        // Switched way, reset counter
        last_turn_clockwise = clockwise;
        turns = 0;
    }
    turns++;
    if (!(turns%ENCODER_RESOLUTION == 0)) {
        return;
    }

	encoder_exec_mapping(0, clockwise);
}

void fix_encoder_action(matrix_row_t current_matrix[]) {
    matrix_row_t encoder_row = current_matrix[ENC_ROW];

    // Check which way the encoder is turned:
    bool colA = encoder_row & (COL_SHIFTER << ENC_A_COL);
    bool colB = encoder_row & (COL_SHIFTER << ENC_B_COL);

    if (colA && colB) {
        colABPressed = true;
    } else if (colA) {
        if (colABPressed) {
            // A+B followed by A means clockwise
            colABPressed = false;
            turned(true);
        }
    } else if (colB) {
        if (colABPressed) {
            // A+B followed by B means counter-clockwise
            colABPressed = false;
            turned(false);
        }
    }
}
