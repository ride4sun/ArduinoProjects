
   //////////////////////////////////////////////
  //        HALL EFFECT SENSOR DEMO           //
 //                                          //
//           http://www.educ8s.tv           //
/////////////////////////////////////////////
int hallSensorPin = 3;     
int ledPin =  13;    
int state = 0;  
        
void setup() {
  Serial.begin (115200);
  pinMode(ledPin, OUTPUT);      
  pinMode(hallSensorPin, INPUT);     
}
void loop(){
  
  state = digitalRead(hallSensorPin);
  if (state == LOW) {        
    digitalWrite(ledPin, HIGH); 
    Serial.println (" YES YES Magnet Detected"); 
    delay (5);
  } 
  else {
    digitalWrite(ledPin, LOW);
    Serial.println (" No Magnet "); 
    delay (5) ;
  }
}
