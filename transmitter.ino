
#include <SPI.h>
#include "RF24.h"

char msg[23]; //SCK -> 13//MISO -> 12//MOSI -> 11//CSN -> 7//CE -> 8
String input;

RF24 radio(8,7);

const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup(void)

{

radio.begin();
Serial.begin(9600);
radio.openWritingPipe(pipe);

}

void loop(void)

{

  if (Serial.available() > 0) {
      // read the incoming byte:
      input = Serial.readString();

      // say what you got:
      Serial.print("I received: ");
      Serial.print(input);
    }

radio.write(&input, 100);s

}
