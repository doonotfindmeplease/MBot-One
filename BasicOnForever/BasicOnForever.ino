/* Basic control of both wheels with ESP32
 *  
 * ZF is a bit buggy, not sure if because of poor wire connection
 *  
 * Notes:
 * Avoid pins 6 to 11 (connected to integrated SPI flash)
 * All output pins suitable for PWM
 * All GPIO pins configurable for interrupts (Though pin 5 seemed buggy)
 * Pins 34 to 39 input only, NO internal pullup/down
*/

//Note: SUPER important for GND and 5V SET pins to be connected
//ALL Hall pins must be connected correctly to work

//GPIO pins
const int M1_EL = 32;     const int M2_EL = 18; //Start Stop
const int M1_VR = 22;     const int M2_VR = 21; //Speed
const int M1_ZF = 33;     const int M2_ZF = 19; //Direction
const int M1_Signal = 34; const int M2_Signal = 35; //Hall

//PWM settings
const int PWM_freq = 5000;
const int PWM_reso = 8;
const int M1_PWM_pin = 0; const int M2_PWM_pin = 5;

//Initial Settings
volatile int M1_pos = 0; volatile int M2_pos = 0
int steps = 500;
int SPEED = 70;

//Boolean Declarations
bool M1_STATE = M1_pos<steps;
bool M2_STATE = M2_pos<steps;

void IRAM_ATTR M1_plus(){
  M1_pos++;
  Serial.print("Motor ONE: " + String(M1_pos));
}

void IRAM_ATTR M2_plus(){
  M2_pos++;
  Serial.println("  Motor TWO: " + String(M2_pos));
}

void setup(){
  Serial.begin(115200);
  
  ledcSetup(M1_PWM_pin, PWM_freq, PWM_reso); ledcSetup(M2_PWM_pin, PWM_freq, PWM_reso);
  ledcAttachPin(M1_VR, M1_PWM_pin);          ledcAttachPin(M2_VR, M2_PWM_pin);
  pinMode(M1_EL, OUTPUT);                    pinMode(M2_EL, OUTPUT);
  pinMode(M1_Signal, INPUT);                 pinMode(M2_Signal, INPUT);
  pinMode(M1_ZF, OUTPUT);                    pinMode(M2_ZF, OUTPUT);
  attachInterrupt(M1_Signal, M1_plus, CHANGE);  attachInterrupt(M2_Signal, M2_plus, CHANGE);

  digitalWrite(M1_EL, LOW); digitalWrite(M2_EL, LOW);
  digitalWrite(M1_ZF, HIGH); digitalWrite(M2_ZF, HIGH);
  ledcWrite(M1_PWM_pin, SPEED); ledcWrite(M2_PWM_pin, SPEED);
  
  delay(1000);
}

void loop(){
  // Run forever
  digitalWrite(M1_EL, HIGH);
  digitalWrite(M2_EL, HIGH);
}
