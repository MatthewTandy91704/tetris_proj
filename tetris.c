#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "tetris.h"

/*
 *
 */

static inline uint8_t board_get(const uint8_t *vals, int32_t width, int32_t row, int32_t col) {

  int32_t index = row * width + col;
  return vals[index];

} /* board_get() */

/*
 *
 */

static inline void board_set(uint8_t *vals, int32_t width, int32_t row, int32_t col, uint8_t value) {

  int32_t index = row * width + col;
  vals[index] = value;

} /* board_set() */

/*
 *
 */

static inline uint8_t shape_get(const shape_t *shape, int32_t row, int32_t col, int32_t rotation) {

  int32_t side = shape->side;
  switch (rotation) {

    case 0:
      return shape->data[row * side + col];
    case 1:
      return shape->data[(side - col - 1) * side + row];
    case 2:
      return shape->data[(side - row - 1) * side + (side - col - 1)];
    case 3:
      return shape->data[col * side + (side - row - 1)];

  }

  return 0;

} /* shape_get() */

/*
 *
 */

int check_piece_valid(const piece_state_t *piece, const uint8_t *board, int32_t width, int32_t height) {

  const shape_t *shape = shapes + piece->shape_index;
  assert(shape);

  for (int32_t row = 0; row < height; ++row) {

    for (int32_t col = 0; col < width; ++col) {

      uint8_t val = shape_get(shape, row, col, piece->rotation);

      if (val > 0) {

        int32_t board_row = piece->row_offset + row;
        int32_t board_col = piece->col_offset + col;

        if (board_row < 0) {

          return -1;

        }

        if (board_row >= height) {

          return -1;

        }

        if (board_col < 0) {

          return -1;

        }

        if (board_col >= width) {

          return -1;

        }

        if (board_get(board, width, board_row, board_col)) {

          return -1;

        }

      }

    }

  }

  return 0;

} /* check_piece_valid() */

/*
 *
 */

void update_gameplay(state_t *cur_state, const input_t *input) {

  piece_state_t piece = cur_state->piece;

  if (input->left > 0) {

    --piece.col_offset;

  }

  if (input->right > 0) {

    ++piece.col_offset;

  }

  if (input->up > 0) {

    piece.rotation = (piece.rotation + 1) % 4;

  }

  /* if (check_piece_valid(&piece, cur_state->board, WIDTH, HEIGHT) == 0) {

    

  } */

} /* update_gameplay() */

/*
 *
 */

void update_game(state_t *cur_state, const input_t *input) {

  switch (cur_state->cur_phase) {

    case GAME_PHASE_PLAY:
      return update_gameplay(cur_state, input);
      break;

  }

} /* update_game() */

int main() {

  printf("make file works");

  return 0;

}

