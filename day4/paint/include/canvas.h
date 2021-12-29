#pragma once
#include <stdlib.h>

typedef struct
{
    int width;
    int height;
    char **canvas;
    char pen;
} Canvas;



Canvas *init_canvas(int width, int height, char pen);
void reset_canvas(Canvas *c);
void print_canvas(Canvas *c);
void free_canvas(Canvas *c);

// display functions
void rewind_screen(unsigned int line); 
void clear_command(void);
void clear_screen(void);



