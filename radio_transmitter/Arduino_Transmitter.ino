#include <SPI.h>
#include <RF24.h>

#define CE 9
#define CS 10
#define RED_LED 2
#define GREEN_LED 3
#define YELLOW_LED 4

RF24 radio(CE, CS);

bool button_pushed(int buttonPin) {
  // if pressed
  if (digitalRead(buttonPin) == HIGH) {
    // wait until released
    while (true) {
      if (digitalRead(buttonPin) == LOW)
        return true;
    }
  }
  return false;
}

int check_button_pushed() {
  if (button_pushed(RED_LED))
    return 1;
  else if (button_pushed(GREEN_LED))
    return 2;
  else if (button_pushed(YELLOW_LED))
    return 3;
  return 0;
}
int userInput(int *sequence) {
  for (int i = 0; i < sizeof(sequence)/sizeof(int); i++) {
    Serial.println("Press button " + String(sequence[i]));
    while (true) {
      //  if red pressed: 1, green: 2, yellow: 3
      int button_pushed = check_button_pushed();

      if (button_pushed != 0) {
        if (button_pushed == sequence[i])
          break;
        else
          return 2;
      }
    }
  }
  return 1;
}

void setup() {
  radio.begin();
  radio.setChannel(2); 
  radio.setPALevel(RF24_PA_MIN);
  
  radio.stopListening();
  
  uint8_t writeAddress[] = { 0xC2, 0xC2, 0xC2, 0xC2, 0xC2 };
  radio.openWritingPipe(writeAddress);
  uint8_t readAddress[] = { 0xE7, 0xE7, 0xE7, 0xE7, 0xE7 };
  radio.openReadingPipe(1, readAddress);

  radio.setCRCLength(RF24_CRC_16);

  pinMode(RED_LED, INPUT);
  pinMode(GREEN_LED, INPUT);
  pinMode(YELLOW_LED, INPUT);

  Serial.begin(9600);
}

int rounds = 1;
int* lightSequence = (int*) malloc( sizeof( int ) );
int buf;
bool roundStarted;
void loop() {
  radio.startListening();
  lightSequence = (int*) malloc( sizeof( int )*rounds );
  if( radio.available() )
  {
     // TEST: sending over a character, one byte at a time
     radio.read((void*)&buf, sizeof(int));

     Serial.print( "Received: " );
     Serial.println( buf );

     if (buf == 1 || buf == 2 || buf == 3) {
        lightSequence[rounds-1] = buf;
        radio.stopListening();
        int result = userInput(lightSequence);
        if (result == 1) {
          Serial.println("ROUND SUCCESSFUL"); 
          rounds++;
        } else {
          Serial.println("ROUND FAILED");
        }
        
        radio.write((void*) &result, sizeof(int), 1);
        Serial.write("wrote result");
     }
     
     // stop listening after bytes are received
     radio.stopListening();
  }
  free(lightSequence);
}
