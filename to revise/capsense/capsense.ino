  

int led = 7;                                          //change '42' to any desired pin...

long time = 0;
int state = HIGH;

boolean yes;
boolean previous = false;

int debounce = 200;

CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,2);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired

// To add more sensors...
//CapacitiveSensor   cs_4_6 = CapacitiveSensor(4,6);        // 10M resistor between pins 4 & 6, pin 6 is sensor pin, add a wire and or foil
//CapacitiveSensor   cs_4_8 = CapacitiveSensor(4,8);        // 10M resistor between pins 4 & 8, pin 8 is sensor pin, add a wire and or foil

void setup()                    
{
   cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);  //Calibrate the sensor... 
   pinMode(led, OUTPUT);
}

void loop()                    
{
    long total1 =  cs_4_2.capacitiveSensor(30);
    if (total1 > 60)
    {
      yes = true;
    }
    else
    {
      yes = false;
    }    
    
    // to toggle the state of state
    if(yes == true && previous  == false && millis() - time>debounce)
    {  
       if(state == LOW){
         state = HIGH;
       }
       else 
         state = LOW;
         time = millis();       
    } 
      digitalWrite(led, state);
      previous = yes;

      Serial.println(millis()-time);
      delay(10);
}
