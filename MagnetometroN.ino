/*
  Arduino LSM9DS1 - Misura del campo magnetico 

  In questo esempio utilizziamo il sensore presente in nano 33 ble sense
  per misurare il campo ambientale e sottrarlo a quello prodotto da un filo percorso da corrente

  The circuit:
  - Arduino Nano 33 BLE Sense
Andrea Colonna

  This example code is in the public domain.
*/

#include <Arduino_LSM9DS1.h>
float B_x=0.0;
float B_y=0.0;
float B_z=0.0;
int N = 0;
void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  Serial.print("Magnetic field sample rate = ");
  Serial.print(IMU.magneticFieldSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Magnetic Field in uT");
  Serial.println("X\tY\tZ");
  float ti = millis();
  /*
  Procedura per calcolare il valor medio del campo magnetico di un luogo
  */
  
  while((millis()-ti)<5000){
    IMU.readMagneticField(B_x, B_y, B_z);
    B_x+=B_x;
    B_y+=B_y;
    B_z+=B_z;
    N+=1;
    }
    B_x=B_x/N;
    B_y=B_y/N;
    B_z=B_z/N;
}

void loop() {
  float x, y, z;

  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(x, y, z);

    Serial.print(x-B_x);
    Serial.print('\t');
    Serial.print(y-B_y);
    Serial.print('\t');
    Serial.println(z-B_z);
  }
}
