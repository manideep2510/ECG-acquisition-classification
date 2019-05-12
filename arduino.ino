// -------------------------------------- Parameters ------------------------------------
#include <LiquidCrystal.h>


LiquidCrystal lcd(12,11,5,4,3,2);
const int analogInPin = A0;

    int reading = 0;
    int BPM = 0;
    float difference=0;
    float avg;
    unsigned long Ttime=0;
    //bool IgnoreReading = false;
    bool FirstPulseDetected = false;
    unsigned long FirstPulseTime = 0;
    unsigned long SecondPulseTime = 0;
    unsigned long PulseInterval = 0;
    int count=0;
    float maximum=0.0;
    float minimum=400.0;
    unsigned long Stime=0;
    unsigned long Rtime=0;
    float Rvalue;
    float Svalue;
    unsigned long qrstime;
    bool R=true;
    bool S=false;
    bool T=false;
    
    bool print1;
/*int ledPin = 50;
int ledPin2 = 48;
int ledPin3 = 46;
int ledPin4 = 44;
int ledPin5 = 13;*/
 
int beat=0;
float UpperThreshold=210;
float LowerThreshold=205;
bool IgnoreReading = false;
unsigned long end_time;
// Originally collected signal (x, x1 and x2), notch filtered (y, y1, y2 and y3) and notch & low-pass filtered ECG signal (z, z1, z2 and z3).
// x = x(t); x1 = x(t-1); x1 = x(t-2). The same applies for y and z.
volatile double x = 0.0;
volatile double x1 = 0.0;
volatile double x2 = 0.0;

volatile double y = 0.0;
volatile double y1 = 0.0;
volatile double y2 = 0.0;
volatile double y3 = 0.0;

volatile double z = 0.0;
volatile double z1 = 0.0;
volatile double z2 = 0.0;
volatile double z3 = 0.0;

String bpm = "Normal Heart Rate";

double maxi;
double maxi2;
double mini;
double sum;

// Flag variables.
int aux = 0;
int aux2 = 0;
int aux3 = 0;
double auxx;

// Iterative variable.
int ii = 0;
int k = 0;
int l;

// Arrays
double timeVecto2 [512]; // Array which keeps 512 samples from the ECG signal (more or less, one cycle: sampling frequency = 500 Hz)
double timeVectorHigh2 [60]; // Saves 30 different instants (in ms) which corresponds to each R peak in the signal.
double timeVectorHigh3 [60]; // Time duration between 30 different R peaks.
double timeVectorLow2 [60]; // Saves 30 different instants (in ms) which corresponds to each S peak in the signal.
double timeVector2Medium [60]; // Saves 30 different instants (in ms) which corresponds to each T peak in the signal.
double rt [60]; // 30 different RT intervals from the ECG signal.
double rs[60]; // 30 different RS intervals from the ECG signal.
double st[60]; // 30 different ST intervals from the ECG signal.
double rs2;
double st2;
double rsf;
double stf;
double res1;
double res2;
double resf;
double maxx = 0;
double minn = 10000;

// Defining combination of on and off pins to show each number.
const int numeral[10] = {
  B11111100, // 0
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
};

// Arduino pins used to connect all the 7 segments + dots.
//const int segmentPins[] = { 22, 7, 8, 6, 5, 3, 2, 9};

// Number of digits allowed in the display.
//const int numberofDigits = 4;

// Arduino pins used to connect all the 4 digits.
//const int digitPins[numberofDigits] = {10, 11, 12, 4};
// -------------------------------------------------
void setup() {
  // put your setup code here, to run once:
  pinMode(A1, OUTPUT);
  lcd.begin(16,2);
lcd.print("BPM");
Serial.begin(9600);
end_time=millis()+10000;

 /* pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  pinMode(ledPin5, OUTPUT);

  for (int i = 0; i < 8; i++)
    pinMode(segmentPins[i], OUTPUT); //set segment and DP pins to output

  //sets the digit pins as outputs
  for (int i = 0; i < numberofDigits; i++)
    pinMode(digitPins[i], OUTPUT);*/

  cli(); //stop interrupts

  //set timer1 interrupt at 500Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1Hz increments
OCR1A = 3999;// = (16*10^6) / (8*500) - 1 (must be <65536)
  
  //OCR1A = 6666 ;// = (16*10^6) / (8*300) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 bits for 8 prescaler
  TCCR1B |= (1 << CS11);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei(); //allow interrupts

}
ISR(TIMER1_COMPA_vect) { // commands executed at each time interrupt

  // -------------------------------------------- Acquiring Data ------------------------------------------

  x = analogRead(analogInPin);

  // ----------------------------------------------- Filtering --------------------------------------------

  // Filtering around 40 and 60 Hz (notch).
//  y = 0.2012 * x - 0.3256 * x1 + 0.2012 * x2 + 0.3256 * y1 + 0.5975 * y2;

//y = 0.5578 * x - 0.9025 * x1 + 0.5578 * x2 + 0.9025 * y1 - 0.1156 * y2;

 

//Filtering around 49Hz and 51Hz (notch -500Hz).
  y = 0.7161 * x - 1.1587 * x1 + 0.7161 * x2 + 1.1587 * y1 - 0.4323 * y2;

//Filtering around 49Hz and 51Hz (notch -300Hz)
//y = 0.6022 * x - 0.6022 * x1 + 0.6022 * x2 + 0.6022 * y1 - 0.2043 * y2;

  // Low-pass Filter 100 Hz.
  z = 0.0495 * y + 0.1486 * y1 + 0.1486 * y2 + 0.0495 * y3 + 1.1619 * z1 - 0.6959 * z2 + 0.1378 * z3;

  x2 = x1;
  x1 = x;

  y3 = y2;
  y2 = y1;
  y1 = y;

  z3 = z2;
  z2 = z1;
  z1 = z;

  // Saving z value
  timeVecto2[k] = z; 
// timeVecto2[k] = y; 

  k = (k + 1) % 512;
  
}

void loop() {
  // put your main code here, to run repeatedly:
//Serial.print(" ");
float voltage=5 * 100 * z / 1023;
analogWrite(A1,voltage);

/*if(millis()>=end_time){
   lcd.clear();
   lcd.print("BPM");
   lcd.setCursor(0,1);
   lcd.print(beat*6);
   
   beat=0;
   end_time=millis()+10000; //reset the timer
}*/


/*if( voltage > UpperThreshold && R==true)
{
  if(voltage>maximum)
          {
          maximum=voltage;
          }
     else 
     {
      Rtime=millis();
      maximum=0;
      R=false;    
      S=true; 
     }
      
}*/

if(voltage > UpperThreshold && IgnoreReading == false){
        //beat=beat+1;
        
        if(FirstPulseDetected == false){
          FirstPulseTime = millis();
          FirstPulseDetected = true;
          
        }
        else{
          SecondPulseTime = millis();
          PulseInterval = SecondPulseTime - FirstPulseTime;
          FirstPulseTime = SecondPulseTime;
          BPM = (1.0/PulseInterval) * 60.0 * 1000;
          lcd.clear();
          lcd.print("BPM");
          lcd.setCursor(0,1);
          lcd.print(BPM);
         
        }
        IgnoreReading = true;
      }

if(voltage < LowerThreshold){
        IgnoreReading = false;
        
}

/*if(voltage<LowerThreshold && S==true)
{
  if(voltage<minimum)
        {
          minimum=voltage;
          
        }
        else
        {
          Stime=millis();
          Svalue=minimum;
          //lcd.setCursor(5,0);
          //lcd.print(2*(Stime-Rtime));
          minimum=400;
          R=true;
          S=false;
          T=true;
        }
}

if(T==true)
{
  
  if(voltage-Svalue>6)
  
  {
    
    Ttime=millis();
    difference=2*(Ttime-Rtime);
    avg=avg +difference;
    count=count+1;
    if(count==5)
    {
      
    count=0;
    lcd.setCursor(5,0);
    lcd.print(avg/5);
    }
    T=false;
    
  }
  
}

/*if(R==false){
        if(voltage<minimum)
        {
          minimum=voltage;
          Stime=millis();
        }
        else{
          if(print1==true){lcd.setCursor(10,0);
        lcd.print(Stime/1000);
        lcd.setCursor(5,0);
        lcd.print(Rtime/1000);
           print1=false;}
         
          }
      }*/

  //Serial.write((byte)voltage/20);
  //Serial.print(voltage);
  //count=count+1;
  //Serial.print(Svalue);
 // Serial.print(Rvalue);
  Serial.print(voltage/20);
  //Serial.print('\t');
  //Serial.print(millis());
  

  
  //Serial.println();
 // Serial.print(',');
  //Serial.print(millis());
  Serial.println();

  //if (count == 10000) Serial.print('e');
}
