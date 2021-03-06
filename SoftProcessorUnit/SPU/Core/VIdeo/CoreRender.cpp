//
//  CoreRender.cpp
//  SPU
//
//  Created by Александр Дремов on 24.10.2020.
//

#include "CoreRender.hpp"
#include <stdio.h>
#include <stdlib.h>

void renderVRAM(SPUCore* core){
//    printf("\x1b[2J");
    printf("\x1b[H");
    for(int h = 0; h < SCREEN_HEIGHT; h++){
        for(int w = 0; w < SCREEN_WIDTH; w++){
            putchar(*(((char*)core->RAM)
                      + VRAM_OFFSET * sizeof(double)
                      + (SCREEN_WIDTH * h)
                      + w));
        }
        putchar(10);
    }
}

void fillBlank(SPUCore* core){
    for(int h = 0; h < SCREEN_HEIGHT; h++){
        for(int w = 0; w < SCREEN_WIDTH; w++){
            *(((char*)core->RAM) + VRAM_OFFSET * sizeof(double) + (SCREEN_WIDTH * h) + w) = ' ';
        }
    }
}

int vramSetPixel(SPUCore* core, int x, int y, char val) {
    char* start = ((char*)core->RAM) + VRAM_OFFSET * sizeof(double);
    start += y * SCREEN_WIDTH;
    start += x;
    *start = val;
    return 1;
}
