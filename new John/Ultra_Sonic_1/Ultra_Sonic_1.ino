//********* Ultra_Sonic_1

#include <NewPing.h>

#define TRIGGER_PIN 9
#define ECHO_PIN  10
#define MAX_DISTANCE 200

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(9600);
  delay(50);
}

void loop() {

Serial.print("Distance is:"  );
Serial.println(sonar.ping_cm());
 delay(1000);
}
