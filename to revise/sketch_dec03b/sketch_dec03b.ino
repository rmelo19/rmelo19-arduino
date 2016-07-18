  void setup() {
  // put your setup code here, to run once:
  pinMode(A2, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  

  analogWrite(9, 20);


  delay(1000);


  
  analogWrite(9, 0 );

  delay(1000);

  analogWrite(10, 20);


  delay(1000);


  
  analogWrite(10, 0 );

  delay(1000);

  digitalWrite(A2, LOW);


  delay(1000);


  
  digitalWrite(A2, HIGH);

  delay(1000);


//  analogWrite(10, 0);
//  analogWrite(11, 0);
  

}   
