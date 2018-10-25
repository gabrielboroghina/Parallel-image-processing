#include "homework1.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

int num_threads;
int resolution;

void initialize(image *img) {
    img->height = img->width = resolution;
    img->pixel = (grayscale_pixel **) malloc(img->height * sizeof(grayscale_pixel *));

    // allocate memory for the pixel matrix
    for (int i = 0; i < img->height; i++)
        img->pixel[i] = (grayscale_pixel *) malloc(img->width);
}

void pixelRender(image *img, int i, int j) {
    // compute pixel's position in image
    int x = j;
    int y = img->height - i - 1;
    // compute pixel's center position
    double xCenter = (double) 50 / img->width * (2 * x + 1);
    double yCenter = (double) 50 / img->height * (2 * y + 1);

    // compute distance from the pixel's center to the segment
    double dist = abs(xCenter - 2 * yCenter) / sqrt(5);
    // fill the pixel
    img->pixel[i][j] = (dist <= 3) ? PIXEL_BLACK : PIXEL_WHITE;
}

void *chunkRender(void *args) {
    thread_data threadArgs = *(thread_data *) args;
    int begin = threadArgs.jobBounds.begin;
    int end = threadArgs.jobBounds.end;

    image *img = threadArgs.img;
    int x = begin / img->height;
    int y = begin % img->width;

    for (int i = begin; i < end; i++) {
        pixelRender(img, x, y);
        y++;
        if (y == img->width) {
            x++;
            y = 0;
        }
    }

    return NULL;
}

bounds jobBoundsForThread(int threadID, int numBlocks) {
    bounds jobBounds;
    int chunkSize = numBlocks / num_threads;
    int r = numBlocks % num_threads;

    jobBounds.begin = threadID * chunkSize + (threadID < r ? threadID : r);
    jobBounds.end = jobBounds.begin + chunkSize + (threadID < r ? 1 : 0);

    return jobBounds;
}

void render(image *img) {
    int i;
    pthread_t thread[num_threads];
    thread_data threadArgs[num_threads];

    int numPixels = img->height * img->width;
    for (i = 0; i < num_threads; i++) {
        threadArgs[i].img = img;
        threadArgs[i].jobBounds = jobBoundsForThread(i, numPixels);
        pthread_create(&(thread[i]), NULL, chunkRender, &threadArgs[i]);
    }

    for (i = 0; i < num_threads; i++)
        pthread_join(thread[i], NULL);
}

void writeData(const char *fileName, image *img) {
    FILE *imageFile = fopen(fileName, "w");

    // print image's type (grayscale)
    fprintf(imageFile, "P5\n");

    // print image's dimensions
    fprintf(imageFile, "%i %i\n%i\n", img->width, img->height, MAXVAL);

    // print the pixel matrix
    for (int i = 0; i < img->height; i++)
        fwrite(img->pixel[i], 1, img->width, imageFile);

    fclose(imageFile);
}
