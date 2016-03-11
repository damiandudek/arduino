/*
 * dht11.cpp
 *
 * Version: 1.0
 * Creation date:
 * 
 * Datasheet: http://www.micro4you.com/files/sensor/DHT11.pdf
 *
 * Author: Damian Dudek
 *
 */
 
 #include "dht11.h"
 
DHT11::DHT11(uint8_t pin){
 	this->_dhtPin = pin;	// set dht11 pin
 	this->_lastReadTime = 0;
	this->humidity = DHT11_INVALID_VALUE;
	this->temperature = DHT11_INVALID_VALUE;
 	pinMode(this->_dhtPin, INPUT);
 	digitalWrite(this->_dhtPin, HIGH);
}
 
DHT11::~DHT11(){
}

byte DHT11::readByteOfData(){
	byte r = 0;

	// data transmission (read bit by bit form the most significant)  	
	for(int8_t i = 7; i >= 0; i--){
		// each data bit starts with 50us low signal
		while(digitalRead(this->_dhtPin) != HIGH);	// wait 50us for high signal
		delayMicroseconds(41);	// 26-28us means that 1-bit data is "0"; 41 is for safety
		
		// ~70us voltage-length means 1-bit data is "1"

		if(digitalRead(this->_dhtPin) == HIGH){
			r |= 1 << (i);
			while(digitalRead(this->_dhtPin) != LOW);	// wait for high signal
		}	
 	}
 	return r;
}
 
int8_t DHT11::measure(){
	// check if dht11 is ready
 	if(((millis() - this->_lastReadTime) < DHT11_MIN_TIME_BETWEEN_READINGS) && this->_lastReadTime != 0)
 		return DHT11_NOT_READY;
 	
 	pinMode(this->_dhtPin, OUTPUT);
 	digitalWrite(this->_dhtPin, LOW);	// send "Start" signal to DHT11 sensor
 	delay(DHT11_START_SIGNAL);	// DHT11 needs at least 18ms to detect signal
 	
 	digitalWrite(this->_dhtPin, HIGH);
 	pinMode(this->_dhtPin, INPUT);
 	
 	delayMicroseconds(DHT11_WAIT_FOR_DHT_RESPONSE);	// wait 40us; waiting for DHT11 response (20 - 40us)
	
 	// wait for first low signal from DHT sensor(80us)
 	uint32_t start = micros();	
 	while(digitalRead(this->_dhtPin) != HIGH){
		if(micros() > start + DHT11_RESPONSE_TIMEOUT) return DHT11_TIMEOUT_ERROR;
	}
	

 	// wait for first high signal from DHT sensor(80us)
 	start = micros();	
 	while(digitalRead(this->_dhtPin) != LOW){
		if(micros() > start + DHT11_RESPONSE_TIMEOUT) return DHT11_TIMEOUT_ERROR;
	}
 	
 	// data format: 8-bits integral RH, 8-bits decimal RH, 8-bits integral T, 8-bits decimal T, 8-bits checksum
 	
 	byte humidityIntegral = this -> readByteOfData();	// integral part of humidity
	byte humidityDecimal = this -> readByteOfData();	// decimal part of humidity
	byte temperatureIntegral = this -> readByteOfData();	// integral part of temperature
	byte temperatureDecimal = this -> readByteOfData();	// decimal part of temperature
	byte cechksum = this -> readByteOfData();	// check sum

	if(cechksum != (humidityIntegral + humidityDecimal + temperatureIntegral  + temperatureDecimal))
		return DHT11_CHECKSUM_ERROR;
		
	this->humidity = humidityIntegral + humidityDecimal;
	this->temperature = temperatureIntegral + temperatureDecimal;

	this->_lastReadTime = millis();
	return 0;	
}

int8_t DHT11::getTemperature(){
	if(this->temperature != DHT11_INVALID_VALUE)
		return this->temperature;
}

int8_t DHT11::getHumidity(){
	if(this->humidity != DHT11_INVALID_VALUE)
		return this->humidity;
}

float DHT11::getFahrenheit(){
	if(this->temperature != DHT11_INVALID_VALUE)
		return 1.8 * this->temperature + 32;
}

float DHT11::getKelvin(){
	if(this->temperature != DHT11_INVALID_VALUE)
		return this->temperature + 273.15;
}
