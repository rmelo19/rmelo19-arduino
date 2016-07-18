const int interruptA = 0;       // Interrupt 0 
const int interruptB = 1;       // Interrupt 1 

int CLK = 2;     // PIN2
int DAT = 3;     // PIN3
int BUTTON = 4;  // PIN4
int LED1 = 5;    // PIN5
int LED2 = 6;    // PIN6
int COUNT = 0;

void setup()
{
  attachInterrupt(interruptA, RoteStateChanged, FALLING);
  // attachInterrupt(interruptB, buttonState, FALLING);
  pinMode(CLK, INPUT);
  digitalWrite(CLK, HIGH);  // Pull High Restance
  pinMode(DAT, INPUT);
  digitalWrite(DAT, HIGH);  // Pull High Restance
  pinMode(BUTTON, INPUT);
  digitalWrite(BUTTON, HIGH);  // Pull High Restance
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  Serial.begin(19200);
}

void loop()
{
  if  (!(digitalRead(BUTTON)))
  {
    COUNT = 0;
    Serial.println("STOP COUNT = 0");
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    delay (2000);
  }
  Serial.println(COUNT);
}

//-------------------------------------------
void RoteStateChanged() //When CLK  FALLING READ DAT
{
  if  (digitalRead(DAT)) // When DAT = HIGH IS FORWARD
  {
    COUNT++;
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    delay(20);
  }
  else                   // When DAT = LOW IS BackRote
  {
    COUNT--;
    digitalWrite(LED2, HIGH);
    digitalWrite(LED1, LOW);
    delay(20);
  }
}




