#ifndef GPS_H
#define GPS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <assert.h>
#include <sys/time.h>
#include "Utils.h"

extern double Latitude;
extern double Longitude;
extern int NumSatalites;
extern double Altitude;
extern uint32_t Time;

int serial;
//uint64_t epochMilli, epochMicro;
char gps_set_sucess = 0;

//unsigned int millis (void);

void sendUBX(uint8_t *MSG, uint8_t len);

int readUBX(char** rx_buffer, uint8_t len);

int getUBX_ACK(uint8_t *MSG);

int initialiseGPS();

int getNMEAChecksum(char *s);

int gpsOpen(); 

void getGPS();   

int closeGPS();

int openGPS();

#endif
