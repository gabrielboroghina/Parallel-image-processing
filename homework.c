#include "homework.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int num_threads;
int resize_factor;

const unsigned char gaussianKernel[3][3] = {
        {1, 2, 1},
        {2, 4, 2},
        {1, 2, 1}
};

void readInput(const char *fileName, image *img) {
    FILE *imageFile = fopen(fileName, "r");

    // read image's type
    char imageType;
    fscanf(imageFile, "%*c%c", &imageType);
    img->type = imageType;

    // read image's dimensions
    fscanf(imageFile, "%i%i%hhu%*c", &img->width, &img->height, &img->maxval);

    int pixelSize = img->type == COLOR ? 3 : 1;
    img->pixel = (void **) malloc(img->height * sizeof(void *));

    // read the pixel matrix
    for (int i = 0; i < img->height; i++) {
        img->pixel[i] = malloc(img->width * pixelSize);
        fread(img->pixel[i], pixelSize, img->width, imageFile);
    }

    fclose(imageFile);
}

void writeData(const char *fileName, image *img) {
    FILE *imageFile = fopen(fileName, "w");

    // print image's type
    fprintf(imageFile, "P%c\n", img->type);

    // print image's dimensions
    fprintf(imageFile, "%i %i\n%hhu\n", img->width, img->height, img->maxval);

    // print the pixel matrix
    int pixelSize = img->type == COLOR ? 3 : 1;
    for (int i = 0; i < img->height; i++)
        fwrite(img->pixel[i], pixelSize, img->width, imageFile);

    fclose(imageFile);
}

void blockCompress(void **in, image_type imgType, void **out, int xBlock, int yBlock) {
    int i, j, c;
    int colorPixel[3] = {0}; // int to avoid overflows
    int grayscalePixel = 0;

    int x = xBlock * resize_factor;
    int y = yBlock * resize_factor;
    if (resize_factor == 3) {
        // apply Gaussian Filter
        for (i = x; i < x + 3; i++)
            for (j = y; j < y + 3; j++) {
                if (imgType == COLOR) {
                    for (c = 0; c < 3; c++) {
                        colorPixel[c] += ((color_pixel **) in)[i][j].channel[c] * gaussianKernel[i - x][j - y];
                    }
                } else {
                    grayscalePixel += ((grayscale_pixel **) in)[i][j].l * gaussianKernel[i - x][j - y];
                }
            }

        if (imgType == COLOR) {
            for (c = 0; c < 3; c++)
                colorPixel[c] /= 16;
        } else {
            grayscalePixel /= 16;
        }
    } else {
        // compute pixels' arithmetic mean
        for (i = x; i < x + resize_factor; i++)
            for (j = y; j < y + resize_factor; j++) {
                if (imgType == COLOR) {
                    for (c = 0; c < 3; c++)
                        colorPixel[c] += ((color_pixel **) in)[i][j].channel[c];
                } else {
                    grayscalePixel += ((grayscale_pixel **) in)[i][j].l;
                }
            }

        int resize_factor2 = resize_factor * resize_factor;
        if (imgType == COLOR) {
            for (c = 0; c < 3; c++)
                colorPixel[c] /= resize_factor2;
        } else {
            grayscalePixel /= resize_factor2;
        }
    }

    if (imgType == COLOR) {
        for (c = 0; c < 3; c++)
            ((color_pixel **) out)[xBlock][yBlock].channel[c] = colorPixel[c];
    } else {
        ((grayscale_pixel **) out)[xBlock][yBlock].l = grayscalePixel;
    }
}

void *chunkResize(void *args) {
    thread_data threadArgs = *(thread_data *) args;
    int begin = threadArgs.jobBounds.begin;
    int end = threadArgs.jobBounds.end;

    void **in = threadArgs.in->pixel;
    void **out = threadArgs.out->pixel;

    int x = begin / threadArgs.numBlocksOnLine;
    int y = begin % threadArgs.numBlocksOnLine;

    for (int i = begin; i < end; i++) {
        blockCompress(in, threadArgs.in->type, out, x, y);
        y++;
        if (y == threadArgs.numBlocksOnLine) {
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

void resize(image *in, image *out) {
    int i;
    pthread_t thread[num_threads];
    thread_data threadArgs[num_threads];

    // compute the number of blocks to be compressed (each block in 1 pixel)
    int numBlocksOnLine = in->width / resize_factor;
    int numBlocks = (in->height / resize_factor) * numBlocksOnLine;

    out->type = in->type;
    out->height = in->height / resize_factor;
    out->width = numBlocksOnLine;
    out->maxval = in->maxval;
    out->pixel = (void **) malloc(out->height * sizeof(void *));
    for (i = 0; i < out->height; i++)
        out->pixel[i] = malloc(out->width * (in->type == COLOR ? 3 : 1));

    for (i = 0; i < num_threads; i++) {
        threadArgs[i].threadID = i;
        threadArgs[i].in = in;
        threadArgs[i].out = out;
        threadArgs[i].jobBounds = jobBoundsForThread(i, numBlocks);
        threadArgs[i].numBlocksOnLine = numBlocksOnLine;
        pthread_create(&(thread[i]), NULL, chunkResize, &threadArgs[i]);
    }

    for (i = 0; i < num_threads; i++)
        pthread_join(thread[i], NULL);
}