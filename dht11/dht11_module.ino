/*
 * dht11_module.ino
 *
 * Author: Damian Dudek
 *
*/

#define HTTP_REQUEST_FILE "req.php"

//===================================================================
// config: DHT11 Humidity & Temperature Sensor
//-------------------------------------------------------------------

#include <dht11.h>  // dht11 library

#define DHT_PIN 7  // data pin in dht11
DHT11 dht11(DHT_PIN);  

#define DHT11_TIME_BETWEEN_READINGS 5000  // ms; time between readings

unsigned long dhtTimer;  // timer for dht11

// end config: DHT11 Humidity & Temperature Sensor
//===================================================================


//===================================================================
// config: LCD i2c converter
//-------------------------------------------------------------------

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define BACKLIGHT_PIN 3
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7);

// end config: LCD i2c converter
//===================================================================


//===================================================================
// config: ETHERNET
//-------------------------------------------------------------------

#include <SPI.h>
#include <Ethernet.h>

// MAC address for controller 
// Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Static IP address of the server
IPAddress serverIP(192, 168, 1, 1);  // IP addres of server (no DNS)
//char server[] = "www.serveraddress.com";    // DNS

// DNS services address
IPAddress dnsAddress(192, 168, 1, 1);

// Gateway (comupter or router address)
IPAddress gateway(192, 168, 1, 1);
	
// Subnet
IPAddress subnet(255, 255, 255, 0);

// Static IP address of the microcontroller (to use if the DHCP fails to assign)
IPAddress ip(192, 168, 1, 10);

EthernetClient client;

// end config: ETHERNET
//===================================================================

void setup(){
  Serial.begin(9600);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Arduino Leonardo only
  }
  
  Serial.println("Program START"); 
//-------------------------------------------------------------------

  // LCD i2C
  lcd.begin(16, 2);
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.clear();
  lcd.home();
  lcd.print("Inicjalizacja..");  
  
//-------------------------------------------------------------------

  dhtTimer = -5000;  // assign value 
  
//-------------------------------------------------------------------

  Ethernet.begin(mac, ip, dnsAddress, gateway, subnet);
  
  // give the ethernet module time to boot up:
  delay(1000);
  
  // print the Ethernet board/shield's IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
}
// end setup();
//===================================================================



// makes a HTTP connection to the server:
// rl, file, params, interval in ms
void httpRequest(String url, String file, String params) {
  
  // if there's a successful connection:
  if (client.connect(serverIP, 80)) {
    
    Serial.println("Connected");
    
    // send the HTTP GET request:
    client.print("GET ");
    client.print(url);
    client.print(file);
    client.print(params);
    client.println(" HTTP/1.1");
    client.println("Host: www.arduino.cc");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("Connection failed");
    Serial.println("Disconnecting");
    client.stop();
  }
}
// end httpRequest();
//===================================================================

char buffer[60];  // buffor for the request data
int8_t errorCode;


int8_t dht11read(){  
  if (millis() > dhtTimer + DHT11_TIME_BETWEEN_READINGS){
    dhtTimer = millis();
    
    if((errorCode = dht11.measure()) == 0){
      // prepare data to send
      sprintf(buffer, "?humidity=%d&temperature=%d", (int)dht11.humidity, (int)dht11.temperature);

      httpRequest("/", HTTP_REQUEST_FILE, buffer);
      Serial.println("Readings sent to server");
      Serial.println("Disconnecting");
      client.stop();
      
      Serial.print("temperature:");
      Serial.print(dht11.temperature);
      Serial.print("C");        
      Serial.print(" humidity:");
      Serial.print(dht11.humidity);
      Serial.println("%");  
      return true;  
    }
    else{
      Serial.println();
      Serial.print("Error Code:");
      Serial.print(errorCode);
      Serial.println();    
    }
  }
  return false;
}
// end dht11read();
//===================================================================

void sendToLCD(){
    // display humidity and temperature on the LCD
    lcd.clear();
    lcd.print("Temp: ");  
    lcd.print(dht11.temperature);
    lcd.print((char)223);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print ("Humi: ");
    lcd.print (dht11.humidity);
    lcd.print("%");
}
// end sendToLCD();
//===================================================================

void loop(){
  if(dht11read())
    sendToLCD();
}
// end loop();
