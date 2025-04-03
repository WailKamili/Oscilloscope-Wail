int FA=1/1;
void setup() { 
  Serial.begin(9600); 
} 
//(2*3.14159265359)
void loop() { 

  for (float x = 0; x < 999999999; x++) { 
    Serial.println(sin(x/8)); 
    delay(200); 
  } 
} 
