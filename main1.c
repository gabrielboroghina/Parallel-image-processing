#include "homework1.h"
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

//DO NOT MODIFY THIS FILE
extern int num_threads;
extern int resolution;
int main(int argc, char * argv[]) {
	//argv[1] output
	//argv[2] resolution
	//argv[3] num_threads
	if(argc < 4) {
		printf("Incorrect number of arguments\n");
		exit(-1);
	}
	resolution = atoi(argv[2]);
	num_threads = atoi(argv[3]);
//DO NOT MODIFY THIS FILE
	image im;
//DO NOT MODIFY THIS FILE
	initialize(&im);
//DO NOT MODIFY THIS FILE
	struct timespec start, finish;
	double elapsed;
//DO NOT MODIFY THIS FILE
	clock_gettime(CLOCK_MONOTONIC, &start);
	render(&im);
	clock_gettime(CLOCK_MONOTONIC, &finish);
//DO NOT MODIFY THIS FILE
	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
//DO NOT MODIFY THIS FILE
	printf("%lf\n", elapsed);
//DO NOT MODIFY THIS FILE
	writeData(argv[1], &im);
	return 0;
}
