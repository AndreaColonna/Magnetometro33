/*
 Misura le componenti del campo mg terrestre in un punto
 B_x, B_y, B_z calcola il valor medio in 5 sec
 Calcola il valore del campo magnetico sottraendogli il campo magnetico terrestre 
*/
#include <phyphoxBle.h>
char board_name[] = "Mobile";  /*Nome della scheda da richiamare sul file phyphox*/

#include <Arduino_LSM9DS1.h>
float B_x = 0.0; /*variabile per memorizzare il campo magnetico medio lungo x*/ 
float B_y = 0.0; /*variabile per memorizzare il campo magnetico medio lungo y*/
float B_z = 0.0; /*variabile per memorizzare il campo magnetico medio lungo z*/
float B = 0.0; /*modulo del campo magnetico*/
unsigned long initial_time, first_time;
float B_M = 0.0;
int N = 0;     /*Numero di campioni*/
float first_difference_float;


void setup() {

pinMode(LED_BUILTIN, OUTPUT); /*inizializza il led sulla scheda*/
/*  Serial.begin(9600); */
  
/*  while (!Serial);
  Serial.println("Started");*/

  if (!IMU.begin()) {
    /*Serial.println("Failed to initialize IMU!");*/
    digitalWrite(LED_BUILTIN, HIGH); /*Nel acso di fallimento accendi il led*/
    while (1);
  }
  
  /*Serial.print("Magnetic field sample rate = ");
  
  Serial.print(IMU.magneticFieldSampleRate());
  
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Magnetic Field in uT");
  Serial.println("X-BX\tY-BY\tZ-Bz\tB");
  */
  PhyphoxBLE::start(board_name);                /*Start the BLE server*/
 
  float ti = millis(); /*tempo iniziale in millisecondi*/
  float x, y, z;      /*variabili in cui sono memorizzate le componenti del campo magnetico*/
  while((millis()-ti)<5000){
    IMU.readMagneticField(x, y, z); /*lettura delle componenti del campo magnetico*/
    B_x+=x;                         /*somma i valori del campo magnetico*/
    B_y+=y;
    B_z+=z;
    N+=1;                           /*incrementa il numero di valori sommati*/
    }
    B_x=B_x/N;                      /*calcola le componenti del campo magnetico medio*/
    B_y=B_y/N;
    B_z=B_z/N;
    B_M=sqrt(B_x*B_x+B_y*B_y+B_z*B_z); /*calcolo del campo magnetico medio*/
    /*PhyphoxBLE::write(B_M);*/
    initial_time = millis();
}

void loop() {
  
  float x, y, z;            /*variabili in cui sono memorizzate le componenti del campo magnetico*/   
   
  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(x, y, z);  /*lettura delle componenti del campo magnetico*/
    first_time = millis();
    first_difference_float = ((float)first_time - (float)initial_time) / 1000;
    x=x-B_x;                         
    y=y-B_y;
    z=z-B_z;
    B=sqrt(x*x+y*y+z*z);
    /*Serial.print(x);
    Serial.print('\t');
    Serial.print(y);
    Serial.print('\t');
    Serial.println(z);
    Serial.print('\t');
    Serial.print(B);
    */
    
    PhyphoxBLE::write(x,y,z,B);
  }
}
