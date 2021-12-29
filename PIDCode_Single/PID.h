int M1_0to255 = 0;
float error;
float targetSpeed = 3.0; //in km/h, changeable
float K = 5; //changeable constant

void computePID(float velocityIN){
  error = targetSpeed - velocityIN;
  M1_0to255 = round(K*error) + M1_0to255;
  
  if(M1_0to255 > 255){
    M1_0to255 = 255;
    Serial.println("  ERR: Upper Limit Reached");
  }
  else if (M1_0to255 < 0){
    M1_0to255 = 0;
    Serial.println("  ERR: Lower Limit Reached");
  }
}
