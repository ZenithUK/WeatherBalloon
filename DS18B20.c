#include "DS18B20.h"

int getTemperature(){

	//printf("S1\n");

	ds18b20=open(ds18b20Path,O_RDONLY);
	//printf("S2\n");
	if(-1 == ds18b20)
	{
		//printf("S3\n");
		perror("Open Failed");
		return 0;
	}

	//printf("S4\n");

	// Get size.
	off_t size = lseek(ds18b20, 0, SEEK_END); // You should check for an error return in real code

	//printf("S5\n");

	if (size != -1){
		// Seek back to the beginning.
		lseek(ds18b20, 0, SEEK_SET);
		// Allocate enough to hold the whole contents plus a '\0' char.
		char *buff = NULL;
		buff = malloc(size + 1);

		//printf("S6\n");

		if(buff == NULL || read(ds18b20,buff,size)<=0)
   		{

			//printf("S7\n");
      			perror("Read Failed");

			if (buff != NULL)
				free(buff);
			
      			return 0;
   		}

		//printf("S8\n");

		int temp = atoi(strstr(buff,"t=")+2);
		externalTemp = ((float)temp) / 1000;

		if (buff != NULL)
			free(buff);

		//printf("S9\n");

		while(close(ds18b20) == 0);

		//printf("S10\n");

		return 1;
	}else{
		return 0;
	}

}

int initialiseDS18B20() {

	system("modprobe w1-gpio");
	system("modprobe w1-therm");

	glob_t  globbuf;

    	glob("/sys/bus/w1/devices/*28\*", 0, NULL, &globbuf);

    	if ( globbuf.gl_pathc == 0 ){
        	printf("There were no matching files\n");

		return 0;
	}else{
        	//printf("the first of the matching files is: %s\n", globbuf.gl_pathv[0]);

		char* folder = globbuf.gl_pathv[0];
		
		ds18b20Path = malloc(strlen(folder)+1+9); /* make space for the new string (should check the return value ...) */
		strcpy(ds18b20Path, folder); /* copy ds18b20Path into the new var */
		strcat(ds18b20Path, "/w1_slave");

		printf("DS18B20 Initilised.\n");

		return 1;
	}
}
