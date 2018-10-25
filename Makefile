all: homework homework1

homework: homework.c homework.h main.c
	gcc -o homework homework.c main.c -lpthread -Wall -lm

homework1: homework1.c homework1.h main1.c
	gcc -o homework1 homework1.c main1.c -lpthread -Wall -lm

.PHONY: clean
clean:
	rm -f homework
	rm -f homework1
	rm -rf out/*
