#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include "MeAuriga.h"


//Encoder Motor
MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);
MeEncoderMotor Encoder_3(0x09, SLOT1); 
MeEncoderMotor Encoder_4(0x09, SLOT2);

  MePort port6(PORT_6);
MePort port7(PORT_7);

MeUltrasonicSensor ultrasonic_Front(PORT_9);
MeUltrasonicSensor ultrasonic_Left(PORT_8);
MeUltrasonicSensor ultrasonic_Right(PORT_10);




Servo myservo1;  // create servo object to control a servo 
Servo myservo2;  // create servo object to control a servo 
Servo myservo3;  // create servo object to control a servo 
Servo myservo4;  // create servo object to control another servo

int16_t servo1pin =  port6.pin1();//attaches the servo on PORT_3 SLOT1 to the servo object
int16_t servo2pin =  port7.pin2();//attaches the servo on PORT_3 SLOT1 to the servo object
int16_t servo3pin =  port7.pin1();//attaches the servo on PORT_3 SLOT1 to the servo object
int16_t servo4pin =  port6.pin2();//attaches the servo on PORT_3 SLOT2 to the servo object



//  SetUp encoder motor 1 (NO Tocar)
void isr_process_encoder1(void)
{
      if(digitalRead(Encoder_1.getPortB()) == 0){
            Encoder_1.pulsePosMinus();
      }else{
            Encoder_1.pulsePosPlus();
      }
}
//  SetUp encoder motor 2 (NO Tocar)
void isr_process_encoder2(void)
{
      if(digitalRead(Encoder_2.getPortB()) == 0){
            Encoder_2.pulsePosMinus();
      }else{
            Encoder_2.pulsePosPlus();
      }
}




void setup(){
  Serial.begin(9600);
  myservo1.attach(servo1pin);  // attaches the servo on servopin1
  myservo2.attach(servo2pin);  // attaches the servo on servopin1
  myservo3.attach(servo3pin);  // attaches the servo on servopin1
  myservo4.attach(servo4pin);  // attaches the servo on servopin2

    //Set Pwm 8KHz
    
    TCCR1A = _BV(WGM10);
    TCCR1B = _BV(CS11) | _BV(WGM12);
    TCCR2A = _BV(WGM21) | _BV(WGM20);
    TCCR2B = _BV(CS21);
//    Serial.begin(9600);
    
    attachInterrupt(Encoder_1.getIntNum(), isr_process_encoder1, RISING);
    attachInterrupt(Encoder_2.getIntNum(), isr_process_encoder2, RISING);
    //attachInterrupt(Encoder_1.getIntNum(), isr_process_encoder1, RISING);
    Encoder_1.setPulse(9);
    Encoder_1.setRatio(39.267);
    Encoder_1.setPosPid(1.8,0,1.2);
    Encoder_1.setSpeedPid(0.18,0,0);
    
    attachInterrupt(Encoder_2.getIntNum(), isr_process_encoder2, RISING);
    Encoder_2.setPulse(9);
    Encoder_2.setRatio(39.267);
    Encoder_2.setPosPid(1.8,0,1.2);
    Encoder_2.setSpeedPid(0.18,0,0);

  Encoder_3.begin();
    Encoder_4.begin();
    
    _delay(1);
    
}
void Stop(){
    Encoder_1.runSpeed(0);
    Encoder_2.runSpeed(0);
    Encoder_3.runSpeed(0);
    Encoder_4.runSpeed(0);
    }

void goFront(int vel, int t)
{
  myservo1.write(19);                   
   myservo2.write(137);                    
   myservo3.write(64);                  
   myservo4.write(155);
    _delay(1);
   
  for(int i; i<10; i++){
   int vel1 = (vel/10) * i;
   Encoder_1.runSpeed(vel1 * 1.133);
   Encoder_2.runSpeed(-vel1 * 1.133);
   Encoder_3.runSpeed(vel1);
   Encoder_4.runSpeed(-vel1);
   _delay(.1);
  }
  
   _delay(t-1);
   Stop();
   
}


void goBack(int vel, int t)
{
   myservo1.write(19);                   
   myservo2.write(137);                   
   myservo3.write(64);                  
   myservo4.write(155);
    _delay(1);

   for(int i; i<10; i++){
    int vel1 = (vel/10) * i;
   Encoder_1.runSpeed(-vel1 * 1.133);
   Encoder_2.runSpeed(vel1 * 1.133);
   Encoder_3.runSpeed(-vel1);
   Encoder_4.runSpeed(vel1);
   _delay(.1);
   }
   
   _delay(t-1);
   Stop();
}

void goLeft(int vel, int t)
{
   myservo1.write(117);                   
   myservo2.write(36);                   
   myservo3.write(162);                  
   myservo4.write(60);
    _delay(1);

   for(int i; i<10; i++){
    int vel1 = (vel/10) * i;
   Encoder_1.runSpeed(-vel1 * 1.133);
   Encoder_2.runSpeed(-vel1 * 1.133);
   Encoder_3.runSpeed(-vel1);
   Encoder_4.runSpeed(-vel1);
   _delay(.1);
   }
   _delay(t-1);
   Stop();
}

void goRight(int vel, int t)
{
   myservo1.write(117);                   
   myservo2.write(36);                   
   myservo3.write(162);                  
   myservo4.write(60);
    _delay(1);

   for(int i; i<10; i++){
    int vel1 = (vel/10) * i;
   Encoder_1.runSpeed(vel1 * 1.133);
   Encoder_2.runSpeed(vel1 * 1.133);
   Encoder_3.runSpeed(vel1);
   Encoder_4.runSpeed(vel1);
   _delay(.1);
   }
   _delay(t-1);
   Stop();
}

//void goFront(int velocity, double target)/
//{
  
//}




void goFrontSensor(double target, double limit, double left ){
      

  myservo1.write(19);                   
   myservo2.write(137);                    
   myservo3.write(64);                  
   myservo4.write(155);
    _delay(.5);
    
  double frontSensor = ultrasonic_Front.distanceCm();
  double leftSensor = ultrasonic_Left.distanceCm();
  if ( abs(target-frontSensor) >  20   ) {
      for(int i = 0; i<10; i++){
      int vel1 = (limit/10) * i;
     Encoder_1.runSpeed(vel1 * 1.133);
     Encoder_2.runSpeed(-vel1 * 1.133);
     Encoder_3.runSpeed(vel1);
     Encoder_4.runSpeed(-vel1);
     _delay(.1);
     }
  }
  //Stop();
  // _delay(5000);
  

   
    double Kp=4;
    double Ki=15;
   frontSensor = ultrasonic_Front.distanceCm();

     // Encoder_1.runSpeed(-limit);//52.5
     // Encoder_2.runSpeed(-limit);
     // _delay(0.2);
    while( abs( frontSensor - target) > 0.3  ){
      frontSensor = ultrasonic_Front.distanceCm();
      double Error=(target-frontSensor)*Kp;

      if (Error > limit)
          Error=limit;
      if (Error < (-limit))
          Error = (-limit);

      if (Error > 0)
          Error=Error+Ki;
      if (Error < 0)
          Error = Error-Ki;
    
      //Encoder_1.runSpeed(Error*1.133);//52.5
      //Encoder_2.runSpeed(Error*1.133);
      //Encoder_3.runSpeed(Error);
      //Encoder_4.runSpeed(Error);
   leftSensor = ultrasonic_Left.distanceCm();
  if (leftSensor >left ){
      
   Encoder_1.runSpeed(-Error * 1.133);
   Encoder_2.runSpeed(Error * 1.133)  ;
   Encoder_3.runSpeed(-Error )  ;
   Encoder_4.runSpeed(Error);
  }   else{
    Encoder_1.runSpeed(-Error * 1.133 )  ;
   Encoder_2.runSpeed(Error * 1.133);
   Encoder_3.runSpeed(-Error);
   Encoder_4.runSpeed(Error )  ;   
   }
               
    _delay(0.01);
    }        
  Stop();
  }



void goRightSensor(double target, double limit){
      

   myservo1.write(117);                   
   myservo2.write(36);                   
   myservo3.write(162);                  
   myservo4.write(60);
    _delay(.5);
    
  double rightSensor = ultrasonic_Right.distanceCm();
  if ( abs(target-rightSensor) >  20   ) {
      for(int i = 0; i<10; i++){
      int vel1 = (limit/10) * i;
     Encoder_1.runSpeed(vel1 * 1.133);
     Encoder_2.runSpeed(vel1 * 1.133);
     Encoder_3.runSpeed(vel1);
     Encoder_4.runSpeed(vel1);
     _delay(.1);
     }
  }
  //Stop();
  // _delay(5000);
  

   
    double Kp=4;
    double Ki=15;
   rightSensor = ultrasonic_Right.distanceCm();

     // Encoder_1.runSpeed(-limit);//52.5
     // Encoder_2.runSpeed(-limit);
     // _delay(0.2);
    while( abs( rightSensor - target) > 0.3  ){
      rightSensor = ultrasonic_Right.distanceCm();
      double Error=(target-rightSensor)*Kp;

      if (Error > limit)
          Error=limit;
      if (Error < (-limit))
          Error = (-limit);

      if (Error > 0)
          Error=Error+Ki;
      if (Error < 0)
          Error = Error-Ki;
    
      //Encoder_1.runSpeed(Error*1.133);//52.5
      //Encoder_2.runSpeed(Error*1.133);
      //Encoder_3.runSpeed(Error);
      //Encoder_4.runSpeed(Error);
   Encoder_1.runSpeed(-Error * 1.133);
   Encoder_2.runSpeed(-Error * 1.133);
   Encoder_3.runSpeed(-Error);
   Encoder_4.runSpeed(-Error);
 
  //    Encoder_1.runSpeed(-Error);
   //   Encoder_2.runSpeed(Error);
              

          
           _delay(0.01);
    }        
  Stop();
  }



void goLeftSensor(double target, double limit){
      

   myservo1.write(117);                   
   myservo2.write(36);                   
   myservo3.write(162);                  
   myservo4.write(60);
    _delay(.5);
    
  double leftSensor = ultrasonic_Left.distanceCm();
  if ( abs(target-leftSensor) >  20   ) {
      for(int i = 0; i<10; i++){
      int vel1 = (limit/10) * i;
     Encoder_1.runSpeed(-vel1);
     Encoder_2.runSpeed(-vel1);
          _delay(.1);
     Encoder_3.runSpeed(-vel1);
     Encoder_4.runSpeed(-vel1);

     }
  }
  //Stop();
  // _delay(5000);
  

   
    double Kp=4;
    double Ki=15;
   leftSensor = ultrasonic_Left.distanceCm();

     // Encoder_1.runSpeed(-limit);//52.5
     // Encoder_2.runSpeed(-limit);
     // _delay(0.2);
    while( abs( leftSensor - target) > 0.3  ){
      leftSensor = ultrasonic_Left.distanceCm();
      double Error=(target-leftSensor)*Kp;

      if (Error > limit)
          Error=limit;
      if (Error < (-limit))
          Error = (-limit);

      if (Error > 0)
          Error=Error+Ki;
      if (Error < 0)
          Error = Error-Ki;
    
      //Encoder_1.runSpeed(Error*1.133);//52.5
      //Encoder_2.runSpeed(Error*1.133);
      //Encoder_3.runSpeed(Error);
      //Encoder_4.runSpeed(Error);
   Encoder_1.runSpeed(Error * 1.133);
   Encoder_2.runSpeed(Error * 1.133);
   Encoder_3.runSpeed(Error);
   Encoder_4.runSpeed(Error);
 
  //    Encoder_1.runSpeed(-Error);
   //   Encoder_2.runSpeed(Error);
              

          
           _delay(0.01);
    }        
  Stop();
  }






void loop(){
  //Serial.print("Distance : ");
  //Serial.println(ultrasonic_Left.distanceCm());    
  //Serial.println(ultrasonic_Right.distanceCm());
  //Serial.println(ultrasonic_Front.distanceCm());

  //Serial.println(" cm");
  //delay(100); /* the minimal measure interval is 100 milliseconds */
    goLeft(60,3);
        Stop();
    _delay(0.5);
    goLeftSensor(30,80);
    Stop();
    _delay(0.5);
    goFrontSensor(20,80,30);
    Stop();
    _delay(0.5);
    goFrontSensor(100,80,30);
        Stop();
    _delay(0.5);
    goLeftSensor(50,80);
        Stop();
    _delay(0.5);
    goFrontSensor(20,80,50);
        Stop();
    _delay(0.5);
    goFrontSensor(100,80,50);
        Stop();
    _delay(0.5);
//    goLeftSensor(70,80);

  //goLeft(60, 60);
  
//  goFront(80, 6);
//  goRight(80, 6);
//  goBack(80, 6);
//  goLeft(80, 6);
//    
 
    _loop();

}


void _delay(float seconds){
    long endTime = millis() + seconds * 1000;
    while(millis() < endTime)_loop();
}

void _loop(){
    Encoder_1.loop();
    Encoder_2.loop();
}



/*
 *     rgbled_0.setpin(44);
    rgbled_0.setColor(0,0,0,0);
    rgbled_0.show();

 */
