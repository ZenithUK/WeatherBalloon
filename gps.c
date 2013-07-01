#include "gps.h"

// Send a byte array of UBX protocol to the GPS
void sendUBX(uint8_t *MSG, uint8_t len) {
	if (serial != -1){
		write(serial, MSG, len);
	}
}

int readUBX(char** rx_buffer, uint8_t len){
	//char* rx_buffer = malloc(len);

	assert(rx_buffer);
    	*rx_buffer = malloc(len);

	int rx_length = read(serial, (void*)*rx_buffer, len);	
	if (rx_length <= 0)
	{
		//An error occured (will occur if there are no bytes)
		return 0;
	}
	else
	{
		//Bytes received
		//return rx_buffer;
		return 1;
	}
}

int getUBX_ACK(uint8_t *MSG) {
  uint8_t b;
  uint8_t ackByteID = 0;
  uint8_t ackPacket[10];
  unsigned long startTime = millis();
  printf(" * Reading ACK response: \n");
 
  // Construct the expected ACK packet    
  ackPacket[0] = 0xB5;	// header
  ackPacket[1] = 0x62;	// header
  ackPacket[2] = 0x05;	// class
  ackPacket[3] = 0x01;	// id
  ackPacket[4] = 0x02;	// length
  ackPacket[5] = 0x00;
  ackPacket[6] = MSG[2];	// ACK class
  ackPacket[7] = MSG[3];	// ACK id
  ackPacket[8] = 0;		// CK_A
  ackPacket[9] = 0;		// CK_B
 
  // Calculate the checksums
  for (uint8_t i=2; i<8; i++) {
    ackPacket[8] = ackPacket[8] + ackPacket[i];
    ackPacket[9] = ackPacket[9] + ackPacket[8];
  }
 
  while (1) {
 
    // Test for success
    if (ackByteID > 9) {
      // All packets in order!
      printf(" (SUCCESS!)\n");
      return 1;
    }
 
    // Timeout if no valid response in 3 seconds
    if (millis() - startTime > 3000) { 
      printf(" (FAILED!)\n");
      return 0;
    }
 
    // Make sure data is available to read
	char* bData;
    if (readUBX(&bData, 1) != 0) {
	b = (int)bData[0];
 
      // Check that bytes arrive in sequence as per expected ACK packet
      if (b == ackPacket[ackByteID]) {
	 //printf ("\nWin: %i and out ", b);
        ackByteID++;
      } 
      else {
	//printf ("\nFail: %i and out ", b) ;
	//getchar();
        ackByteID = 0;	// Reset and look again, invalid order
      }
 
    }
	free(bData);
  }
}


int initialiseGPS(){

	if(openGPS() != 1){
		return 0;
	}

	printf("Setting uBlox max-6 nav mode\n");

	uint8_t setNav[] = {
    		0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00,
    		0x05, 0x00, 0xFA, 0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0xDC };
  	while(!gps_set_sucess)
	{
    		sendUBX(setNav, sizeof(setNav)/sizeof(uint8_t));
    		gps_set_sucess=getUBX_ACK(setNav);
  	}
  	gps_set_sucess=0;

   	printf("Switching off NMEA GLL: \n");
   	uint8_t setGLL[] = { 
   		0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x2B                   };
   	while(!gps_set_sucess)
   	{		
   		sendUBX(setGLL, sizeof(setGLL)/sizeof(uint8_t));
   		gps_set_sucess=getUBX_ACK(setGLL);
   	}
   	gps_set_sucess=0;

   	printf("Switching off NMEA GSA: \n");
	uint8_t setGSA[] = { 
   		0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x32                 };
   	while(!gps_set_sucess)
   	{		
   		sendUBX(setGSA, sizeof(setGSA)/sizeof(uint8_t));
   		gps_set_sucess=getUBX_ACK(setGSA);
   	}
   	gps_set_sucess=0;

   	printf("Switching off NMEA GSV: \n");
	uint8_t setGSV[] = { 
   		0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x39                 };
   	while(!gps_set_sucess)
   	{		
   		sendUBX(setGSV, sizeof(setGSV)/sizeof(uint8_t));
   		gps_set_sucess=getUBX_ACK(setGSV);
   	}
   	gps_set_sucess=0;

	printf("Switching off NMEA VTG: \n");
   	uint8_t setVTG[] = { 
   		0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x05, 0x47                   };
   	while(!gps_set_sucess)
   	{
   		sendUBX(setVTG, sizeof(setVTG)/sizeof(uint8_t));
   		gps_set_sucess=getUBX_ACK(setVTG);
	}

	printf("Switching off NMEA RMC: \n");
   	uint8_t setRMC[] = { 
   		0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x04, 0x40                   };
   	while(!gps_set_sucess)
   	{
   		sendUBX(setRMC, sizeof(setRMC)/sizeof(uint8_t));
   		gps_set_sucess=getUBX_ACK(setRMC);
   	}

	printf("Initialised GPS\n");

	while(closeGPS() == 0);

	return 1;
}

int getNMEAChecksum(char *s) {
    int c = 0;
 
    while(*s)
        c ^= *s++;
 
    return c;
}

int gpsOpen(){
}

double getLatitude(const char *token)
{
	char degrees[3];
	if (strlen(token) >= 4) {
		degrees[0] = token[0];
		degrees[1] = token[1];
		degrees[2] = '\0';
		return atof(degrees) + atof(token + 2) / 60;
	}else{
		return 0.0;
	}
}

double getLongitude(const char *token)
{
	char degrees[4];
	if (strlen(token) >= 4) {
    		degrees[0] = token[0];
    		degrees[1] = token[1];
    		degrees[2] = token[2];
    		degrees[3] = '\0';
		return atof(degrees) + atof(token + 3) / 60;
	}else{
		return 0.0;
	}
}

uint32_t getTime(const char *token)
{
	char new_time[7];
  // Time can have decimals (fractions of a second), but we only take HHMMSS
  strncpy(new_time, token, 6);
  // Terminate string
  new_time[6] = '\0';
 
  return
    ((new_time[0] - '0') * 10 + (new_time[1] - '0')) * 10000 +
    ((new_time[2] - '0') * 10 + (new_time[3] - '0')) * 100 +
    ((new_time[4] - '0') * 10 + (new_time[5] - '0'));
}

void getGPS()
{
	//printf("a\n");
	openGPS();

  	char* bData;
	int status = 0;	

	ByteArray data;
	initByteArray(&data, 0);
	int i = 0;

	//printf("b\n");

  	while(status == 0)
  	{
		//printf("c\n");
    		if (readUBX(&bData, 1) != 0) {
			//printf("d\n");
			char inByte = bData[0];

			//printf("d2\n");

			if (bData[0] == '\n' && data.array[i-1] == '\r'){

				//printf("e\n");

				char* packet = NULL;
				char* token = NULL;
				char* header = NULL;

				double tLatitude = 0.0;
				double tLongitude = 0.0;
				int tNumSatalites = 0;
				double tAltitude = 0.0;
				uint32_t tTime = 0;
				int updateData = -1;
				int updateSatData = -1;
				int updateTimeData = -1;

				if (byteArrayToChar(&data, &packet, 1 , 4) == 1){

				//printf("P: %s\n", packet);

				//printf("f\n");

				//printf("P: %s\n", packet);

				int readPos = 0;
				int tokenPos = 1;
				
				while(readPos < data.used-1 && data.used > 0 && (readPos = readArrayData(&data, &token, readPos, ','))){
					//printf("g\n");
					if (token != NULL && tokenPos == 1){
						if (header != NULL){
							//printf("h\n");
							free(header);
							header = NULL;
						}
						//printf("i\n");
						if (strlen(token) != 0){
							//printf("i2 %i\n", strlen(token));
							header = (char*)malloc(sizeof(char) * strlen(token) + 1);
						}else
							break;
						strcpy (header,token);
					}else if(token == NULL && tokenPos == 1){
						//printf("j\n");
						//bail
						break;
					}
					//printf("k\n");
					//Now to deal with different packets!
					if(strcmp(header, "$GPGGA") == 0){
						//printf("l\n");
						switch(tokenPos){
							case 2:
								if (token != NULL && (updateTimeData == -1 || updateTimeData == 1)){
									updateTimeData = 1;
									
									tTime = getTime(token);
								}else{
									updateTimeData = 0;
								}
								break;
							case 3:
								if (token != NULL && (updateData == -1 || updateData == 1)){
									updateData = 1;
									
									tLatitude = getLatitude(token);
								}else{
									updateData = 0;
								}
								break;
							case 4:
								if (token != NULL && (updateData == -1 || updateData == 1)){
									updateData = 1;
									if (strcmp(token, "S") == 0){
										tLatitude = -tLatitude;
									}
								}else{
									updateData = 0;
								}
								break;	
							case 5:
								if (token != NULL && (updateData == -1 || updateData == 1)){
									updateData = 1;
									tLongitude = getLongitude(token);
								}else{
									updateData = 0;
								}
								break;	
							case 6:
								if (token != NULL && (updateData == -1 || updateData == 1)){
									updateData = 1;
									if (strcmp(token, "W") == 0){
										tLongitude = -tLongitude;
									}
								}else{
									updateData = 0;
								}
								break;	
							case 8:
								if (token != NULL && (updateSatData == -1 || updateSatData == 1)){
									updateSatData = 1;
									tNumSatalites = atoi(token);
								}else{
									updateSatData = 0;
								}
								break;	
							case 10:
								if (token != NULL && (updateData == -1 || updateData == 1)){
									updateData = 1;
									tAltitude = atof(token);
								}else{
									updateData = 0;
								}
								break;	
							case 15:
								if (token != NULL){
									int checksum = (int)strtol(token+1, NULL, 16);
									if (checksum == getNMEAChecksum(packet))
										//printf("Checksum matches!\n");
										status = 1;
										if (updateData == 1){
											Latitude = tLatitude;
											Longitude = tLongitude;
											Altitude = tAltitude;
										}if (updateSatData == 1){
											NumSatalites = tNumSatalites;
										}if (updateTimeData == 1){
											Time = tTime;
										}
									}
								break;
						}
					//printf("m\n");
					}else{
						//bail
						if (token != NULL){
							//printf("n\n");
							free(token);
							token = NULL;
						}
						break;
					}
					tokenPos++;
					//printf("o\n");
					if (token != NULL){
						//printf("p\n");
						free(token);
						token = NULL;
					}
				}

				}else{
				}
				
				if (packet != NULL){
					free(packet);
					packet = NULL;
				}
				if (header != NULL){
					free(header);
					header = NULL;
				}
				freeByteArray(&data);
				initByteArray(&data, 0);
				i = 0;
			}else{
				insertByteArray(&data, bData[0]);
				i++;
			}
		}
		if (bData != NULL){
			free(bData);
			bData = NULL;
		}
  	}

	while(closeGPS() == 0);

}    

int closeGPS(){
	if (close(serial) == 0){
		return 1;
	}else{
		return 0;
	}
}

int openGPS() {

	//printf("Opening GPS at 9600 baud\n");

	if ((serial = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY)) < 0)
	{
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		return 0;
	}

	//CONFIGURE THE UART
	//The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
	//	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
	//	CSIZE:- CS5, CS6, CS7, CS8
	//	CLOCAL - Ignore modem status lines
	//	CREAD - Enable receiver
	//	IGNPAR = Ignore characters with parity errors
	//	ICRNL - Map CR to NL on input
	//	PARENB - Parity enable
	//	PARODD - Odd parity (else even)
	struct termios options;
	tcgetattr(serial, &options);
	options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(serial, TCIFLUSH);
	tcsetattr(serial, TCSANOW, &options);

	return 1;
}
