#include <Servo.h>

//  if ( Serial.available()) {
//    char ch = Serial.read();
//    
//    Serial.print(ch);
    servo1.write(0);
    delay(5000);
    servo1.write(90);
    delay(5000);
    servo1.write(179);
    delay(5000);
    servo1.write(90);
    delay(5000);
    
    
    
//    switch(ch) {
//      case '0'...'9':
//        v = v * 10 + ch - '0';
//        Serial.println(v);
//        break;
//      case 's':
//        servo1.write(v);
//        v = 0;
//        break;
//    }
//  }
} 
