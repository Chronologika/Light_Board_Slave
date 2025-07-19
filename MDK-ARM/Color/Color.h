#ifndef _COLOR_H
#define _COLOR_H

#include <stdint.h>

typedef struct
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
} RGB_Color_t;

typedef enum
{
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_WHITE,
    COLOR_BLACK,
    COLOR_YELLOW,
    COLOR_CYAN,
    COLOR_MAGENTA,
    COLOR_ORANGE,
    COLOR_PINK,
    COLOR_GOLD,
    COLOR_LIME,
    COLOR_MINT,
    COLOR_VIOLET,
    COLOR_INDIGO,
    COLOR_WARM_WHITE,
    COLOR_COOL_WHITE,
    COLOR_LIGHT_BLUE,
    COLOR_GRAY,
    COLOR_CORAL,
    COLOR_COUNT
} ColorName_t;

extern const RGB_Color_t Color_Table[COLOR_COUNT];

#endif
