#include "Arduino.h"

byte potPin = A0;
int potVal  = analogRead(A0);

void setup()
{
    Serial.println("Setup Start");
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    potVal = analogRead(potPin);
    Serial.println("potVal4:");
    Serial.println(potVal);
    digitalToggleFast(LED_BUILTIN);
    delay(250);
}
