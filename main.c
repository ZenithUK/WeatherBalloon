#include <stdint.h>
#include <stdio.h>

double Latitude = 0.0;
double Longitude = 0.0;
int NumSatalites = 0;
double Altitude = 0.0;

float internalTemp = 0.0f;
unsigned int pressure = 0;

short ADCValue[8] = { 0 };

float externalTemp = 0.0f;

int geigerCount = 0;

uint32_t Time = 0;

int initialise() {

	setbuf(stdout, NULL);
		
	if (initialiseDS18B20() == 0)
		return 0;

	if (initialiseMCP3008() == 0)
		return 0;

	if (initialiseGPS() == 0)
		return 0;

	if (initialiseGeiger() == 0)
		return 0;

	bmp085_Calibration();

	return 1;
}

void loop() {
	while(1){
		sendString(Time, Latitude, Longitude, Altitude, internalTemp, externalTemp, NumSatalites, pressure, geigerCount, ADCValue[0]);
		getADCValue(0);
		getTemperature();
		readBMP085();
		readGeigerCount();
		//printf("UV: %i Temp: %F Lat: %F Long: %F Alt: %F SatNu: %i Pressure %i iTemp %F GMC: %i\n", ADCValue[0], externalTemp, Latitude, Longitude, Altitude, NumSatalites, pressure, internalTemp, geigerCount);
		closeRadio();
		getGPS();
		//sleep(1);
	}
}

int main() {
	initialise();

	loop();

	return 0;
}
