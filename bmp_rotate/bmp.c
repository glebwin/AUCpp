#include "bmp.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

void bmp_read_file(Bmp *bmp, char *file_name) {
    FILE *file;
    file = fopen(file_name, "rb");

    fread(&bmp->bf_type, sizeof(bmp->bf_type), 1, file);
    fread(&bmp->bf_size, sizeof(bmp->bf_size), 1, file);
    fread(&bmp->bf_reserved1, sizeof(bmp->bf_reserved1), 1, file);
    fread(&bmp->bf_reserved2, sizeof(bmp->bf_reserved2), 1, file);
    fread(&bmp->bf_off_bits, sizeof(bmp->bf_off_bits), 1, file);
    fread(&bmp->bi_size, sizeof(bmp->bf_size), 1, file);
    fread(&bmp->bi_width, sizeof(bmp->bi_width), 1, file);
    fread(&bmp->bi_height, sizeof(bmp->bi_height), 1, file);
    fread(&bmp->bi_planes, sizeof(bmp->bi_planes), 1, file);
    fread(&bmp->bi_bit_count, sizeof(bmp->bi_bit_count), 1, file);
    fread(&bmp->bi_compression, sizeof(bmp->bi_compression), 1, file);
    fread(&bmp->bi_size_image, sizeof(bmp->bi_size_image), 1, file);
    fread(&bmp->bi_x_pels_per_meter, sizeof(bmp->bi_x_pels_per_meter), 1, file);
    fread(&bmp->bi_y_pels_per_meter, sizeof(bmp->bi_y_pels_per_meter), 1, file);
    fread(&bmp->bi_clr_used, sizeof(bmp->bi_clr_used), 1, file);
    fread(&bmp->bi_clr_important, sizeof(bmp->bi_clr_important), 1, file);

    alloc_picture(&bmp->data, bmp->bi_width, bmp->bi_height);

    uint8_t null_padding = (4 - bmp->bi_width * sizeof(Pixel) % 4) % 4;

    if(bmp->bi_height < 0)
        for(uint32_t i = 0; i < bmp->bi_height; i++) {
            for(uint32_t j = 0; j < bmp->bi_width; j++) {
                fread(&bmp->data[i][j].r, 1, 1, file);
                fread(&bmp->data[i][j].g, 1, 1, file);
                fread(&bmp->data[i][j].b, 1, 1, file);
            }
            fseek(file, null_padding, SEEK_CUR);
        }
    else if(bmp->bi_height > 0)
        for(uint32_t i = bmp->bi_height; i-- > 0;) {
            for(uint32_t j = 0; j < bmp->bi_width; j++) {
                fread(&bmp->data[i][j].r, 1, 1, file);
                fread(&bmp->data[i][j].g, 1, 1, file);
                fread(&bmp->data[i][j].b, 1, 1, file);
            }
            fseek(file, null_padding, SEEK_CUR);
        }

    fclose(file);
}

void bmp_write_file(Bmp *bmp, char *file_name) {
    FILE *file;
    file = fopen(file_name, "wb");

    fwrite(&bmp->bf_type, sizeof(bmp->bf_type), 1, file);
    fwrite(&bmp->bf_size, sizeof(bmp->bf_size), 1, file);
    fwrite(&bmp->bf_reserved1, sizeof(bmp->bf_reserved1), 1, file);
    fwrite(&bmp->bf_reserved2, sizeof(bmp->bf_reserved2), 1, file);
    fwrite(&bmp->bf_off_bits, sizeof(bmp->bf_off_bits), 1, file);
    fwrite(&bmp->bi_size, sizeof(bmp->bf_size), 1, file);
    fwrite(&bmp->bi_width, sizeof(bmp->bi_width), 1, file);
    fwrite(&bmp->bi_height, sizeof(bmp->bi_height), 1, file);
    fwrite(&bmp->bi_planes, sizeof(bmp->bi_planes), 1, file);
    fwrite(&bmp->bi_bit_count, sizeof(bmp->bi_bit_count), 1, file);
    fwrite(&bmp->bi_compression, sizeof(bmp->bi_compression), 1, file);
    fwrite(&bmp->bi_size_image, sizeof(bmp->bi_size_image), 1, file);
    fwrite(&bmp->bi_x_pels_per_meter, sizeof(bmp->bi_x_pels_per_meter), 1, file);
    fwrite(&bmp->bi_y_pels_per_meter, sizeof(bmp->bi_y_pels_per_meter), 1, file);
    fwrite(&bmp->bi_clr_used, sizeof(bmp->bi_clr_used), 1, file);
    fwrite(&bmp->bi_clr_important, sizeof(bmp->bi_clr_important), 1, file);

    uint8_t null_padding = (4 - bmp->bi_width * sizeof(Pixel) % 4) % 4;
    uint32_t null_buffer = 0;

    if(bmp->bi_height < 0)
        for(uint32_t i = 0; i < bmp->bi_height; i++) {
            for(uint32_t j = 0; j < bmp->bi_width; j++) {
                fwrite(&bmp->data[i][j].r, 1, 1, file);
                fwrite(&bmp->data[i][j].g, 1, 1, file);
                fwrite(&bmp->data[i][j].b, 1, 1, file);
            }
            fwrite(&null_buffer, 1, null_padding, file);
        }
    else if(bmp->bi_height > 0)
        for(uint32_t i = bmp->bi_height; i-- > 0;) {
            for(uint32_t j = 0; j < bmp->bi_width; j++) {
                fwrite(&bmp->data[i][j].r, 1, 1, file);
                fwrite(&bmp->data[i][j].g, 1, 1, file);
                fwrite(&bmp->data[i][j].b, 1, 1, file);
            }
            fwrite(&null_buffer, 1, null_padding, file);
        }

    fclose(file);
}

void bmp_finit(Bmp *bmp) {
    free(bmp->data[0]);
    free(bmp->data);
}

void bmp_crop(Bmp **dest, Bmp *src, int x0, int y0, int x1, int y1) {
    *dest = malloc(sizeof(Bmp));

    **dest = *src;

    (*dest)->bi_width = x1 - x0 + 1;
    (*dest)->bi_height = y1 - y0 + 1;
    update_bmp_sizes(*dest);

    alloc_picture(&(*dest)->data, (*dest)->bi_width, (*dest)->bi_height);

    for(uint32_t i = y0; i <= y1; i++)
        for(uint32_t j = x0; j <= x1; j++)
            (*dest)->data[i - y0][j - x0] = src->data[i][j];
}

void bmp_rotate(Bmp *bmp) {
    Pixel **result;
    alloc_picture(&result, bmp->bi_height, bmp->bi_width);
    for(uint32_t i = 0; i < bmp->bi_height; i++)
        for(uint32_t j = 0; j < bmp->bi_width; j++)
            result[j][bmp->bi_height - i - 1] = bmp->data[i][j];

    bmp_finit(bmp);
    bmp->data = result;

    uint32_t temp = bmp->bi_height;
    bmp->bi_height = bmp->bi_width;
    bmp->bi_width = temp;

    update_bmp_sizes(bmp);
}

void alloc_picture(Pixel ***ptr, uint32_t width, uint32_t height) {
    Pixel *memory = malloc(width * height * sizeof(Pixel));
    *ptr = malloc(height * sizeof(Pixel*));
    for(uint32_t i = 0; i < height; i++) {
        (*ptr)[i] = memory;
        memory += width;
    }
}

void update_bmp_sizes(Bmp *bmp) {
    uint8_t null_padding = (4 - bmp->bi_width * sizeof(Pixel) % 4) % 4;
    uint8_t file_header_sz = sizeof(bmp->bf_type) + sizeof(bmp->bf_size) + sizeof(bmp->bf_reserved1)
                             + sizeof(bmp->bf_reserved2) + sizeof(bmp->bf_off_bits);
    bmp->bi_size_image = bmp->bi_height * (bmp->bi_width * sizeof(Pixel) + null_padding);
    bmp->bf_size = file_header_sz + bmp->bi_size + bmp->bi_size_image;
}

