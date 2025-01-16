#include<DHT.h>
#include<LiquidCrystal_I2C.h>
#include<Wire.h>
#include <Servo.h>

int sensePin = A0;
int ledPower = 4;
 
int Tsampling = 280;
int Tdelta = 40;
int Tsleep = 9680;
 
float outVo=0 ;
float sigVolt=0;
float dustLevel=0 ;
 
float h;
float t;
Servo myservo;
LiquidCrystal_I2C lcd(0x27,16,2);

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN,DHTTYPE);
int i;
int j;
int k;
int m;
int n;
float t1=0.0;
float h1=0.0;
float d1=0.0;
float tavg;
float havg;
float davg;


 
void setup() 
{  
  myservo.write(0);
  dht.begin();
  lcd.begin();
  myservo.attach(9);
  pinMode(ledPower,OUTPUT);
  Serial.begin(9600);
 
}
 
void loop() {

get_temp_hum_dust();
delay(500); 
pump_con();
delay(300); 
}  

void get_temp_hum_dust()
{
  for(m=1;m<=60;m=m+1)
 { 
  digitalWrite(ledPower,LOW); 
  delayMicroseconds(Tsampling);
  outVo = analogRead(sensePin); 
  delayMicroseconds(Tdelta);
  digitalWrite(ledPower,HIGH); 
  delayMicroseconds(Tsleep);
  sigVolt = outVo * (5.0 / 1024.0);
  dustLevel = (170 * sigVolt) - 0.1;
  h = dht.readHumidity();
  t = dht.readTemperature();
 
 h1=h1+h;
 t1=t1+t;
 d1=d1+dustLevel;
 delay(1000);

 Serial.println(m);
Serial.println("   Dust Density Level: ");
Serial.println(dustLevel);
Serial.print("Humidity= ");
Serial.print(h);
Serial.print("Temperature= ");
Serial.print(t);
Serial.println("C");
 delay(500);
 }
 
 havg=h1/60; //taking the average of humidity
 tavg=t1/60; // taking the average of temperature
 davg=d1/60; // taking the average of dust density
 
Serial.println("Humidity avg= ");
Serial.print(havg);
Serial.println("Temperature avg= ");
Serial.print(tavg);
Serial.println("C");
Serial.println("dust avg= ");
Serial.print(davg);

lcd.setCursor(0,0);
lcd.print("T=");
lcd.print(tavg);
lcd.print("C ");
lcd.setCursor(0,1);
lcd.print("H=");
lcd.print(havg);
lcd.print("%");


  lcd.setCursor(10,0);
  lcd.print(" Dust");
  lcd.setCursor(9,1);
  lcd.print(davg);
  t1=0.0;
  h1=0.0;
  d1=0.0;
  Serial.print(t1);
}
void pump_con()
{
  get_temp_hum_dust();
  if(tavg>25.0 || havg>80.0 || davg> 500.0)
  {                  
   myservo.write(0);
  delay(50);
 
  for(n=1;n<=5;n=n+1)
  {
  for(j=0;j<=90;j=j+5)
  {
  myservo.write(j);
  delay(50);  
  }
  delay(300);
  for(i=90;i>=0;i=i-5)
  {
    myservo.write(i);
    delay(50);
  }
  }
  delay(2000);
  
  }
  } 

  
