#ifndef TETRIS_H
#define TETRIS_H

#define WIDTH (10)
#define HEIGHT (22)
#define VISIBLE_HEIGHT (20)

typedef struct piece_state {

  uint8_t shape_index;
  int32_t row_offset;
  int32_t col_offset;
  int32_t rotation;

} piece_state_t;

typedef enum game_phase {

  GAME_PHASE_PLAY

} game_phase_t;

typedef struct game_state {

  uint8_t board[WIDTH * HEIGHT];
  piece_state_t piece;
  game_phase_t cur_phase;

} state_t;

typedef struct input_state {

  int8_t left;
  int8_t right;
  int8_t up;

} input_t;

typedef struct shape {

  const uint8_t *data;
  const int32_t side;

} shape_t;

inline shape_t shape_make(const uint8_t *data, int32_t side) {

  shape_t result = {data, side};
  return result;

} /* shape_make() */

const uint8_t shape_1[] = {

  0, 0, 0, 0,
  1, 1, 1, 1,
  0, 0, 0, 0,
  0, 0, 0, 0

};

const uint8_t shape_2[] = {

  2, 2,
  2, 2

};

const uint8_t shape_3[] = {

  0, 0, 0,
  3, 3, 3,
  0, 3, 0

};



const shape_t shapes[] = {

  {shape_1, 4},
  {shape_2, 2},
  {shape_3, 3}

};

static inline uint8_t shape_get(const shape_t *shape, int32_t row, int32_t col, int32_t rotation);

static inline uint8_t board_get(const uint8_t *vals, int32_t width, int32_t row, int32_t col);

static inline void board_set(uint8_t *vals, int32_t width, int32_t row, int32_t col, uint8_t value);

void update_gameplay(state_t *state, const input_t *input);

void update_game(state_t *state, const input_t *input);

int check_piece_valid(const piece_state_t *piece, const uint8_t *board, int32_t width, int32_t height);



#endif // TETRIS_H
