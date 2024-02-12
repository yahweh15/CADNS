
// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 6, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Temperature sensor pin
const int Temperature_sensor = A0;

// Temperature sensor pin
const int Vibration_sensor = A1;

const int SWITCH = 3; // the cancel switch
volatile bool State = LOW;// state of the interrupt to be updated by the interrupt
volatile bool ButtonIsPressed = LOW;


float Temperature_reading;
float Voltage_reading;

void ContactingEmergencyServices(){
  lcd.setCursor(0,1);
    lcd.print("calling Emergency Services");
    delay(1000);
     lcd.clear();
}

void Normal(){
   lcd.setCursor(0,0);
    lcd.print("Normal");
     delay(1000);
     lcd.clear();
}

// interrupt switch fuction
void switchPressed ()
{
 State = digitalRead(SWITCH);
 if(State == HIGH)
  ButtonIsPressed = HIGH;
  else
  ButtonIsPressed = LOW;
  
}



void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("CADNS");
  //configuring pin A0 as an input
  pinMode(Temperature_sensor,INPUT);
  // configuring pin 3 as input 
   pinMode (SWITCH, INPUT);
   // configuring pin A1 as an input
   pinMode(Vibration_sensor,INPUT);
  attachInterrupt (digitalPinToInterrupt (SWITCH), switchPressed, CHANGE);  // attach interrupt handler

}

void loop() {
  // the vibration Value
int  VibrationReading = analogRead(Vibration_sensor);
  // reading the voltage from the LM35
  Voltage_reading = analogRead(Temperature_sensor);
  // conerting Volatage into degreees celcius
   Temperature_reading = (Voltage_reading*500)/1023;
 
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print("Temp");
  lcd.print(Temperature_reading);
  delay(1000);
  lcd.clear(); 
  
  lcd.print("Vibration");
  lcd.print(VibrationReading);
  delay(1000);
  lcd.clear();
  
  // definate accident
  if(Temperature_reading >= 60||VibrationReading >= 1023 ){
    lcd.setCursor(0,0);
    lcd.print("accident detected");
     delay(1000);
     ContactingEmergencyServices();
  }
// not a definate acciden
  else if ((Temperature_reading >= 50 && Temperature_reading < 60)||(VibrationReading >= 576 && VibrationReading < 1023)){
    lcd.setCursor(0,0);
    lcd.print("Posiible accident");
     delay(1000);
     lcd.clear();
     
     lcd.setCursor(0,0);
     lcd.print("contacting emergency");
     lcd.setCursor(0,1);
     lcd.print("in 4 minutes");
     delay(1000);
     lcd.clear();
     
  // from the interrupt if the button is pressed
     if(ButtonIsPressed == HIGH){
      Normal();
      
     }
     
  //  from the interrupt if the button is NOT pressed  
     else{
      delay(2000);
     ContactingEmergencyServices();
     }
     
      
  }
  
  else{
   Normal();
  }

 
}
