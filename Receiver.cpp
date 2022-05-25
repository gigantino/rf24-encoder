#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define LEFT_LED_PIN 5
#define RIGHT_LED_PIN 6
#define PWM_LED_PIN 3

// radio(CE, CSN)
RF24 radio(10, 9);
const byte address[6] = "00001";

int brightness = 0;

/*
	-1 -> none (default)
	0 -> increase
	1 -> decrease
*/
int lastValue = -1;

void setup() {
	radio.begin();
  	radio.openReadingPipe(0, address);
  	radio.startListening();

	pinMode(LEFT_LED_PIN, OUTPUT);
  	pinMode(RIGHT_LED_PIN, OUTPUT);
	pinMode(PWM_LED_PIN, OUTPUT);

	Serial.begin(9600);
}

void loop() {
	if (radio.available()) {
		char message[32] = "";
    	radio.read(&message, sizeof(message));
		if (strcmp(message, "INCREASE") == 0) lastValue = 0;
		else if (strcmp(message, "DECREASE") == 0) lastValue = 1;
	}

	// Increase
	if (lastValue == 0) {
		digitalWrite(LEFT_LED_PIN, HIGH);
      digitalWrite(RIGHT_LED_PIN, LOW);      
      if (brightness < 255) brightness++;
	} 
	// Decrease
	else if (lastValue == 1) {
		digitalWrite(LEFT_LED_PIN, LOW);
		digitalWrite(RIGHT_LED_PIN, HIGH);      
		brightness--;
	}
	
	if (brightness < 0) brightness = 0;
	analogWrite(PWM_LED_PIN, brightness);
}
