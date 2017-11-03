#include <SPI.h>
#include <RF24.h>

#define CE 9
#define CS 10


RF24 radio(CE, CS);

void setup() {
  radio.begin();
  radio.setChannel(2); 
  radio.setPALevel(RF24_PA_MIN);
  
  radio.stopListening();
  
  uint8_t writeAddress[] = { 0xC2, 0xC2, 0xC2, 0xC2, 0xC2 };
  radio.openWritingPipe(writeAddress);

  radio.setCRCLength(RF24_CRC_16);
  Serial.begin(9600);
}

char buf = 'a';
void loop() {
  Serial.println("sent a");
//  uint8_t data;

//  if( radio.available() )
//  {
//      // read in bytes
//  }
//  else
//  {
//  }
  radio.write((void*)&buf, 1, false);

  delay(1000);

}
