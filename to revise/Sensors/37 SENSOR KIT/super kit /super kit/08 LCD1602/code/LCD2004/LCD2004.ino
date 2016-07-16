// include the library code
//www.RobotLinking.com
#include <LiquidCrystal.h>
/**********************************************************/
char array1[]=" SUNFOUNDER               ";  //the string to print on the LCD
char array2[]="hello, world!             ";  //the string to print on the LCD
int tim = 250;  //the value of delay time
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(4, 6, 10, 11, 12, 13);
/*********************************************************/
void setup()
{
  lcd.begin(20, 4);  // set up the LCD's number of columns and rows:
  lcd.setCursor(0,0); 
  lcd.print("hello");
  lcd.setCursor(0,1);
  lcd.print("world");
  lcd.setCursor(0,2);
  lcd.print("ok");
  lcd.setCursor(0,3);
  lcd.print("test");
}
/*********************************************************/
void loop() 
{
    while(1);
    lcd.setCursor(15,0);  // set the cursor to column 15, line 0
    for (int positionCounter1 = 0; positionCounter1 < 26; positionCounter1++)
    {
      lcd.scrollDisplayLeft();  //Scrolls the contents of the display one space to the left.
      lcd.print(array1[positionCounter1]);  // Print a message to the LCD.
      delay(tim);  //wait for 250 microseconds
    }
    lcd.clear();  //Clears the LCD screen and positions the cursor in the upper-left corner.
    lcd.setCursor(15,1);  // set the cursor to column 15, line 1
    for (int positionCounter2 = 0; positionCounter2 < 26; positionCounter2++)
    {
      lcd.scrollDisplayLeft();  //Scrolls the contents of the display one space to the left.
      lcd.print(array2[positionCounter2]);  // Print a message to the LCD.
      delay(tim);  //wait for 250 microseconds
    }
    lcd.clear();  //Clears the LCD screen and positions the cursor in the upper-left corner.
}
/************************************************************/
