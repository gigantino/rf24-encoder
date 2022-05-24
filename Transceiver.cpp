#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CLK 2
#define DT 3
#define SW 4
#define LEFT_LED_PIN 5
#define RIGHT_LED_PIN 6

// radio(CE, CSN)
RF24 radio(10, 9);
const byte address[6] = "00001";

int currentStateCLK;
int lastStateCLK;

void setup() {
	radio.begin();
	radio.openWritingPipe(address);
	// https://nrf24.github.io/RF24/group__PALevel
	radio.setPALevel(RF24_PA_MIN);
	radio.stopListening();

	pinMode(CLK,INPUT);
	pinMode(DT,INPUT);
	pinMode(SW, INPUT_PULLUP);	
	pinMode(LEFT_LED_PIN, OUTPUT);
  	pinMode(RIGHT_LED_PIN, OUTPUT);
	
	lastStateCLK = digitalRead(CLK);
	
	Serial.begin(9600);
}

void loop() {
	currentStateCLK = digitalRead(CLK);
	if (currentStateCLK != lastStateCLK && currentStateCLK == 1) {
		if (digitalRead(DT) != currentStateCLK) {
			const char message[] = "DECREASE";
			radio.write(&message, sizeof(message));
      	digitalWrite(LEFT_LED_PIN, HIGH);
      	digitalWrite(RIGHT_LED_PIN, LOW);
		} else {
			const char message[] = "INCREASE";
			radio.write(&message, sizeof(message));
      	digitalWrite(LEFT_LED_PIN, HIGH);
      	digitalWrite(RIGHT_LED_PIN, LOW);
		}
	}
	// delay(100);
	lastStateCLK = currentStateCLK;
}
