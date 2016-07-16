#include <IRremote.h>

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  pinMode(RECV_PIN, INPUT);
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Receiver initialized!");
}
  
void loop() {
  if (irrecv.decode(&results)) {
    Serial.print("Received: ");
    Serial.println(results.value);
    Serial.println();
    irrecv.resume(); // Receive the next value
  }
  else
  {
    Serial.println("Nothing...");
  }
  delay(100);
}
