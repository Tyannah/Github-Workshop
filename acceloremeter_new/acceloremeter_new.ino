 #include <SPI.h> 
 #include <SoftwareSerial.h>
SoftwareSerial bluetooth(2, 3);
 
 
#include<Wire.h>
# define accel_module (0x53)// The ADXL345 sensor I2C address
byte values[6];
//byte addresses[][6] = {"0"};
void setup()
{
  
  bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600);  // Start bluetooth serial at 9600
  //BTserial.begin(38400);
  Serial.begin(9600);
  delay(1000);
  Wire.begin();
  Wire.beginTransmission(accel_module);// Start communicating with the device 
  Wire.write(0x2D);// Access POWER_CTL Register - 0x2D
  Wire.write(0);
  Wire.endTransmission();
  Wire.beginTransmission(accel_module);
  Wire.write(0x2D);
  Wire.write(16);
  Wire.endTransmission();
  Wire.beginTransmission(accel_module);
  Wire.write(0x2D);
  Wire.write(8); // Enable measurement
  Wire.endTransmission();}


int get_a()
{
  int xyzregister=0x32;
  int x,y,z;
  
  Wire.beginTransmission(accel_module);
  Wire.write(xyzregister);
  Wire.endTransmission();

  Wire.beginTransmission(accel_module);
  Wire.requestFrom(accel_module,6);

  int i=0;
  while(Wire.available()){
    values[i]=Wire.read(); //receive a byte
    i++;
  }

  Wire.endTransmission();
  x=(((int)values[1])<<8) | values[0];
  y=(((int)values[3])<<8) | values[2];
  z=(((int)values[5])<<8) | values[4];
  
  if(x<50  && x>-50)
  {
if(y>30) { x=0; y=1;}
  else if(y<-30) { x=1; y=0;}
  else {x=-1; y=-1;}
  }
  else
  if(x<-10) { x=0;y=0;}
  else if(x>10) { x=1;y=1;}
  return x; 
}


int get_b()
{
   
int xyzregister=0x32;
  int x,y,z;
  
  Wire.beginTransmission(accel_module);
  Wire.write(xyzregister);
  Wire.endTransmission();

  Wire.beginTransmission(accel_module);
  Wire.requestFrom(accel_module,6);//read from 6 registers

  int i=0;
  while(Wire.available()){
    values[i]=Wire.read();
    i++;
  }

  Wire.endTransmission();
  x=(((int)values[1])<<8) | values[0];
  y=(((int)values[3])<<8) | values[2];
  z=(((int)values[5])<<8) | values[4];
  if(x<50  && x>-50)
  {
if(y>30) { x=0; y=1;}
  else if(y<-30) { x=1; y=0;}
  else {x=-1; y=-1;}
  }
  else
  if(x<-10) { x=0;y=0;}
  else if(x>10) { x=1;y=1;}
  return y; 
}

 
void loop()
{
  int s,j;
  s=get_a();// x- axis
  j=get_b();//y-axis
  
  Serial.print("S=");Serial.println(s);
  Serial.print("j=");Serial.print(j);
  if( s==0 && j==0)
  {
      Serial.println("FORWARD");
      bluetooth.write('1');
  }
  if( s==-1 && j==-1)
  {
     Serial.println("STOP");
     bluetooth.write('0');  
  }
  if( s==1 && j==1)
  {
     Serial.println("BACKWARD");
     bluetooth.write('2');
  }
if( s==0 && j==1)
  {
       Serial.println("RIGHT");
       bluetooth.write('3');
  } 
  if( s==1 && j==0)
  {
       Serial.println("LEFT");
       bluetooth.write('4');
  }
 delay(100);
  
}
