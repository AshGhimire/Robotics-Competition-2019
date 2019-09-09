#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include "MeAuriga.h"


//Encoder Motor
MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);
//MeUltrasonicSensor ultraSensor(PORT_6); 

//MeUltrasonicSensor ultrasonic_front(9);
//double frontSensor;

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

    //Set Pwm 8KHz
    TCCR1A = _BV(WGM10);
    TCCR1B = _BV(CS11) | _BV(WGM12);
    TCCR2A = _BV(WGM21) | _BV(WGM20);
    TCCR2B = _BV(CS21);
    Serial.begin(9600);
    
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

    
    _delay(1);
    
}

void goFrontBlind(int x)
{
    Encoder_1.runSpeed(-x);
    Encoder_2.runSpeed(x);
}

void loop(){
    //frontSensor=ultrasonic_front.distanceCm();
    //First Line
    Encoder_1.runSpeed(-60);
    Encoder_2.runSpeed(60);
    _delay(6);

    //First Stop and Right Turn
    Encoder_1.runSpeed(0);
    Encoder_2.runSpeed(0);
    _delay(1);
    
    Encoder_1.runSpeed(-60);
    Encoder_2.runSpeed(95);
    _delay(4.2);

    Encoder_1.runSpeed(0);
    Encoder_2.runSpeed(0);
    _delay(1);

     Encoder_1.runSpeed(-60);
    Encoder_2.runSpeed(60);
    _delay(5.5);

    //First Stop and Right Turn
    Encoder_1.runSpeed(0);
    Encoder_2.runSpeed(0);
    _delay(1);
    
    Encoder_1.runSpeed(-60);
    Encoder_2.runSpeed(100);
    _delay(4.0);

    Encoder_1.runSpeed(0);
    Encoder_2.runSpeed(0);
    _delay(1);
    



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
