#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>

pthread_t geigerThread;
int  geigerThreadID;

extern int geigerCount;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int count = 0;

int openGPIO(unsigned int gpio){

	int fd, value;
	char buf[64];

	snprintf(buf, sizeof(buf), "echo %i >/sys/class/gpio/export", gpio);

	system(buf);	

	snprintf(buf, sizeof(buf), "echo in > /sys/class/gpio/gpio%i/direction", gpio);

	system(buf);	
}

int GPIOValue(unsigned int gpio)
{
	char buf[64];
	snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%i/value", gpio);

	int fd = open(buf, O_RDONLY);
	if (fd > 0){
		char ch = 0;
 
		read(fd, &ch, 1);

		close(fd);

		if (ch != '0') {
			return 1;
		} else {
			return 0;
		}
	}else{
		return -1;
	}
}

void *geigerHandler(){
	while(1){
		if(GPIOValue(18) == 1){		
			pthread_mutex_lock( &mutex );
			count++;
			pthread_mutex_unlock( &mutex );
		}
	}
}

int readGeigerCount() 
{
	pthread_mutex_lock( &mutex );
	geigerCount = count;
	count = 0;
	pthread_mutex_unlock( &mutex );

	return 1;
}

int initialiseGeiger() 
{
	openGPIO(18);


	if((geigerThreadID = pthread_create(&geigerThread, NULL, &geigerHandler, NULL))){
		printf("failed\n");
	}

        return 1;
}
