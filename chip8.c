#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void disassemble_opcode(uint8_t *buffer, uint16_t pc);

struct Chip8State {
  uint8_t V[16];
  uint16_t I;
  uint16_t sp;
  uint16_t pc;
  uint8_t delay;
  uint8_t sound;
  uint8_t *memory;
  uint8_t *screen; // this is memory[0xF00];
  bool advance_pc;
};

///
struct Chip8State *init_chip8(void) {
  struct Chip8State *s = calloc(sizeof(struct Chip8State), 1);

  s->memory = calloc(1024 * 4, 1);
  s->screen = &s->memory[0xf00];
  s->sp = 0xfa0;
  s->pc = 0x200;
  s->advance_pc = true;

  return s;
}

///
void unimplemented(struct Chip8State *state) {
  uint8_t *op = &state->memory[state->pc];
  int first = (*op & 0xf0) >> 4;
  printf("%04x %02x %02x ", state->pc, first, *op & 0x00ff);
  printf("%01x not handled yet", first);
}

/// executes the opcode at current program counter
void execute_opcode(struct Chip8State *state) {

  uint8_t *op = &state->memory[state->pc];
  uint16_t addr = ((*op << 8) | *(op + 1)) & 0x0fff;

  int first = (*op & 0xf0) >> 4;
  switch (first) {
  case 0x00:
    unimplemented(state);
    break;
  case 0x01: {
    state->pc = addr;
    state->advance_pc = false;
  } break;
  case 0x02:
    break;
  case 0x06:
    state->V[(*op & 0x0f)] = *(op + 1);
    break;
  case 0x07:
    state->V[(*op & 0x0f)] += *(op + 1);
    break;
  case 0x0a:
    state->I = addr;
    break;
  }
}

int main(int argc, char **argv) {
  (void)argc;
  FILE *f = fopen(argv[1], "rb");
  if (f == NULL) {
    printf("error: Couldn't open %s\n", argv[1]);
    exit(1);
  }
  // Get file size
  fseek(f, 0L, SEEK_END);
  uint16_t fsize = ftell(f);
  fseek(f, 0L, SEEK_SET);
  struct Chip8State *state = init_chip8();
  fread(state->memory + 0x200, fsize, 1, f);
  fclose(f);

  while (state->pc < (fsize + 0x200)) {
    disassemble_opcode(state->memory, state->pc);
    execute_opcode(state);
    if (state->advance_pc) {
      state->pc += 2;
    }
    printf("\n");
  }

  return 0;
}

/// processes a 2-byte opcode into its two halves
void disassemble_opcode(uint8_t *buffer, uint16_t pc) {
  uint8_t *code = &buffer[pc];
  uint8_t first = (code[0] >> 4);
  printf("%04x %02x %02x ", pc, code[0], code[1]);
  switch (first) {
  case 0x00:
    switch (code[1]) {
    case 0xE0:
      printf("%-10s", "CLS");
      break;
    case 0xEE:
      printf("%-10s", "RTS");
      break;
    }
    break;
  case 0x01: {
    uint8_t reg = code[0] & 0x0f;
    printf("%-10s #$%01x%02x", "JUMP", reg, code[1]);
  } break;

  case 0x02: {
    uint8_t reg = code[0] & 0x0f;
    printf("%-10s #$%01x%02x", "CALL", reg, code[1]);
  } break;
  case 0x03: {
    uint8_t reg = code[0] & 0x0f;
    printf("%-10s V%0x, #$%02x", "SKIP.EQ", reg, code[1]);
  } break;
  case 0x04: {
    uint8_t reg = code[0] & 0x0f;
    printf("%-10s V%0x, #$%02x", "SKIP.NE", reg, code[1]);
  } break;
  case 0x05: {
    uint8_t reg1 = code[0] & 0x0f;
    uint8_t reg2 = code[1] >> 2;
    printf("%-10s V%0x, V%01x", "SKIP.EQ", reg1, reg2);
  } break;
  case 0x06: {
    uint8_t reg = code[0] & 0x0f;
    printf("%-10s V%01X, #$%02x", "MVI", reg, code[1]);
  } break;
  case 0x07: {
    uint8_t reg = code[0] & 0x0f;
    printf("%-10s V%01X, %02x", "ADD", reg, code[1]);
  } break;
  case 0x08:
    printf("8 not handled yet");
    break;
  case 0x09: {
    uint8_t reg1 = code[0] & 0x0f;
    uint8_t reg2 = code[1] >> 2;
    printf("%-10s V%01X, V%01x", "SKIP.NE", reg1, reg2);
  } break;
  case 0x0a: {
    uint8_t addresshi = code[0] & 0x0f;
    printf("%-10s I, #$%01x%02x", "MVI", addresshi, code[1]);
  } break;
  case 0x0b: {
    uint8_t reg = code[0] & 0x0f;
    printf("%-10s #$%01x%02x(V0)", "JUMP", reg, code[1]);
  } break;
  case 0x0c:
    printf("c not handled yet");
    break;
  case 0x0d:
    printf("d not handled yet");
    break;
  case 0x0e:
    printf("e not handled yet");
    break;
  case 0x0f:
    printf("f not handled yet");
    break;
  }
}
