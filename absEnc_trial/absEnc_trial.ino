//Tested and working using Arduino Mega 19/04/21
//                         ESP32        20/04/21
//Mega: D4 D5 D6    ESP: D25 D26 D27
const int CS_PIN = 25;     //yellow
const int CLOCK_PIN = 26;  //green
const int DATA_PIN = 27;   //white
unsigned int reading;
int result;

void setup() {
  pinMode(DATA_PIN, INPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(CS_PIN, OUTPUT);

  Serial.begin(115200);
}

void loop() {
  result = getAbsEncReading();
  Serial.println(result);
}

int getAbsEncReading(){
  digitalWrite(CS_PIN, LOW);
  reading = 0;
  
  for(byte i = 0; i < 10; i++){ //10-bit
    digitalWrite(CLOCK_PIN, LOW);
    delayMicroseconds(1); // must be >=1 
    digitalWrite(CLOCK_PIN, HIGH);
    delayMicroseconds(1); // must be >=1
    reading = (reading << 1) + digitalRead(DATA_PIN); //shifts bits leftwards
  }

  digitalWrite(CS_PIN, HIGH);
  return reading;
}
