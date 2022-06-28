byte potPin = A0;
//long int potVal;
int potVal = analogRead(A0);
void setup() {
  Serial.println("Setup Start");
  // put your setup code here, to run once:

Serial.begin(115200);
}

void loop()
   {
  // put your main code here, to run repeatedly:

potVal = analogRead(potPin);
Serial.println(potVal);
delay(200);
}
