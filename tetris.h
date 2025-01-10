#ifndef TETRIS_H
#define TETRIS_H

#define WIDTH (10)
#define HEIGHT (22)
#define VISIBLE_HEIGHT (20)
#define GRID_SIZE (30)

typedef struct color {

  uint8_t r; /* red */
  uint8_t g; /* green */
  uint8_t b; /* blue */
  uint8_t a; /* alpha/opacity */

} color_t;

inline color_t make_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {

  color_t result;
  result.r = r;
  result.g = g;
  result.b = b;
  result.a = a;
  return result;

} /* make_color() */

const color_t base_colors[] = {

  {0x2D, 0x99, 0x99, 0xFF}, /* teal   */
  {0x99, 0x99, 0x2D, 0xFF}, /* yellow */
  {0x99, 0x2D, 0x99, 0xFF}, /* purple */
  {0x2D, 0x99, 0x51, 0xFF}, /* green  */
  {0x99, 0x2D, 0x2D, 0xFF}, /* red    */
  {0x2D, 0X63, 0X99, 0XFF}  /* blue   */

};

const color_t light_colors[] = {

  {0x44, 0xE5, 0xE5, 0xFF}, /* light teal   */
  {0xE5, 0xE5, 0x44, 0xFF}, /* light yellow */
  {0xE5, 0x44, 0xE5, 0xFF}, /* light purple */
  {0x44, 0xE5, 0x7A, 0xFF}, /* light green  */
  {0xE5, 0x44, 0x44, 0xFF}, /* light red    */
  {0x44, 0x95, 0xE5, 0xFF}  /* light blue   */

};

const color_t dark_colors[] = {

  {0x1E, 0x66, 0x66, 0xFF}, /* dark teal   */
  {0x66, 0x66, 0x1E, 0xFF}, /* dark yellow */
  {0x66, 0x66, 0x1E, 0xFF}, /* dark purple */
  {0x1E, 0x66, 0x36, 0xFF}, /* dark green  */
  {0x66, 0x1E, 0x1E, 0xFF}, /* dark red    */
  {0x1E, 0x42, 0x66, 0xFF}  /* dark blue   */

};

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
