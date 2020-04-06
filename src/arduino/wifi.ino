#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <Servo.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Nu5722WMn9Qp7QTZT6Pfo4w3oUsJzAFK";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "505";
char pass[] = "45678910";
Servo servo;

// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin V1
BLYNK_WRITE(V5) // timer
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  //Serial.println(pinValue);
  if(pinValue == true)
  {
    servo.write(180);
    //Serial.println("timer 0n");
    
  }
  else
  {
    servo.write(0);
    //Serial.println("timer off");
  }
  // process received value
}
BLYNK_WRITE(V3) // timer
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  //Serial.println(pinValue);
  if(pinValue == true)
  {
    servo.write(180);
    //Serial.println("timer 0n");
    
  }
  else
  {
    servo.write(0);
    //Serial.println("timer off");
  }
  // process received value
}
BLYNK_WRITE(V1) // Cho an
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  //Serial.println(pinValue);
  if(pinValue == true)
  {
    servo.write(180);
    //Serial.println("Cho ca an");
  }
  else
  {
    //Serial.println("Khong cho ca an");
    servo.write(0);
  }
  // process received value
}
int fan;
BLYNK_WRITE(V2)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  //Serial.println(pinValue);
  if(pinValue == true)
  {
    fan = 1;
    Serial.print(fan);
  }
  else
  {
    fan = 2;
    Serial.print(fan);
  }
  // process received value
}
int light;
BLYNK_WRITE(V6)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  //Serial.println(pinValue);
  // process received value
  if(pinValue == true)
  {
    light = 3;
    Serial.print(light);
  }
  else
  {
    light = 4;
    Serial.print(light);
  }
}
String s;
BLYNK_WRITE(V0) // slider
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  switch(pinValue)
            {            
                case 23:
                {
                    s = "5";
                    break;
                }
                case 24:
                {
                    s = "6";
                    break;
                }
                case 25:
                {
                    s = "7";
                    break;
                }
                case 26:
                {
                    s = "8";
                    break;
                }
                case 27:
                {
                    s = "9";
                    break;
                }
                case 28:
                {
                    s = "q";
                    break;
                }
                case 29:
                {
                    s = "w";
                    break;
                }
                case 30:
                {
                    s = "e";
                    break;
                }
                case 31:
                {
                    s = "r";
                    break;
                }
                default:
                {
                    break;
                }
            }
            Serial.println(s);
}
volatile int interruptCounter;
int totalInterruptCounter;
 
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
 
}
void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // timer
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 500000, true);
  timerAlarmEnable(timer);

  servo.attach(13);

}
int inByte = 0;         // incoming serial byte
String inString = "";    // string to hold input
void loop()
{
  Blynk.run();
  if (Serial.available() > 0) {
    inByte = Serial.read();
    if (isDigit(inByte)) {
      inString += (char)inByte;
    }
    if (inByte == '\n') {
      Blynk.virtualWrite(V4, (float)inString.toInt()/10);
      inString = "";
    }
    
  }
  if (interruptCounter > 0) {
 
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);

    if(totalInterruptCounter >=3) totalInterruptCounter = 0; 
    switch(totalInterruptCounter)
    {
      case 0: {Serial.println(s); break;}
      case 1: {Serial.println(fan); break;}
      case 2: {Serial.println(light); break;}
      default: break;
    }
  totalInterruptCounter++;
  }
}
