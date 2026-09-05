/*
    stb_truetype.h - v1.26 - public domain font processing
*/
#ifndef STB_TRUETYPE_INCLUDE_STB_TRUETYPE_H
#define STB_TRUETYPE_INCLUDE_STB_TRUETYPE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
   unsigned short x0, y0, x1, y1;
   float xoff, yoff, xadvance;
} stbtt_bakedchar;

int stbtt_BakeFontBitmap(const unsigned char *data, int offset,
                         float pixel_height,
                         unsigned char *pixels, int pw, int ph,
                         int first_char, int num_chars,
                         stbtt_bakedchar *chardata);

#ifdef __cplusplus
}
#endif

#ifdef STB_TRUETYPE_IMPLEMENTATION

int stbtt_BakeFontBitmap(const unsigned char *data, int offset,
                         float pixel_height,
                         unsigned char *pixels, int pw, int ph,
                         int first_char, int num_chars,
                         stbtt_bakedchar *chardata) {
    (void)data; (void)offset; (void)pixel_height; (void)pixels; (void)pw; (void)ph;
    (void)first_char; (void)num_chars; (void)chardata;
    return 0;
}

#endif /* STB_TRUETYPE_IMPLEMENTATION */

#endif /* STB_TRUETYPE_INCLUDE_STB_TRUETYPE_H */
