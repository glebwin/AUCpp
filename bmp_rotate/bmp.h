#ifndef BMP_H
#define BMP_H

#include <stdint.h>

typedef struct Pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Pixel;

typedef struct Bmp {
    uint16_t bf_type;
    uint32_t bf_size;
    uint16_t bf_reserved1;
    uint16_t bf_reserved2;
    uint32_t bf_off_bits;
    uint32_t bi_size;
    uint32_t bi_width;
    int32_t bi_height;
    uint16_t bi_planes;
    uint16_t bi_bit_count;
    uint32_t bi_compression;
    uint32_t bi_size_image;
    uint32_t bi_x_pels_per_meter;
    uint32_t bi_y_pels_per_meter;
    uint32_t bi_clr_used;
    uint32_t bi_clr_important;

    struct Pixel **data;
} Bmp;

void bmp_read_file(Bmp *bmp, char *file_name);
void bmp_write_file(Bmp *bmp, char *file_name);
void bmp_finit(Bmp *bmp);

void bmp_crop(Bmp **dest,  Bmp *src, int x0, int y0, int x1, int y1);
void bmp_rotate(Bmp *bmp);

void alloc_picture(Pixel ***ptr, uint32_t width, uint32_t height);
void update_bmp_sizes(Bmp *bmp);

#endif

