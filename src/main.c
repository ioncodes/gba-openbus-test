#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "common.h"

#define DISPCNT ((volatile uint16_t*)0x04000000)
#define VRAM ((volatile uint16_t*)0x06000000)
#define MODE_3 0x0003
#define BG2_ENABLE 0x0400

void display_result(bool failed) {
    *DISPCNT = MODE_3 | BG2_ENABLE;

    uint16_t color = failed ? 0x001F : 0x03E0;

    for (int i = 0; i < 240 * 160; i++) {
        VRAM[i] = color;
    }
}

void __attribute__((optimize("O0"))) main(void) {
    uint32_t openbus_bios = 0;
    bool failed = false;
    
    openbus_bios = *(uint32_t*)(0x00000000);
    failed |= openbus_bios != 0xE129F000;

    asm("mov r0, #6\nmov r1, #3\nmov r11, #0x3000000\nswi #0x60000");
    openbus_bios = *(uint32_t*)(0x00000000);
    failed |= openbus_bios != 0xE3A02004;

    display_result(failed);

    while(true);
}