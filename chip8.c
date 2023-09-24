#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void executeOpcode(uint16_t opcode);

int main(int argc, char **argv) {


  uint8_t *memory = malloc(4096 * sizeof(uint8_t));

  if (argc != 2) {
    printf("No rom passed in!\n");
    return 1;
  }

  FILE *rom;
  rom = fopen(argv[1], "rb");
  if (rom) {

  }


  executeOpcode(0x00e0);
  executeOpcode(0x00ee);
  executeOpcode(0x8343);
  return 0;
}

void executeOpcode(uint16_t opcode) {
  uint8_t first = opcode >> 4;
  uint8_t second = opcode & 0x0000FFFF;

  switch(first & 0xF0) {
    case 0x0:
      switch(second) {
        case 0xe0:
          printf("clear\n");
          break;
        case 0xee:
          printf("return\n");
          break;
      }
      break;
    case 0x1:
      break;
    case 0x2:
      break;
    case 0x3:
      break;
    case 0x4:
      break;
    case 0x5:
      break;
    case 0x6:
      break;
    case 0x7:
      break;
    case 0x8:
      break;
    case 0x9:
      break;
    case 0xa:
      break;
    case 0xb:
      break;
    case 0xc:
      break;
    case 0xd:
      break;
    case 0xe:
      break;
    case 0xf:
      break;
    default:
      break;
  }
}
