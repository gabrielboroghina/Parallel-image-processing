// Gabriel Boroghina, 333CB

#ifndef HOMEWORK_H
#define HOMEWORK_H

#define IMG_TYPE_COLOR '6'
#define IMG_TYPE_GRAYSCALE '5'

typedef struct {
    unsigned char channel[3]; // channel[0] = red, channel[1] = green, channel[2] = blue
}__attribute__((packed)) color_pixel;

typedef struct {
    unsigned char l; // light of the grayscale pixel
}__attribute__((packed)) grayscale_pixel;

typedef enum {
    COLOR = IMG_TYPE_COLOR,
    GRAYSCALE = IMG_TYPE_GRAYSCALE
} image_type;

typedef struct {
    image_type type;
    int height, width;
    unsigned char maxval;
    void **pixel;
} image;

typedef struct {
    int begin, end;
} bounds;

/**
 * Arguments to be sent to the thread
 */
typedef struct {
    int numBlocksOnLine;
    bounds jobBounds;
    image *in, *out;
} thread_data;

void readInput(const char *fileName, image *img);

void writeData(const char *fileName, image *img);

void resize(image *in, image *out);

#endif /* HOMEWORK_H */