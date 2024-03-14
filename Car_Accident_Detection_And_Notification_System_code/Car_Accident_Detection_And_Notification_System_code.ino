
// include the library code:
#include <stdio.h>
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 6, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

 const int  X_PIN = A2;
 const int  Y_PIN = A3;
 const int  Z_PIN = A4;

// the array to store recent and curerent acceleration value readings
   float Recent_ax[2]= {0,0}; // array of size 2 for Recent_ax
   float Recent_ay[2]= {0,0};// array of size 2 for Recent_ay
   float Recent_az[2]= {0,0};// array of size 2 for Recent_az
   int i = 0.; // an iterator for the recent value arrays

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
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:

 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("CADNS");
  
  //configuring pin A0 as an input for the temerature sensor
  pinMode(Temperature_sensor,INPUT);
  
  // configuring pin 3 as input for the delay switch
   pinMode (SWITCH, INPUT);
   
   // configuring pin A1 as an input for the vibration sensor
   pinMode(Vibration_sensor,INPUT);

    // attach interrupt handler for the delay switch
  attachInterrupt (digitalPinToInterrupt (SWITCH), switchPressed, CHANGE); 
  
  // Accelerometer pins
  pinMode(X_PIN, INPUT);
  pinMode(Y_PIN, INPUT);
  pinMode(Z_PIN, INPUT);
}

void loop() {
  // Reading the vibration Value
int  VibrationReading = analogRead(Vibration_sensor);

// Reading the voltage from the LM35
Voltage_reading = analogRead(Temperature_sensor);
// converting Volatage into degreees celcius
 Temperature_reading = (Voltage_reading*500)/1023;

// Reading the X,Y,Z values from the three potentiometers
  int x = analogRead(X_PIN);
  int y = analogRead(Y_PIN);
  int z = analogRead(Z_PIN);

//Calculating the current acceleration value reading
float ax = ((float)x / 1023 * 90)- 45; // map this to -45 to 45 m/s^2
float ay = ((float)y / 1023) * 90 - 45; // map this to -45 to 45 m/s^2
float az = ((float)z / 1023) * 90 - 45; // map this to -45 to 45 m/s^2

  
// storing the current acceleration values as recent in the recent array at index i
   Recent_ax[i] = ax;
   Recent_ay[i] = ay;
   Recent_az[i] = az;
  i = (i+1) % 2; // making i to be between 0 and one;

  // calculating the difference in accelation every second between current and recent
   float difference_ax =abs(Recent_ax[1] - Recent_ax[0]); 
   float difference_ay =abs(Recent_ay[1] - Recent_ay[0]); 
   float difference_az =abs(Recent_az[1] - Recent_az[0]); 

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // printing the Temperature reading
  lcd.print("Temp");
  lcd.print(Temperature_reading);
  delay(1000);
  lcd.clear(); 

  // printing the vibration reading
  lcd.print("Vibration");
  lcd.print(VibrationReading);
  delay(1000);
  lcd.clear();
  
  // printing the X axix acceleration reading reading
   lcd.print("Acel in X: ");
  lcd.print(ax);
  lcd.setCursor(0, 1);
  lcd.print("Burst in X: ");
  lcd.print(difference_ax);
  delay(1000);
  lcd.clear();

  // printing the Y axix acceleration reading
   lcd.print("Acel in Y: ");
  lcd.print(ay);
  lcd.setCursor(0, 1);
  lcd.print("Burst in Y: ");
  lcd.print(difference_ay);
  delay(1000);
  lcd.clear();

  // printing the Z axix acceleration reading
   lcd.print("Acel in Z: ");
  lcd.print(az);
  lcd.setCursor(0, 1);
  lcd.print("Burst in z: ");
  lcd.print(difference_az);
  delay(1000);
  lcd.clear();

//   Serial.println("Acceleration in X: ");
//  Serial.println(ax);
//  Serial.println("Acceleration in Y: ");
//  Serial.println(ay);
//  Serial.println("Acceleration in Z: ");
//  Serial.println(az);
//  Serial.println("Burst in X = ");
//  Serial.println(difference_ax);
//  Serial.println("Burst in Y = ");
//  Serial.println(difference_ay);
//  Serial.println("Burst in Z = ");
//  Serial.println(difference_az);
 Serial.println(Temperature_reading);
   
 

  delay(2000); // wait for a second
  
  // definate accident
  if(Temperature_reading >= 60||VibrationReading >= 1023 ||difference_ax >= 10 || difference_ay >= 10 || difference_az >= 10 ){
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
