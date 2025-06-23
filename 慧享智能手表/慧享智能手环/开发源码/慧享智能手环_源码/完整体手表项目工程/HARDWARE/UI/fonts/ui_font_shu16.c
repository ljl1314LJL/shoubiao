/*******************************************************************************
 * Size: 16 px
 * Bpp: 1
 * Opts: --bpp 1 --size 16 --font C:/Users/featherwit/Desktop/手表ui - 自己字库/assets/AlimamaDongFangDaKai-Regular.ttf -o C:/Users/featherwit/Desktop/手表ui - 自己字库/assets\ui_font_shu16.c --format lvgl -r 0x20-0x7f --symbols 1234567890% --no-compress --no-prefilter
 ******************************************************************************/

#include "../ui.h"

#ifndef UI_FONT_SHU16
#define UI_FONT_SHU16 1
#endif

#if UI_FONT_SHU16

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0xff, 0xc,

    /* U+0022 "\"" */
    0x7a, 0xbc,

    /* U+0023 "#" */
    0x9, 0x6, 0x41, 0xb1, 0xff, 0x12, 0xc, 0x83,
    0x63, 0xfe, 0x26, 0x19, 0x6, 0xc0,

    /* U+0024 "$" */
    0x18, 0x18, 0x7e, 0x7a, 0x7b, 0x78, 0x7c, 0x1f,
    0x1b, 0xdb, 0xff, 0x7e, 0x18,

    /* U+0025 "%" */
    0x78, 0xcc, 0xd8, 0xcd, 0x8c, 0xf0, 0x7a, 0x0,
    0x7e, 0xf, 0x30, 0xf3, 0x1b, 0x33, 0x9e,

    /* U+0026 "&" */
    0x1c, 0x13, 0x9, 0x87, 0x83, 0x83, 0xed, 0xbd,
    0x8e, 0x47, 0x1e, 0xc0,

    /* U+0027 "'" */
    0x69, 0x0,

    /* U+0028 "(" */
    0x33, 0x66, 0xcc, 0xcc, 0xcc, 0xcc, 0x66, 0x33,

    /* U+0029 ")" */
    0xcc, 0x66, 0x33, 0x33, 0x33, 0x33, 0x66, 0xcc,

    /* U+002A "*" */
    0x25, 0x7f, 0xfa, 0x90,

    /* U+002B "+" */
    0x18, 0x30, 0x67, 0xf1, 0x83, 0x6, 0x0,

    /* U+002C "," */
    0xf6,

    /* U+002D "-" */
    0xfc,

    /* U+002E "." */
    0xc0,

    /* U+002F "/" */
    0xc, 0x21, 0x86, 0x18, 0x43, 0xc, 0x31, 0x86,
    0x18, 0x43, 0x0,

    /* U+0030 "0" */
    0x3c, 0x66, 0xe6, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0x63, 0x66, 0x3c,

    /* U+0031 "1" */
    0xfd, 0xb6, 0xdb, 0x6d, 0x80,

    /* U+0032 "2" */
    0x7c, 0xc6, 0x86, 0x6, 0xe, 0xc, 0x1c, 0x38,
    0x70, 0xf0, 0xff,

    /* U+0033 "3" */
    0x3e, 0x63, 0x43, 0x7, 0x3e, 0x7, 0x3, 0x3,
    0x43, 0x47, 0x7c,

    /* U+0034 "4" */
    0x6, 0x7, 0x3, 0x83, 0xc3, 0x63, 0x31, 0x99,
    0xfe, 0x6, 0x3, 0x1, 0x80,

    /* U+0035 "5" */
    0xff, 0x60, 0x60, 0x60, 0x7c, 0x6, 0x3, 0x3,
    0x83, 0xc6, 0x7c,

    /* U+0036 "6" */
    0xe, 0x30, 0x60, 0x7c, 0xe6, 0xc3, 0xc3, 0xc3,
    0xc3, 0x66, 0x3c,

    /* U+0037 "7" */
    0xff, 0x6, 0xe, 0xc, 0xc, 0x1c, 0x18, 0x18,
    0x18, 0x18, 0x38,

    /* U+0038 "8" */
    0x7e, 0xc3, 0xc3, 0xc7, 0x3e, 0x66, 0xc3, 0xc3,
    0xc3, 0xe6, 0x3c,

    /* U+0039 "9" */
    0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0x67, 0x3f, 0x6,
    0x6, 0xc, 0x70,

    /* U+003A ":" */
    0xc0, 0xc,

    /* U+003B ";" */
    0xc0, 0xf6,

    /* U+003C "<" */
    0x6, 0x3d, 0xe6, 0x7, 0x83, 0xc1, 0x80,

    /* U+003D "=" */
    0xfe, 0x0, 0x7, 0xf0,

    /* U+003E ">" */
    0x81, 0xe0, 0xf0, 0x73, 0xde, 0x20, 0x0,

    /* U+003F "?" */
    0xf4, 0xc6, 0x33, 0xb9, 0x8c, 0x0, 0x18,

    /* U+0040 "@" */
    0x1e, 0xc, 0x66, 0xb, 0xff, 0xf2, 0xfc, 0xbf,
    0x2b, 0x7c, 0x60, 0x1c, 0x1, 0xf0,

    /* U+0041 "A" */
    0x3, 0x0, 0x70, 0xf, 0x80, 0xd8, 0xd, 0x81,
    0xdc, 0x1f, 0xc3, 0xc, 0x30, 0x6f, 0x6, 0x60,
    0x70,

    /* U+0042 "B" */
    0xfc, 0xc6, 0xc6, 0xc6, 0xfc, 0xc6, 0xc3, 0xc3,
    0xc3, 0xc7, 0xfe,

    /* U+0043 "C" */
    0x1f, 0x1c, 0x66, 0x1b, 0x0, 0xc0, 0x30, 0xc,
    0x3, 0x0, 0x60, 0xcc, 0x71, 0xf8,

    /* U+0044 "D" */
    0xfc, 0x63, 0xb0, 0xd8, 0x3c, 0x1e, 0xf, 0x7,
    0x83, 0xc3, 0x63, 0x3f, 0x0,

    /* U+0045 "E" */
    0xfd, 0x83, 0x6, 0xc, 0x1f, 0xb0, 0x60, 0xc1,
    0x83, 0xf0,

    /* U+0046 "F" */
    0xfd, 0x83, 0x6, 0xc, 0x1f, 0xb0, 0x60, 0xc1,
    0x83, 0x0,

    /* U+0047 "G" */
    0x1f, 0x6, 0x39, 0x87, 0x60, 0xc, 0x1, 0x8f,
    0xb0, 0x36, 0x6, 0x60, 0xc6, 0x30, 0x7c, 0x0,

    /* U+0048 "H" */
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3,
    0xc3, 0xc3, 0xc3,

    /* U+0049 "I" */
    0xff, 0xff, 0xfc,

    /* U+004A "J" */
    0x18, 0xc6, 0x31, 0x8c, 0x63, 0x18, 0xc6, 0x31,
    0xf8,

    /* U+004B "K" */
    0xc2, 0xc6, 0xce, 0xd8, 0xf0, 0xe0, 0xf0, 0xd8,
    0xcc, 0xc7, 0xc2,

    /* U+004C "L" */
    0xc1, 0x83, 0x6, 0xc, 0x18, 0x30, 0x60, 0xc1,
    0x83, 0xf0,

    /* U+004D "M" */
    0xc0, 0x3c, 0x7, 0xe0, 0x7e, 0xf, 0xf0, 0xfd,
    0x9b, 0xd9, 0xbc, 0xf3, 0xcf, 0x3c, 0x63, 0xc0,
    0x30,

    /* U+004E "N" */
    0xc1, 0xe0, 0xf8, 0x7e, 0x3f, 0x9e, 0xcf, 0x37,
    0x8f, 0xc7, 0xe1, 0xf0, 0x60,

    /* U+004F "O" */
    0x1e, 0x1c, 0xe6, 0x1b, 0x3, 0xc0, 0xf0, 0x3c,
    0xf, 0x3, 0x61, 0x9c, 0xe1, 0xe0,

    /* U+0050 "P" */
    0xfc, 0xc6, 0xc3, 0xc3, 0xc3, 0xc6, 0xfc, 0xc0,
    0xc0, 0xc0, 0xc0,

    /* U+0051 "Q" */
    0x1e, 0x1c, 0xc6, 0x1b, 0x3, 0xc0, 0xf0, 0x3c,
    0xf, 0x13, 0x65, 0xdd, 0xe1, 0xf8, 0x6,

    /* U+0052 "R" */
    0xfc, 0x63, 0x30, 0xd8, 0x6e, 0x77, 0xf3, 0x71,
    0x98, 0xce, 0x63, 0xf0, 0xc0,

    /* U+0053 "S" */
    0x3c, 0x46, 0xc3, 0xe0, 0xf8, 0x7e, 0x3f, 0x7,
    0x83, 0xc7, 0x7e,

    /* U+0054 "T" */
    0xff, 0x86, 0x3, 0x1, 0x80, 0xc0, 0x60, 0x30,
    0x18, 0xc, 0x6, 0x3, 0x0,

    /* U+0055 "U" */
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc3, 0xc7, 0x7f,

    /* U+0056 "V" */
    0xe0, 0xd8, 0x76, 0x1d, 0xc6, 0x31, 0x8c, 0xe3,
    0xb0, 0x6c, 0x1f, 0x3, 0x80, 0xe0,

    /* U+0057 "W" */
    0xe7, 0x6, 0xc6, 0x1d, 0x8c, 0x33, 0x9c, 0x63,
    0x18, 0xc6, 0x73, 0xe, 0xf6, 0xd, 0xec, 0x1e,
    0xf0, 0x3c, 0xe0, 0x39, 0xc0,

    /* U+0058 "X" */
    0xe1, 0x98, 0xe3, 0x30, 0xf8, 0x1e, 0x3, 0x1,
    0xe0, 0xf8, 0x33, 0x18, 0x7e, 0x18,

    /* U+0059 "Y" */
    0xe1, 0xd8, 0x63, 0x30, 0xfc, 0x1e, 0x7, 0x0,
    0xc0, 0x30, 0xc, 0x3, 0x0, 0xc0,

    /* U+005A "Z" */
    0xff, 0x80, 0xe0, 0x70, 0x38, 0xe, 0x7, 0x3,
    0x81, 0xe0, 0xf0, 0x38, 0xf, 0xfc,

    /* U+005B "[" */
    0xfc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcf,

    /* U+005C "\\" */
    0xc0, 0x60, 0x60, 0x20, 0x30, 0x30, 0x18, 0x18,
    0xc, 0xc, 0x4, 0x6, 0x6, 0x3,

    /* U+005D "]" */
    0xf3, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3f,

    /* U+005E "^" */
    0x30, 0x71, 0xa3, 0x6c, 0x58, 0xc0,

    /* U+005F "_" */
    0xff,

    /* U+0060 "`" */
    0x8, 0x80,

    /* U+0061 "a" */
    0x7e, 0x37, 0x31, 0x98, 0xcc, 0x66, 0x33, 0xb8,
    0xfe,

    /* U+0062 "b" */
    0xc1, 0x83, 0x6, 0xf, 0xdd, 0xb1, 0xe3, 0xc7,
    0x8f, 0xbf, 0xe0,

    /* U+0063 "c" */
    0x3c, 0xcf, 0x6, 0xc, 0x18, 0x59, 0x9e,

    /* U+0064 "d" */
    0x6, 0x3, 0x1, 0x80, 0xc7, 0xe3, 0x73, 0x19,
    0x8c, 0xc6, 0x63, 0x3b, 0x8f, 0xe0,

    /* U+0065 "e" */
    0x3c, 0xcf, 0x1e, 0x7f, 0x98, 0x59, 0x9e,

    /* U+0066 "f" */
    0x1c, 0xc3, 0xc, 0xfc, 0xc3, 0xc, 0x30, 0xc3,
    0xc,

    /* U+0067 "g" */
    0x7e, 0xdf, 0x1e, 0x3c, 0x78, 0xfb, 0xbf, 0x6,
    0xf, 0x33, 0xc0,

    /* U+0068 "h" */
    0xc1, 0x83, 0x6, 0xf, 0x99, 0xb3, 0x66, 0xcd,
    0x9b, 0x3e, 0x70,

    /* U+0069 "i" */
    0xc0, 0x6d, 0xb6, 0xdb, 0x80,

    /* U+006A "j" */
    0x30, 0x3, 0x33, 0x33, 0x33, 0x33, 0x33, 0xe0,

    /* U+006B "k" */
    0xc1, 0x83, 0x6, 0xc, 0x5b, 0xbe, 0x70, 0xf1,
    0xb3, 0x3e, 0x30,

    /* U+006C "l" */
    0xdb, 0x6d, 0xb6, 0xdb, 0x70,

    /* U+006D "m" */
    0xff, 0x99, 0x9b, 0x33, 0x66, 0x6c, 0xcd, 0x99,
    0xb3, 0x3e, 0x67,

    /* U+006E "n" */
    0xf9, 0x9b, 0x36, 0x6c, 0xd9, 0xb3, 0xe7,

    /* U+006F "o" */
    0x38, 0xfb, 0xbe, 0x3c, 0x78, 0xdb, 0x1c,

    /* U+0070 "p" */
    0xfd, 0xdb, 0x1e, 0x3c, 0x78, 0xfb, 0xfe, 0xc1,
    0x83, 0x6, 0x0,

    /* U+0071 "q" */
    0x7e, 0x6e, 0xc6, 0xc6, 0xc6, 0xc6, 0xee, 0x7e,
    0x6, 0x6, 0x6, 0x7,

    /* U+0072 "r" */
    0xfe, 0x31, 0x8c, 0x63, 0x18,

    /* U+0073 "s" */
    0x7b, 0x3c, 0x3e, 0x7c, 0x78, 0xfe,

    /* U+0074 "t" */
    0x30, 0xc3, 0x3f, 0x30, 0xc3, 0xc, 0x30, 0xc3,
    0x80,

    /* U+0075 "u" */
    0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x7e,

    /* U+0076 "v" */
    0xc7, 0x8f, 0x1a, 0x36, 0xed, 0x9f, 0x1c,

    /* U+0077 "w" */
    0xc6, 0x3c, 0x63, 0x46, 0x36, 0x63, 0x67, 0x76,
    0x76, 0x3f, 0xe3, 0x9c,

    /* U+0078 "x" */
    0xe3, 0x33, 0xf, 0x83, 0x81, 0xc1, 0xf1, 0x9f,
    0xc6,

    /* U+0079 "y" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x78, 0xfb, 0xbf, 0x7,
    0xf, 0x33, 0xc0,

    /* U+007A "z" */
    0xfe, 0x1c, 0x71, 0xc7, 0x8e, 0x38, 0x7f,

    /* U+007B "{" */
    0x1c, 0xc3, 0xc, 0x30, 0xc7, 0x38, 0xe1, 0xc3,
    0xc, 0x30, 0xc3, 0x7,

    /* U+007C "|" */
    0xff, 0xff, 0xff, 0xff,

    /* U+007D "}" */
    0xe0, 0xc3, 0xc, 0x30, 0xc3, 0x6, 0x18, 0xc3,
    0xc, 0x30, 0xc3, 0x38,

    /* U+007E "~" */
    0x70, 0x2c, 0xf3, 0x40, 0xe0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 72, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 88, .box_w = 2, .box_h = 11, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 4, .adv_w = 82, .box_w = 5, .box_h = 3, .ofs_x = 0, .ofs_y = 8},
    {.bitmap_index = 6, .adv_w = 184, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 20, .adv_w = 148, .box_w = 8, .box_h = 13, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 33, .adv_w = 206, .box_w = 12, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 48, .adv_w = 164, .box_w = 9, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 60, .adv_w = 49, .box_w = 3, .box_h = 4, .ofs_x = 0, .ofs_y = 7},
    {.bitmap_index = 62, .adv_w = 86, .box_w = 4, .box_h = 16, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 70, .adv_w = 86, .box_w = 4, .box_h = 16, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 78, .adv_w = 112, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 82, .adv_w = 139, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 89, .adv_w = 84, .box_w = 2, .box_h = 4, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 90, .adv_w = 118, .box_w = 6, .box_h = 1, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 91, .adv_w = 84, .box_w = 2, .box_h = 1, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 92, .adv_w = 97, .box_w = 6, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 103, .adv_w = 158, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 114, .adv_w = 108, .box_w = 3, .box_h = 11, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 119, .adv_w = 158, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 130, .adv_w = 158, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 141, .adv_w = 158, .box_w = 9, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 154, .adv_w = 158, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 165, .adv_w = 158, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 176, .adv_w = 135, .box_w = 8, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 187, .adv_w = 158, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 198, .adv_w = 158, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 209, .adv_w = 84, .box_w = 2, .box_h = 7, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 211, .adv_w = 84, .box_w = 2, .box_h = 8, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 213, .adv_w = 135, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 220, .adv_w = 139, .box_w = 7, .box_h = 4, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 224, .adv_w = 135, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 231, .adv_w = 126, .box_w = 5, .box_h = 11, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 238, .adv_w = 187, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 252, .adv_w = 171, .box_w = 12, .box_h = 11, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 269, .adv_w = 160, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 280, .adv_w = 182, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 294, .adv_w = 178, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 307, .adv_w = 142, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 317, .adv_w = 132, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 327, .adv_w = 186, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 343, .adv_w = 172, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 354, .adv_w = 71, .box_w = 2, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 357, .adv_w = 95, .box_w = 5, .box_h = 14, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 366, .adv_w = 158, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 377, .adv_w = 134, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 387, .adv_w = 229, .box_w = 12, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 404, .adv_w = 182, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 417, .adv_w = 191, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 431, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 442, .adv_w = 191, .box_w = 10, .box_h = 12, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 457, .adv_w = 159, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 470, .adv_w = 146, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 481, .adv_w = 153, .box_w = 9, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 494, .adv_w = 172, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 505, .adv_w = 166, .box_w = 10, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 519, .adv_w = 242, .box_w = 15, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 540, .adv_w = 163, .box_w = 10, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 554, .adv_w = 159, .box_w = 10, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 568, .adv_w = 174, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 582, .adv_w = 75, .box_w = 4, .box_h = 16, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 590, .adv_w = 130, .box_w = 8, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 604, .adv_w = 75, .box_w = 4, .box_h = 16, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 612, .adv_w = 139, .box_w = 7, .box_h = 6, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 618, .adv_w = 134, .box_w = 8, .box_h = 1, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 619, .adv_w = 85, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = 10},
    {.bitmap_index = 621, .adv_w = 144, .box_w = 9, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 630, .adv_w = 141, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 641, .adv_w = 130, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 648, .adv_w = 145, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 662, .adv_w = 126, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 669, .adv_w = 103, .box_w = 6, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 678, .adv_w = 140, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 689, .adv_w = 131, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 700, .adv_w = 68, .box_w = 3, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 705, .adv_w = 82, .box_w = 4, .box_h = 15, .ofs_x = 0, .ofs_y = -4},
    {.bitmap_index = 713, .adv_w = 132, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 724, .adv_w = 67, .box_w = 3, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 729, .adv_w = 185, .box_w = 11, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 740, .adv_w = 131, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 747, .adv_w = 135, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 754, .adv_w = 141, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 765, .adv_w = 145, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 777, .adv_w = 97, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 782, .adv_w = 111, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 788, .adv_w = 107, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 797, .adv_w = 134, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 805, .adv_w = 126, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 812, .adv_w = 201, .box_w = 12, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 824, .adv_w = 141, .box_w = 9, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 833, .adv_w = 144, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 844, .adv_w = 135, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 851, .adv_w = 102, .box_w = 6, .box_h = 16, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 863, .adv_w = 57, .box_w = 2, .box_h = 16, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 867, .adv_w = 102, .box_w = 6, .box_h = 16, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 879, .adv_w = 158, .box_w = 9, .box_h = 4, .ofs_x = 0, .ofs_y = 4}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Pair left and right glyphs for kerning*/
static const uint8_t kern_pair_glyph_ids[] =
{
    9, 43,
    9, 75,
    19, 21,
    19, 23,
    19, 24,
    24, 13,
    24, 15,
    26, 13,
    26, 15,
    34, 34,
    34, 53,
    34, 55,
    34, 56,
    34, 58,
    37, 55,
    37, 56,
    37, 57,
    37, 58,
    39, 15,
    39, 34,
    39, 66,
    39, 68,
    39, 69,
    39, 70,
    39, 72,
    39, 75,
    39, 80,
    39, 82,
    40, 53,
    40, 55,
    40, 56,
    40, 57,
    40, 58,
    44, 34,
    44, 36,
    44, 40,
    44, 48,
    44, 50,
    44, 66,
    44, 68,
    44, 69,
    44, 70,
    44, 72,
    44, 80,
    44, 82,
    45, 34,
    45, 36,
    45, 40,
    45, 48,
    45, 50,
    45, 53,
    45, 55,
    45, 56,
    45, 58,
    48, 53,
    48, 55,
    48, 56,
    48, 57,
    48, 58,
    49, 15,
    49, 34,
    49, 58,
    51, 34,
    51, 53,
    51, 56,
    51, 58,
    51, 66,
    51, 68,
    51, 69,
    51, 70,
    51, 72,
    51, 80,
    51, 82,
    52, 34,
    53, 15,
    53, 34,
    53, 36,
    53, 40,
    53, 48,
    53, 50,
    53, 66,
    53, 68,
    53, 69,
    53, 70,
    53, 72,
    53, 75,
    53, 78,
    53, 79,
    53, 80,
    53, 81,
    53, 82,
    53, 83,
    53, 84,
    53, 85,
    53, 86,
    53, 87,
    53, 88,
    53, 89,
    53, 90,
    53, 91,
    55, 15,
    55, 34,
    55, 36,
    55, 40,
    55, 48,
    55, 50,
    55, 66,
    55, 68,
    55, 69,
    55, 70,
    55, 72,
    55, 78,
    55, 79,
    55, 80,
    55, 81,
    55, 82,
    55, 83,
    55, 84,
    55, 86,
    55, 90,
    56, 15,
    56, 34,
    56, 36,
    56, 40,
    56, 48,
    56, 50,
    56, 66,
    56, 68,
    56, 69,
    56, 70,
    56, 72,
    56, 78,
    56, 79,
    56, 80,
    56, 81,
    56, 82,
    56, 83,
    56, 84,
    56, 86,
    56, 90,
    57, 34,
    57, 36,
    57, 40,
    57, 48,
    57, 50,
    58, 15,
    58, 34,
    58, 36,
    58, 40,
    58, 48,
    58, 50,
    58, 66,
    58, 68,
    58, 69,
    58, 70,
    58, 72,
    58, 78,
    58, 79,
    58, 80,
    58, 81,
    58, 82,
    58, 83,
    58, 84,
    58, 85,
    58, 86,
    58, 90,
    60, 43,
    60, 75,
    66, 34,
    67, 34,
    67, 53,
    67, 55,
    67, 56,
    67, 58,
    68, 34,
    68, 53,
    68, 55,
    68, 56,
    68, 58,
    69, 34,
    70, 34,
    70, 53,
    70, 55,
    70, 56,
    70, 58,
    73, 34,
    74, 34,
    76, 34,
    77, 34,
    78, 34,
    79, 34,
    80, 34,
    80, 53,
    80, 55,
    80, 56,
    80, 58,
    81, 53,
    81, 55,
    81, 56,
    81, 58,
    84, 34,
    84, 53,
    84, 55,
    84, 56,
    84, 58,
    85, 53,
    86, 34,
    87, 53,
    88, 53,
    89, 34,
    89, 53,
    90, 53,
    91, 53,
    92, 43,
    92, 75
};

/* Kerning between the respective left and right glyphs
 * 4.4 format which needs to scaled with `kern_scale`*/
static const int8_t kern_pair_values[] =
{
    10, 10, -5, -5, -5, -5, -5, -5,
    -5, 5, -15, -18, -18, -18, -5, -5,
    -5, -5, -10, -15, -5, -5, -5, -5,
    -5, -5, -5, -5, -5, -5, -5, -5,
    -5, 8, -5, -5, -5, -5, -8, -8,
    -8, -8, -8, -8, -8, 8, -8, -8,
    -8, -8, -18, -18, -18, -18, -5, -5,
    -5, -5, -5, -10, -15, -8, 8, -8,
    -8, -8, -3, -3, -3, -3, -3, -3,
    -3, 3, -13, -20, -5, -5, -5, -5,
    -18, -18, -18, -18, -18, -5, -13, -13,
    -18, -13, -18, -13, -13, -10, -13, -13,
    -13, -10, -13, -13, -13, -15, -5, -5,
    -5, -5, -13, -13, -13, -13, -13, -8,
    -8, -13, -8, -13, -8, -8, -8, -8,
    -13, -15, -5, -5, -5, -5, -10, -10,
    -10, -10, -10, -8, -8, -10, -8, -10,
    -8, -8, -8, -3, 8, -5, -5, -5,
    -5, -13, -20, -5, -5, -5, -5, -23,
    -23, -23, -23, -23, -13, -13, -23, -13,
    -23, -13, -15, -8, -13, -13, 10, 10,
    8, 5, -18, -13, -10, -23, 5, -18,
    -13, -10, -23, 8, 5, -18, -13, -10,
    -23, 8, 8, 9, 8, 8, 8, 5,
    -18, -13, -10, -23, -18, -13, -10, -23,
    3, -13, -8, -8, -15, -10, 8, -13,
    -13, 8, -13, -13, -13, 10, 10
};

/*Collect the kern pair's data in one place*/
static const lv_font_fmt_txt_kern_pair_t kern_pairs =
{
    .glyph_ids = kern_pair_glyph_ids,
    .values = kern_pair_values,
    .pair_cnt = 215,
    .glyph_ids_size = 0
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_pairs,
    .kern_scale = 16,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t ui_font_shu16 = {
#else
lv_font_t ui_font_shu16 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 18,          /*The maximum line height required by the font*/
    .base_line = 4,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -2,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if UI_FONT_SHU16*/

