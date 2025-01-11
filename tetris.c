#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <SDL.h>
#include "tetris.h"

/*
 *
 */

inline uint8_t board_get(const uint8_t *vals, int32_t width, int32_t row, int32_t col) {

  int32_t index = row * width + col;
  return vals[index];

} /* board_get() */

/*
 *
 */

inline void board_set(uint8_t *vals, int32_t width, int32_t row, int32_t col, uint8_t value) {

  int32_t index = row * width + col;
  vals[index] = value;

} /* board_set() */

/*
 *
 */

inline uint8_t shape_get(const shape_t *shape, int32_t row, int32_t col, int32_t rotation) {

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

  for (int32_t row = 0; row < shape->side; ++row) {

    for (int32_t col = 0; col < shape->side; ++col) {

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

void merge_piece(state_t *cur_state) {

  const shape_t *shape = shapes + cur_state->piece.shape_index;

  for (int32_t row = 0; row < shape->side; ++row) {

    for (int32_t col = 0; col < shape->side; ++col) {

      uint8_t value = shape_get(shape, row, col, cur_state->piece.rotation);
      if (value) {

        int32_t board_row = cur_state->piece.row_offset + row;
        int32_t board_col = cur_state->piece.col_offset + col;

        board_set(cur_state->board, WIDTH, board_row, board_col, value);

      }

    }

  }

} /* merge_piece() */

/*
 *
 */

void soft_drop(state_t *cur_state) {

  ++cur_state->piece.row_offset;

  if (check_piece_valid(&cur_state->piece, cur_state->board, WIDTH, HEIGHT) != 0) {

    --cur_state->piece.row_offset;
    merge_piece(cur_state);

  }

} /* soft_drop() */

/*
 *
 */

void update_gameplay(state_t *cur_state, const input_t *input) {

  piece_state_t piece = cur_state->piece;

  if (input->dleft > 0) {

    --piece.col_offset;

  }

  if (input->dright > 0) {

    ++piece.col_offset;

  }

  if (input->dup > 0) {

    piece.rotation = (piece.rotation + 1) % 4;

  }

  if (check_piece_valid(&piece, cur_state->board, WIDTH, HEIGHT) == 0) {

    cur_state->piece = piece;

  }

  if (input->ddown > 0) {

    

  }

} /* update_gameplay() */

/*
 *
 */

void update_game(state_t *cur_state, const input_t *input) {

  switch (cur_state->cur_phase) {

    case GAME_PHASE_PLAY:
      update_gameplay(cur_state, input);
      break;

  }

} /* update_game() */

/*
 *
 */

void fill_rect(SDL_Renderer *renderer, int32_t x, int32_t y, int32_t width, int32_t height, color_t color) {

  SDL_Rect rect = {0};
  rect.x = x;
  rect.y = y;
  rect.w = width;
  rect.h = height;
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect(renderer, &rect);

} /* fill_rect() */

/*
 *
 */

void draw_rect(SDL_Renderer *renderer, int32_t x, int32_t y, int32_t width, int32_t height, color_t color) {

  SDL_Rect rect = {0};
  rect.x = x;
  rect.y = y;
  rect.w = width;
  rect.h = height;
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawRect(renderer, &rect);

} /* draw_rect() */

/*
 *
 */

void draw_cell(SDL_Renderer *renderer, int32_t row, int32_t col, uint8_t value, int32_t offset_x, int32_t offset_y) {

  color_t base_color = base_colors[value];
  color_t light_color = light_colors[value];
  color_t dark_color = dark_colors[value];

  int32_t edge = GRID_SIZE / 8;
  int32_t x = col * GRID_SIZE + offset_x;
  int32_t y = row * GRID_SIZE + offset_y;

  fill_rect(renderer, x, y, GRID_SIZE, GRID_SIZE, dark_color);
  fill_rect(renderer, x + edge, y, GRID_SIZE - edge, GRID_SIZE - edge, light_color);
  fill_rect(renderer, x + edge, y + edge, GRID_SIZE - edge * 2, GRID_SIZE - edge * 2, base_color);

} /* draw_cell() */

/*
 *
 */

void draw_piece(SDL_Renderer *renderer, const piece_state_t *piece, int32_t offset_x, int32_t offset_y) {

  const shape_t *shape = shapes + piece->shape_index;

  for (int row = 0; row < shape->side; row++) {

    for (int col = 0; col < shape->side; col++) {

      uint8_t value = shape_get(shape, row, col, piece->rotation);

      if (value) {

        draw_cell(renderer, row + piece->row_offset, col + piece->col_offset, value, offset_x, offset_y);

      }

    }

  }

} /* draw_piece() */

/*
 *
 */

void draw_board(SDL_Renderer *renderer, const uint8_t *board, int32_t width, int32_t height, int32_t x_offset, int32_t y_offset) {

  for (int32_t row = 0; row < height; ++row) {

    for (int32_t col = 0; col < width; ++col) {

      uint8_t value = board_get(board, width, row, col);

      if (value) {

        draw_cell(renderer, row, col, value, x_offset, y_offset);

      }

    }

  }

} /* draw_board() */

/*
 *
 */

void render_game(state_t *cur_state, SDL_Renderer *renderer) {

  draw_board(renderer, cur_state->board, WIDTH, HEIGHT, 0, 0);
  draw_piece(renderer, &cur_state->piece, 0, 0);

} /* render_game() */

/*
 *
 */

int WinMain(int argc, char *argv[]) {

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {

    return 1;

  }

  SDL_Window *window = SDL_CreateWindow(
  "Tetris",
  SDL_WINDOWPOS_UNDEFINED,
  SDL_WINDOWPOS_UNDEFINED,
  400,
  720,
  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

  SDL_Renderer *renderer = SDL_CreateRenderer(
  window, -1,
  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  state_t game = {};
  input_t input = {};

  int quit = 0;

  while (quit == 0) {

    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {

      if (e.type == SDL_QUIT) {

        quit = 1;

      }

    }

    int32_t key_count;
    const uint8_t *key_states = SDL_GetKeyboardState(&key_count);

    input_t prev_input = input;

    input.left = key_states[SDL_SCANCODE_LEFT];
    input.right = key_states[SDL_SCANCODE_RIGHT];
    input.up = key_states[SDL_SCANCODE_UP];
    input.down = key_states[SDL_SCANCODE_DOWN];
    input.a = key_states[SDL_SCANCODE_SPACE];

    input.dleft = (int8_t)input.left - (int8_t)prev_input.left;
    input.dright = (int8_t)input.right - (int8_t)prev_input.right;
    input.dup = (int8_t)input.up - (int8_t)prev_input.up;
    input.ddown = (int8_t)input.down - (int8_t)prev_input.down;
    input.da = (int8_t)input.a - (int8_t)prev_input.a;

    //printf("input: left %d, right %d, up %d\n", input.left, input.right, input.up);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    update_game(&game, &input);
    render_game(&game, renderer);

    SDL_RenderPresent(renderer);

  }

  SDL_DestroyRenderer(renderer);
  SDL_Quit();

  printf("make file works");

  return 0;

} /* WinMain() */

