/**
  Author: Adrian Airinei
  
  Test open syscall time response.	
*/
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>


int main(void)
{
	//char fileName[30];
	char fileTest[30];
	char *number;
	int f, i;
	struct timeval tv;
	suseconds_t currentTime;
	suseconds_t currentTime2;

	number = (char*)malloc(5*sizeof(char));

    gettimeofday(&tv, NULL); 
    currentTime = tv.tv_usec;
	
	for(i=0;i<100;i++)
	{
		sprintf(number, "%d", i);
		strcpy(fileTest, "\tmp\test");
		strcat(fileTest, number);
		strcat(fileTest, ".txt");
		
		f = open(fileTest, O_RDONLY);
	}

	gettimeofday(&tv, NULL); 
    currentTime2 = tv.tv_usec;
	
	printf("total time: %ld", currentTime2-currentTime);
	
	return 0;
}
