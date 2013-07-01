#ifndef DS18B20_H
#define DS18B20_H

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <assert.h>
#include <glob.h>

int ds18b20;
char* ds18b20Path;

extern float externalTemp;

int getTemperature();

int initialiseDS18B20();

#endif
