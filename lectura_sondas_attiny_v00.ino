
#include <SoftwareSerial.h>
#include <TinyWireM.h>

SoftwareSerial serial_debug(4,3); 

#define DEBUG 1

int I2c_direcctions[] = {21,22,23,24};
float I2c_temperature[] = {0.0,0.0,0.0,0.0};
float I2c_humidity[] = {0.0,0.0,0.0,0.0};

void setup(){
  delay(5);
  TinyWireM.begin();

#ifdef DEBUG
    serial_debug.begin(9600);  // start serial for output
    serial_debug.println("Amphenol Advanced Sensors");
    serial_debug.println("Arduino T9602 Monitor");
#endif

}


void getdata(int dir, byte *a, byte *b, byte *c, byte *d)
{
  delay(80);  
  
  TinyWireM.beginTransmission(dir);
  TinyWireM.write(0);
  TinyWireM.endTransmission();
  
  TinyWireM.requestFrom(dir, 4);
  *a = TinyWireM.read();
  *b = TinyWireM.read();
  *c = TinyWireM.read();
  *d = TinyWireM.read();
}

 void load_data()
{
  for (int n= 0; n<4; n++){
    byte aa,bb,cc,dd;
    float temperature=0.0;  float humidity=0.0;
    getdata(I2c_direcctions[n],&aa,&bb,&cc,&dd);
    humidity = (float)(((aa & 0x3F ) << 8) + bb) / 16384.0 * 100.0;
    temperature = (float)((unsigned)(cc  * 64) + (unsigned)(dd >> 2 )) / 16384.0 * 165.0 - 40.0; 
    I2c_humidity[n] = humidity;
    I2c_temperature[n] = temperature;
    
#ifdef DEBUG
      serial_debug.print("sensor: ");serial_debug.print(I2c_direcctions[n]);serial_debug.print(" --> ");
      serial_debug.print(temperature);serial_debug.print(" degC  ");serial_debug.print(humidity);serial_debug.println(" %rH");; 
#endif

  }
  delay(1000);
}

void loop(){
  load_data();
  delay(3000);
}
