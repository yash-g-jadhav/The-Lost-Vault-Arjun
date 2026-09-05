/*
    stb_image.h - v2.28 - public domain image loader
    Minimal stub implementation for phase 0; full header can be expanded or replaced as needed.
*/
#ifndef STBI_INCLUDE_STB_IMAGE_H
#define STBI_INCLUDE_STB_IMAGE_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char stbi_uc;

unsigned char *stbi_load(char const *filename, int *x, int *y, int *channels_in_file, int desired_channels);
unsigned char *stbi_load_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *channels_in_file, int desired_channels);
void stbi_image_free(void *retval_from_stbi_load);
void stbi_set_flip_vertically_on_load(int flag_true_if_should_flip);

#ifdef __cplusplus
}
#endif

#ifdef STB_IMAGE_IMPLEMENTATION

unsigned char *stbi_load(char const *filename, int *x, int *y, int *channels_in_file, int desired_channels) {
    (void)filename; (void)x; (void)y; (void)channels_in_file; (void)desired_channels;
    return NULL;
}

unsigned char *stbi_load_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *channels_in_file, int desired_channels) {
    (void)buffer; (void)len; (void)x; (void)y; (void)channels_in_file; (void)desired_channels;
    return NULL;
}

void stbi_image_free(void *retval_from_stbi_load) {
    (void)retval_from_stbi_load;
}

void stbi_set_flip_vertically_on_load(int flag_true_if_should_flip) {
    (void)flag_true_if_should_flip;
}

#endif /* STB_IMAGE_IMPLEMENTATION */

#endif /* STBI_INCLUDE_STB_IMAGE_H */
