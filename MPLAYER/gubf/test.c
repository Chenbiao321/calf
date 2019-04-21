#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <signal.h>

enum mode{MODE1, MODE2} mode;

void fun(FILE *fp)
{
	fprintf(fp, "hello\n");
	getchar();
}

int main()
{
	FILE *pp = fopen("CBB.txt", "w");

	fun(pp);

	return 0;
}