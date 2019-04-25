#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>

int main()
{
	shmctl(3407877, IPC_RMID, NULL);
}