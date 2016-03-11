/*
 * dht11.h
 *
 * Version: 1.0
 * 
 * DHT11 Temperature & Humidity Sensor library for Adruino
 * 
 * Datasheet: http://www.micro4you.com/files/sensor/DHT11.pdf
 *
 * Author: Damian Dudek
 *
 */
 
#ifndef DHT11_H_
#define DHT11_H_
 
#if defined(ARDUINO) && (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

/* set parameters of dht11 sensor */
#define DHT11_MIN_TIME_BETWEEN_READINGS 1000 // 1000ms; min time between readings
#define DHT11_START_SIGNAL 25 // ms; DHT11 needs at least 18ms to detect signal; 25 is for safety
#define DHT11_WAIT_FOR_DHT_RESPONSE 40 // us; DHT11 nedds 20-40us for response
#define DHT11_RESPONSE 80	// us
#define DHT11_RESPONSE_TIMEOUT (DHT11_RESPONSE + 70)	// us

// dht11 status
#define DHT11_OK 0
#define DHT11_NOT_READY -1
#define DHT11_TIMEOUT_ERROR -2
#define DHT11_CHECKSUM_ERROR -3

#define DHT11_INVALID_VALUE -99

class DHT11 {
	private: 
		uint8_t _dhtPin;
		uint32_t _lastReadTime;
		
		byte readByteOfData();
	
	public:
		int8_t humidity;
		int8_t temperature;

		DHT11(uint8_t pin);
		~DHT11();
			
		int8_t measure();
		int8_t getTemperature();
		int8_t getHumidity();
		float getFahrenheit();
		float getKelvin();
};

#endif /* DHT11_H_ */
