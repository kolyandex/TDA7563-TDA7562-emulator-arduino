#include <Wire.h>
#define TDA7563_ADDR 0x6C
static uint8_t DB[4] = {0x00, 0x00, 0x00, 0x00};
static uint8_t IB[2] = {0x00, 0x00};
void setup() 
{
  Wire.begin(TDA7563_ADDR);  
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() 
{
  
}

void handleIb()
{
  if(IB[1] & 0x10) //Standby
  {
    digitalWrite(LED_BUILTIN, HIGH);
    DB[2] |= 0x80;
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW); 
    DB[2] &= ~0x80;
  }

  if(IB[0] & 0x20) //Offset detection
  {
    DB[1] |= 0x80;
  }
  else
  {
    DB[1] &= ~0x80;
  }

  if(IB[0] & 0x40) //Diagnostics enable
  {
    DB[2] |= 0x40;
  }
  else
  {
    DB[2] &= ~0x40;
  }

  if(IB[0] & 0x06) // front/rear channels
  {
    DB[0] |= 0x10; //LF
    DB[1] |= 0x10; //LR
    DB[2] |= 0x10; //RF
    DB[3] |= 0x10; //RR
  }
  else
  {
    DB[0] &= ~0x10; //LF
    DB[1] &= ~0x10; //LR
    DB[2] &= ~0x10; //RF
    DB[3] &= ~0x10; //RR
  }  
}

void requestEvent() 
{
  Wire.write(DB, sizeof(DB)); 
}

void receiveEvent(int count) 
{
  if(count == 2)
  {
    IB[0] = Wire.read();
    IB[1] = Wire.read();
    handleIb();
  }
  else
  {
    while (Wire.available()) 
    { 
      volatile char c = Wire.read();
    }
  }  
}
