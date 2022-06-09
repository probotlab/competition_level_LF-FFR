#include <math.h>
const int thermistor1_output = A5; // right from back
const int thermistor2_output = 7; // left from back

#include <AFMotor.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

#define RM_S A0
#define R_S A1
#define M_S A2
#define L_S A3
#define LM_S A4

int lms,ls,ms,rs,rms ;
int check=0;

void setup() {
  Serial.begin(9600);  /* Define baud rate for serial communication */

  pinMode(R_S, INPUT);
  pinMode(M_S, INPUT);
  pinMode(L_S, INPUT);
  pinMode(RM_S, INPUT);
  pinMode(LM_S, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);
 
}

void loop(){
  
  Serial.print(lms);
  Serial.print(ls);
  Serial.print(ms);
  Serial.print(rs);
  Serial.println(rms);
  
firefighting();
readsensors();
}

void firefighting(){


//void right_thermistor
  int thermistor1_adc_val;
  double output1_voltage, thermistor1_resistance, therm1_res_ln, temperature_right; 
  thermistor1_adc_val = analogRead(thermistor1_output);
  output1_voltage = ( (thermistor1_adc_val * 5.0) / 1023.0 );
  thermistor1_resistance = ( ( 5 * ( 10.0 / output1_voltage ) ) - 10 ); /* Resistance in kilo ohms */
  thermistor1_resistance = thermistor1_resistance * 1000 ; /* Resistance in ohms   */
  therm1_res_ln = log(thermistor1_resistance);
  /*  Steinhart-Hart Thermistor Equation: */
  /*  temperature_right in Kelvin = 1 / (A + B[ln(R)] + C[ln(R)]^3)   */
  /*  where A = 0.001129148, B = 0.000234125 and C = 8.76741*10^-8  */
  temperature_right = ( 1 / ( 0.001129148 + ( 0.000234125 * therm1_res_ln ) + ( 0.0000000876741 * therm1_res_ln * therm1_res_ln * therm1_res_ln ) ) ); /* temperature_right in Kelvin */
  temperature_right = temperature_right - 273.15; /* temperature_right in degree Celsius */
  Serial.print("temperature_right in degree Celsius = ");
  Serial.print(temperature_right);
  Serial.print("\t\t");
  Serial.print("Resistance in ohms = ");
  Serial.print(thermistor1_resistance);
  Serial.print("\n\n");
/////////////////////////////////////////////////////////////////


//void left_thermistor

  int thermistor2_adc_val;
  double output2_voltage, thermistor2_resistance, therm2_res_ln, temperature_left; 
  thermistor2_adc_val = analogRead(thermistor2_output);
  output2_voltage = ( (thermistor2_adc_val * 5.0) / 1023.0 );
  thermistor2_resistance = ( ( 5 * ( 10.0 / output2_voltage ) ) - 10 ); /* Resistance in kilo ohms */
  thermistor2_resistance = thermistor2_resistance * 1000 ; /* Resistance in ohms   */
  therm2_res_ln = log(thermistor2_resistance);
  /*  Steinhart-Hart Thermistor Equation: */
  /*  temperature_left in Kelvin = 1 / (A + B[ln(R)] + C[ln(R)]^3)   */
  /*  where A = 0.001129148, B = 0.000234125 and C = 8.76741*10^-8  */
  temperature_left = ( 1 / ( 0.001129148 + ( 0.000234125 * therm2_res_ln ) + ( 0.0000000876741 * therm2_res_ln * therm2_res_ln * therm2_res_ln ) ) ); /* temperature_left in Kelvin */
  temperature_left = temperature_left - 273.15; /* temperature_left in degree Celsius */
  Serial.print("temperature_left in degree Celsius = ");
  Serial.print(temperature_left);
  Serial.print("\t\t");
  Serial.print("Resistance in ohms = ");
  Serial.print(thermistor2_resistance);
  Serial.print("\n\n");
////////////////////////////////////////////////////////////


if (temperature_left > 45 ){

  stop();
  motor2.setSpeed(255);
  motor2.run(FORWARD);
   
   }
else{
   readsensors();
   followline();
 }
  
if (temperature_right > 45 ){
   stop();
   motor1.setSpeed(255);
   motor1.run(BACKWARD);
   }
  else{
    
   readsensors();
   followline();
  }

if (temperature_right > 45 && temperature_left > 45 ){
  stop();
  motor1.setSpeed(255);
  motor1.run(BACKWARD);
  motor2.setSpeed(255);
  motor2.run(FORWARD);
}
else{
  readsensors();
  followline();
}
}

void followline()
{

if (lms==1 && ls==0  && ms==0 && rs==0 && rms==1)                                     
{
 forward();
}
//else if (lms==0 && ls==0  && ms==0 && rs==0 && rms==0)                                     
//{
// forward();
//}
else if (lms==1 && ls==1  && ms==0 && rs==1 && rms==1)                                     
{
 // check=0;
forward();
}
else if (lms==1 && ls==1  && ms==0 && rs==0 && rms==1)                                     
{//check=0;
turnRight();
}
else if (lms==1 && ls==1  && ms==1 && rs==0 && rms==1)                                     
{//check=0;
turnRight();
}

else if (lms==1 && ls==1  && ms==1 && rs==0 && rms==0)                                    
{//check=2;
turnRight();
}
else if (lms==1 && ls==1  && ms==1 && rs==1 && rms==0)                                      
{//check=2;
midturnRight();
}
else if (lms==1 && ls==0  && ms==0 && rs==0 && rms==0)                                     
{//check=2;
turnRight();
}

else if (lms==1 && ls==0  && ms==0 && rs==1 && rms==1)                                      
{//check=0;
turnLeft();
}
else if (lms==1 && ls==0  && ms==1 && rs==1 && rms==1)                                      
{
  //check=0;
turnLeft();
}
else if (lms==0 && ls==0  && ms==1 && rs==1 && rms==1)                                      
{//check=1;
turnLeft();
}
else if (lms==0 && ls==0  && ms==0 && rs==0 && rms==0)                                      
{//check=1;
stop();
}

  else if (lms == 1 && ls == 1  && ms == 1 && rs == 1 && rms == 1)                            
  {
    if(check==1)
   {
   // readsensors();
        sharpturnLeft();
    
  }
  if(check==2)
  {
    //readsensors(); 
        sharpturnRight();
    
  }
  

  }
}

void readsensors(){
  lms = digitalRead(LM_S);
  ls = digitalRead(L_S);
  ms = digitalRead(M_S);
  rs = digitalRead(R_S);
  rms = digitalRead(RM_S);


if(lms==0)
{
  check=1;
}
else if(rms==0)
{
  check=2;
}
  
    
}


void forward(){

  motor1.setSpeed(0);
  motor2.setSpeed(0);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  
  motor3.setSpeed(110);
  motor4.setSpeed(110);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
    
} 

void turnRight(){

  motor3.setSpeed(160);
  motor4.setSpeed(0);
  motor3.run(BACKWARD);
  motor4.run(RELEASE);

  }


void sharpturnRight(){
  
  motor3.setSpeed(170);
  motor4.setSpeed(170);

   motor3.run(BACKWARD);
   motor4.run(FORWARD);
   
  }
  void midturnRight(){

  motor3.setSpeed(255);
  motor4.setSpeed(255);
  motor3.run(BACKWARD);
  motor4.run(FORWARD);
  

  }

void turnLeft(){

  motor3.setSpeed(0);
  motor4.setSpeed(160);
  motor4.run(BACKWARD);
  motor3.run(RELEASE);

}
void sharpturnLeft(){
  
  motor3.setSpeed(170);
  motor4.setSpeed(170);

  motor3.run(FORWARD);
  motor4.run(BACKWARD);
  
}

void midturnLeft(){

  motor3.setSpeed(255);
  motor4.setSpeed(255);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);

}

void stop(){

  motor3.setSpeed(0);
  motor4.setSpeed(0);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  delay(1000);
  
}
