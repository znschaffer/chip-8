#include "core.h"

#include <string.h>

#include "stdlib.h"
static void execute_opcode(struct Core *state, uint16_t op);
static void push(struct Core *state, uint16_t val);
static uint16_t pop(struct Core *state);
static void unimplemented(struct Core *state);
static void print_screen_memory(struct Core *state);

struct Core {
	uint8_t vReg[NUM_REGS];
	uint16_t iReg;
	uint16_t sp;
	uint16_t pc;
	uint16_t stack[STACK_SIZE];
	uint8_t dt;
	bool screen[SCREEN_WIDTH * SCREEN_HEIGHT];
	bool keys[NUM_KEYS];
	uint8_t st;
	uint8_t ram[RAM_SIZE];
	bool advance_pc;
};

struct Core *new_core(void)
{
	struct Core *s = calloc(sizeof(struct Core), 1);
	s->pc = 0x200;
	s->advance_pc = true;

	// Load fontset into ram
	for (int i = 0; i < FONTSET_SIZE; i++) {
		s->ram[i] = FONT_SET[i];
	}
	return s;
}

inline bool *get_screen(struct Core *state)
{
	return state->screen;
}

/// Push an item onto the state's stack
static inline void push(struct Core *state, uint16_t val)
{
	state->stack[state->sp] = val;
	state->sp++;
}

/// Pop an item off the state's stack and return it
static inline uint16_t pop(struct Core *state)
{
	state->sp--;
	return state->stack[state->sp];
}

static inline void unimplemented(struct Core *state)
{
	uint8_t *op = &state->ram[state->pc];
	int first = (*op & 0xf0) >> 4;
	printf("%04x %02x %02x ", state->pc, first, *op & 0x00ff);
	printf("%01x not handled yet", first);
}

static inline uint16_t fetch_opcode(struct Core *state)
{
	uint8_t high = state->ram[state->pc];
	uint8_t low = state->ram[state->pc + 1];
	uint16_t op = (high << 8) | low;
	state->pc += 2;
	return op;
}

void tick(struct Core *state)
{
	// Fetch
	uint16_t op = fetch_opcode(state);
	// Decode & Execute
	execute_opcode(state, op);
}

/// executes the opcode at current program counter
static inline void execute_opcode(struct Core *state, uint16_t op)
{
	// first nib (4 bits)
	uint8_t first_digit = (op & 0xF000) >> 12;
	// second nib (4 bits)
	uint8_t second_digit = (op & 0x0F00) >> 8;
	// third nib (4 bits)
	uint8_t third_digit = (op & 0x00F0) >> 4;
	// fourth nib (4 bits)
	uint8_t fourth_digit = (op & 0x000F);

	// printf("%01x %01x %01x %01x\n", first_digit, second_digit, third_digit,
	//        fourth_digit);

	switch (first_digit) {
	case 0x00:
		switch (third_digit << 4 | fourth_digit) {
		case 0xEE:
			if (DEBUG) {
				printf("%04x %04x RET\n", state->pc, op);
			}
			state->pc = pop(state);
			break;
		case 0xE0:
			if (DEBUG) {
				printf("%04x %04x CLS\n", state->pc, op);
			}
			memset(state->screen, 0, sizeof(state->screen));
			break;
		}
		break;
	case 0x01: {
		uint16_t addr = (second_digit << 8) | (third_digit << 4) |
				fourth_digit;
		state->pc = addr;
		if (DEBUG) {
			printf("%04x %04x JP %03x\n", state->pc, op, addr);
		}
	} break;
	case 0x06: {
		uint8_t val = (third_digit << 4) | fourth_digit;
		state->vReg[second_digit] = val;
		if (DEBUG) {
			printf("%04x %04x LD V%01x, %02x\n", state->pc, op,
			       second_digit, val);
		}
	} break;
	case 0x07: {
		uint8_t val = (third_digit << 4) | fourth_digit;
		state->vReg[second_digit] += val;
		if (DEBUG) {
			printf("%04x %04x ADD V%01x, %02x\n", state->pc, op,
			       second_digit, val);
		}
	} break;
	case 0x0A: {
		uint16_t addr = (second_digit << 8) | (third_digit << 4) |
				fourth_digit;
		state->iReg = addr;
		if (DEBUG) {
			printf("%04x %04x LD I, %03x\n", state->pc, op, addr);
		}
	} break;
	case 0x0D: {
		uint8_t x_coordinate = state->vReg[second_digit];
		uint8_t y_coordinate = state->vReg[third_digit];
		bool flipped = false;
		for (int y_line = 0; y_line < fourth_digit; y_line++) {
			uint8_t pixels = state->ram[state->iReg + y_line];
			for (int x_line = 0; x_line < 8; x_line++) {
				if ((pixels & (0b10000000 >> x_line)) != 0) {
					uint16_t x = (x_coordinate + x_line) %
						     SCREEN_WIDTH;
					uint16_t y = (y_coordinate + y_line) %
						     SCREEN_HEIGHT;
					uint16_t idx = x + (SCREEN_WIDTH * y);
					flipped |= state->screen[idx];
					state->screen[idx] ^= true;
				}
			}
		}
		if (flipped)
			state->vReg[0xF] = 1;
		else
			state->vReg[0xF] = 0;
	} break;
	default:
		if (DEBUG) {
			printf("%04x %04x UNK\n", state->pc, op);
		}
	}
}

void load_rom(struct Core *state, FILE *file)
{
	fseek(file, 0L, SEEK_END);
	uint16_t fsize = ftell(file);
	fseek(file, 0L, SEEK_SET);
	fread(state->ram + 0x200, fsize, 1, file);
	fclose(file);
}

static void print_screen_memory(struct Core *state)
{
	for (int y = 0; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			printf("%d", state->screen[x + (SCREEN_WIDTH * y)]);
		}
		printf("\n");
	}
}
