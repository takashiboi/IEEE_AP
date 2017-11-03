#include <SPI.h>
#include <RF24.h>

#define CE 9
#define CS 10
#define RED_LED 2
#define GREEN_LED 3
#define YELLOW_LED 4

RF24 radio(CE, CS);

void setup() {
  radio.begin();
  radio.setChannel(2); 
  radio.setPALevel(RF24_PA_MIN);
  
  radio.stopListening();
  
  uint8_t readAddress[] = { 0xC2, 0xC2, 0xC2, 0xC2, 0xC2 };
  radio.openReadingPipe(1, readAddress);

  radio.setCRCLength(RF24_CRC_16);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
}

char buf;
void loop() {
  radio.startListening();

  if( radio.available() )
  {
     // TEST: sending over a character, one byte at a time
     radio.read((void *)&buf, 1);

     //char character = *(char*) buf;
     Serial.print("Character received: ");
     Serial.println(buf);

     if (buf == 'a') {
        digitalWrite(GREEN_LED, HIGH);
        delay(300);
        digitalWrite(GREEN_LED, LOW);
     }
     
     // stop listening after bytes are received
     radio.stopListening();
  }
}
