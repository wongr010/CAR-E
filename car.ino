/*//#include <Servo.h>
  #include <SoftwareSerial.h>

  #include <SPI.h>

  #include "RF24.h"
*/
#define MSGLEN 50
#define LL_LEN 50
//Servo myservo;

//SCK -> 13//MISO -> 12//MOSI -> 11//CSN -> 7//CE -> 8

//RF24 radio(8, 7);

//const uint64_t pipe = 0xE8E8F0F0E1LL;

char msg[LL_LEN];
char instruction[LL_LEN];
String turn_left, turn_right, straight, ennd;
//int i = 0;

// Pins
const int TRIG_PIN = 10;
const int ECHO_PIN = 13;
const int LEFT_MOTOR = 3;
const int RIGHT_MOTOR = 5;

const unsigned int LOWER_BOUND = 10;

void setup() {
  // The Trigger pin will tell the sensor to range find
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(LEFT_MOTOR, OUTPUT);
  pinMode(RIGHT_MOTOR, OUTPUT);

  // We'll use the serial monitor to view the sensor output
  Serial.begin(9600);
  /*radio.begin();
    radio.openReadingPipe(1, pipe);
    radio.startListening();
  */
  turn_left = String("turn-left");
  turn_right = String("turn-right");
  straight = String("straight");
  ennd = String("end");
}

void loop() {

  //if (radio.available()) {

  //bool done = false;

  //while (!done) {
  //done = radio.read(msg, MSGLEN);

  //if (i < 1) { // received msg
  //Serial.print(msg);
  //Serial.print('\n');
  //int comma_pos;

  float distance = 1;

  //msg = "0.3 km,turn-left,end\0";

  msg[0] = '0';
  msg[1] = '.';
  msg[2] = '3';
  msg[3] = ' ';
  msg[4] = 'k';
  msg[5] = 'm';
  msg[6] = ',';
  msg[7] = 't';
  msg[8] = 'u';
  msg[9] = 'r';
  msg[10] = 'n';
  msg[11] = '-';
  msg[12] = 'l';
  msg[13] = 'e';
  msg[14] = 'f';
  msg[15] = 't';
  msg[16] = ',';
  msg[17] = 'e';
  msg[18] = 'n';
  msg[19] = 'd';
  msg[20] = '\0';

  bool is_distance = false;
  int instr_index = 0;

  // parse through received message string
  for (int n = 0; msg[n] != '\0'; n++ ) {
    if (msg[n] == ' ') {          // space indicates a distance was received
      is_distance = true;
      if (msg[n + 1] == 'k') {    // convert km -> m
        distance = distance * 1000;
        n = n + 2;                // skip to comma
      } else {
        n = n + 1;
      }
      distance = distance * atof(instruction);    // get distance
    } else if (msg[n] == ',') {     // comma, do stuff then erase instruction for new one to parse into
      if (is_distance) {
        for ( int i = 0; i < sizeof(instruction);  ++i )    // erase instruction
          instruction[i] = (char)0;
        is_distance = false;
        instr_index = 0;          // reset instruction index
      } else if (is_distance == false) {
        if ((String)instruction == turn_left) {             // stop right motor, move left motor
          Serial.print("turn left for ");
          Serial.print(distance);
          Serial.println("m");
          analogWrite(LEFT_MOTOR, 255);
          analogWrite(RIGHT_MOTOR, 0);
          // check for obstacles before continuing
          sensors(distance);
          //delay(10 * distance); // proportional to distance
        } else if ((String)instruction == turn_right) {     // stop left motor, move right motor
          Serial.print("turn right for ");
          Serial.print(distance);
          Serial.println("m");
          analogWrite(LEFT_MOTOR, 0);
          analogWrite(RIGHT_MOTOR, 255);
          // check for obstacles before continuing
          sensors(distance);
          //delay(10 * distance);           // proportional to distance
        } else if ((String)instruction == straight) {       // move both motors
          Serial.print("go straight for ");
          Serial.print(distance);
          Serial.println("m");
          analogWrite(LEFT_MOTOR, 255);
          analogWrite(RIGHT_MOTOR, 255);
          // check for obstacles before continuing
          sensors(distance);
          //delay(10 * distance); // proportional to distance
        }
        for ( int i = 0; i < sizeof(instruction);  ++i ) {    // erase instruction
          instruction[i] = (char)0;
        }
        instr_index = 0;
      }
    } else {
      instruction[instr_index] = msg[n];      // append message characters to instruction
      instr_index = instr_index + 1;          // increment instruction index
    }

  } if ((String)instruction == ennd) {        // end of message, end program
    Serial.println("reached destination");
    analogWrite(LEFT_MOTOR, 0);
    analogWrite(RIGHT_MOTOR, 0);
    endloop();
  }
}

void sensors(float distance) {

  unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;
  float cm;

  for (int a = 0; a < 10; a++) {

    // Hold the trigger pin high for at least 10 us
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Wait for pulse on echo pin
    while ( digitalRead(ECHO_PIN) == 0 );

    // Measure how long the echo pin was held high (pulse width)
    // Note: the micros() counter will overflow after ~70 min
    t1 = micros();
    while ( digitalRead(ECHO_PIN) == 1);
    t2 = micros();
    pulse_width = t2 - t1;

    // Calculate distance in centimeters and inches. The constants
    // are found in the datasheet, and calculated from the assumed speed
    //of sound in air at sea level (~340 m/s).
    cm = pulse_width / 58.0;

    // Print out results
    if (cm > LOWER_BOUND) {   // move car
      Serial.print(cm);
      Serial.println(" cm \t");
    } else {                     // stop car
      Serial.println("STOP! Obstacle ahead!");
      analogWrite(LEFT_MOTOR, 0);
      analogWrite(RIGHT_MOTOR, 0);
      endloop();
    }
    // Wait distance ms before next measurement
    delay(distance);
    
  }
}

// end program loop
void endloop() {
  while (1) {
    
  }
}
