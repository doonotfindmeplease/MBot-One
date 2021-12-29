#include "OpticalEncoder.h"
#include "Motor.h"
#include "PID.h"
#include <dummy.h>

void setup(){
  Serial.begin(115200); 
  pinMode(encoder_pin, INPUT);
  attachInterrupt(encoder_pin, counter, RISING);
  startTime = millis();

  Serial.print("Seconds ");
  Serial.print("RPM ");
  Serial.print("Pulses ");
  Serial.println("Velocity[Km/h]");

  ledcSetup(PWM_pin, PWM_freq, PWM_reso);
  ledcAttachPin(VR_Speed, PWM_pin);

  pinMode(EL_Start_Stop, OUTPUT);
  pinMode(Signal_Hall, INPUT);
  pinMode(ZF_Direction, OUTPUT);
  attachInterrupt(Signal_Hall, plus, CHANGE);

  digitalWrite(EL_Start_Stop, LOW);
  digitalWrite(ZF_Direction, HIGH);
  ledcWrite(PWM_pin, 50);
  delay(1000);
  digitalWrite(EL_Start_Stop, HIGH);
  delay(4000);
  
  for(int i = 0; i < 4; i++){
    delay(1000);
    gpio_intr_disable(encoder_pin); //Don't process interrupts during calculations
    rpm = (60 * 1000 / pulsesperturn ) / 1000 * pulses; // calculate rpm
    velocity = rpm * 3.1416 * wheel_diameter * 60 / 1000000;
    Serial.println("SETUP: " + String(velocity));
    pulses = 0; 
    gpio_intr_enable(encoder_pin); // Restart the interrupt processing
  }
}

void loop(){
  currentTime = millis();
//  while(pos<steps){
    digitalWrite(EL_Start_Stop, HIGH);
    if(currentTime - startTime >= 1000){ 
      gpio_intr_disable(encoder_pin); //Don't process interrupts during calculations
      rpm = (60 * 1000 / pulsesperturn ) / (currentTime - startTime) * pulses; // calculate rpm
      velocity = rpm * 3.1416 * wheel_diameter * 60 / 1000000; // convert to km/h
      Serial.println("V: " + String(velocity));
      computePID(velocity); //gets M1_0to255 value
      Serial.println(M1_0to255);
      ledcWrite(PWM_pin, M1_0to255);
      pulses = 0; 
      gpio_intr_enable(encoder_pin); // Restart the interrupt processing
      startTime = currentTime;
    }
//  }
//    ledcWrite(PWM_pin, 0);
//    digitalWrite(EL_Start_Stop, LOW);
}

/*
 * Wheel has torque issues when starting up. Might be due to poor connection in wires.
 */
