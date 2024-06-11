//transistor with push and pot and joystick 
//connect to arduino nano 

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t pipeOut = 0xF9E8F0F0E1LL;   //IMPORTANT: The same as in the receiver 0xF9E8F0F0E1LL

RF24 radio(8, 9); // select CE,CSN pin

struct PacketData 
{
  byte lxAxisValue;
  byte lyAxisValue;
  byte rxAxisValue;
  byte ryAxisValue;
  byte wxAxisValue;
  byte wyAxisValue;
  byte txAxisValue;
  byte tyAxisValue;

  byte lPotValue;  
  byte rPotValue;    
  byte switch1Value;
  byte switch2Value;
  byte switch3Value; 
};
PacketData data;

void setup()
{
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipeOut);
  radio.stopListening(); //start the radio Transmitter 

  pinMode(6,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);
     
}

int mapAndAdjustJoystickDeadBandValues(int value, bool reverse)
{
  if (value >= 530)
  {
    value = map(value, 530, 1023, 127, 254);
  }
  else if (value <= 500)
  {
    value = map(value, 500, 0, 127, 0);  
  }
  else if (value >= 530)
  {
    value = map(value, 530, 1023, 127, 254);  
  }
   else if (value <= 500)
  {
    value = map(value, 500, 0, 127, 0);  
  }
  else
  {
    value = 127;
  }
  if (reverse)
  {
    value = 254 - value;
  }
  return value;
}

void loop()
{
  data.lxAxisValue    = mapAndAdjustJoystickDeadBandValues(analogRead(A1), false);
  data.lyAxisValue    = mapAndAdjustJoystickDeadBandValues(analogRead(A0), false);
  data.rxAxisValue    = mapAndAdjustJoystickDeadBandValues(analogRead(A3), false);
  data.ryAxisValue    = mapAndAdjustJoystickDeadBandValues(analogRead(A2), false);

  data.wxAxisValue    = mapAndAdjustJoystickDeadBandValues(analogRead(A5), false);
  data.wyAxisValue    = mapAndAdjustJoystickDeadBandValues(analogRead(A4), false);
  data.txAxisValue    = mapAndAdjustJoystickDeadBandValues(analogRead(A7), false);
  data.tyAxisValue    = mapAndAdjustJoystickDeadBandValues(analogRead(A6), false);



  data.lPotValue      = map(analogRead(2), 0, 1023, 0, 254);
  data.rPotValue      = map(analogRead(3), 0, 1023, 0, 254);  
  data.switch1Value   = !digitalRead(6);
  data.switch2Value   = !digitalRead(4);
  data.switch3Value   = !digitalRead(5);
 
        
  radio.write(&data, sizeof(PacketData));
}
