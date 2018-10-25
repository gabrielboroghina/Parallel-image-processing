#ifndef HOMEWORK_H1
#define HOMEWORK_H1

#define MAXVAL 255
#define PIXEL_BLACK 0
#define PIXEL_WHITE 255

typedef unsigned char grayscale_pixel;

typedef struct {
    int height, width;
    grayscale_pixel **pixel;
} image;

typedef struct {
    int begin, end;
} bounds;

typedef struct {
    bounds jobBounds;
    image *img;
} thread_data;

void initialize(image *im);

void render(image *im);

void writeData(const char *fileName, image *img);

#endif /* HOMEWORK_H1 */