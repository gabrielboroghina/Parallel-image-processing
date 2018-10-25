#include "homework.h"
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

//DO NOT MODIFY THIS FILE
extern int num_threads;
extern int resize_factor;
int main(int argc, char * argv[]) {
	//agrv[1] input
	//argv[2] output
	//argv[3] resize_factor
	//argv[4] num_threads
	if(argc < 5) {
		printf("Incorrect number of arguments\n");
		exit(-1);
	}
//DO NOT MODIFY THIS FILE
	resize_factor = atoi(argv[3]);
	num_threads = atoi(argv[4]);
//DO NOT MODIFY THIS FILE
	image input;
	image output;
//DO NOT MODIFY THIS FILE
	readInput(argv[1], &input);
//DO NOT MODIFY THIS FILE
	struct timespec start, finish;
	double elapsed;
//DO NOT MODIFY THIS FILE
	clock_gettime(CLOCK_MONOTONIC, &start);
	resize(&input, &output);
	clock_gettime(CLOCK_MONOTONIC, &finish);
//DO NOT MODIFY THIS FILE
	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
//DO NOT MODIFY THIS FILE
	printf("%lf\n", elapsed);
//DO NOT MODIFY THIS FILE
	writeData(argv[2], &output);
	return 0;
}
