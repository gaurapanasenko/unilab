#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"

#define HEDAER_OFFSET 14

typedef struct InfoHeader {
    uint32_t size, width, height;
    uint16_t planes, bits_per_pixel;
    uint32_t compression, image_size, x_pixels_per_m, y_pixels_per_m,
    colors_used, important_colors;
} __attribute__((packed)) InfoHeader;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s filename.bmp output.bmp\n", argv[0]);
        return -1;
    }
    FILE *f = fopen(argv[1], "rb");
    FILE *of = fopen(argv[2], "wb");
    if (f == NULL || of == NULL) {
        fprintf(stderr, "Failed to open files.\n");
        return -2;
    }
    fseek(f, 0L, SEEK_END);
    size_t size = (size_t)ftell(f);
    fseek(f, 0L, SEEK_SET);
    char *buffer = (char*)calloc(1, size + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Failed to alloc buffer.\n");
        return -2;
    }
    InfoHeader *header = (InfoHeader*)(buffer + HEDAER_OFFSET);
    fread(buffer, size, 1, f);
    printf("Header size:    %u\n", header->size);
    printf("Width:          %u\n", header->width);
    printf("Height:         %u\n", header->height);
    printf("Planes:         %u\n", header->planes);
    printf("Bits per pixel: %u\n", header->bits_per_pixel);
    printf("Compression:    %u\n", header->compression);
    printf("Image size:     %u\n", header->image_size);
    printf("X pixels/meter: %u\n", header->x_pixels_per_m);
    printf("Y pixels/meter: %u\n", header->y_pixels_per_m);
    printf("Colors used:    %u\n", header->colors_used);
    printf("Important cols: %u\n", header->important_colors);
    if (header->compression != 0) {
        fprintf(stderr, "Bmp file is compressed.\n");
        return -2;
    }
    if (header->bits_per_pixel != 24) {
        fprintf(stderr, "Bits per pixel must be 24.\n");
        return -2;
    }
    uint8_t (*colors)[3] = (void*)(buffer + HEDAER_OFFSET + header->size);
    size_t sz = header->width * header->height;
    uint8_t avg;
    for (size_t i = 0; i < sz; i++) {
        avg = (colors[i][0] + colors[i][1] + colors[i][2]) / 3;
        colors[i][0] = 255;
        colors[i][1] = avg;
        colors[i][2] = avg;
    }
    fwrite(buffer, size, 1, of);
    fclose(f);
    fclose(of);
    free(buffer);
    return 0;
}
