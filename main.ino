#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"



#define STEP 4
#define DIR 7
#define EN 8

// clockwise == LOW (0), counter clockwise == HIGH (1)

// change
int degree = 100;
int number_of_steps_between = 1;
int delay_step = 50;
int dir = 0;
// change


int i = 0;
int max_steps = round(degree / 0.1125);


int stav = 0;

// 0 = mereni, 1 = vratit zpatky, 2 = hotovo

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);

void displaySensorDetails(void)
{
  sensor_t sensor;
  tsl.getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.print  (F("Sensor:       ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:   ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:    ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:    ")); Serial.print(sensor.max_value); Serial.println(F(" lux"));
  Serial.print  (F("Min Value:    ")); Serial.print(sensor.min_value); Serial.println(F(" lux"));
  Serial.print  (F("Resolution:   ")); Serial.print(sensor.resolution, 4); Serial.println(F(" lux"));  
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
  delay(500);
}

void configureSensor(void)
{
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  //tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
  //tsl.setGain(TSL2591_GAIN_MED);      // 25x gain
  tsl.setGain(TSL2591_GAIN_HIGH);   // 428x gain //
  //tsl.setGain(TSL2591_GAIN_MAX);   // 9800x gain //
  
  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)

  /* Display the gain and integration time for reference sake */  
  Serial.println(F("------------------------------------"));
  Serial.print  (F("Gain:         "));
  tsl2591Gain_t gain = tsl.getGain();
  switch(gain)
  {
    case TSL2591_GAIN_LOW:
      Serial.println(F("1x (Low)"));
      break;
    case TSL2591_GAIN_MED:
      Serial.println(F("25x (Medium)"));
      break;
    case TSL2591_GAIN_HIGH:
      Serial.println(F("428x (High)"));
      break;
    case TSL2591_GAIN_MAX:
      Serial.println(F("9876x (Max)"));
      break;
  }
  Serial.print  (F("Timing:       "));
  Serial.print((tsl.getTiming() + 1) * 100, DEC); 
  Serial.println(F(" ms"));
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
}

void setup() {
  Serial.begin(9600);
  
  Serial.println(F("Starting Adafruit TSL2591 Test!"));
  
  if (tsl.begin()) 
  {
    Serial.println(F("Found a TSL2591 sensor"));
  } 
  else 
  {
    Serial.println(F("No sensor found ... check your wiring?"));
    while (1);
  }
    
  /* Display some basic information on this sensor */
  displaySensorDetails();
  
  /* Configure the sensor */
  configureSensor();

  Serial.println("Start steps");
  while (Serial.available() == 0) { // wait for serial port to connect. Needed for native USB
    ;
  }
  Serial.println("Start...");
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(EN, OUTPUT);

  if (dir == 0) {
  digitalWrite(DIR, LOW);
  } else if (dir == 1) {
    digitalWrite(DIR, HIGH);
  }
  digitalWrite(EN, LOW);

}

void advancedRead(void)
{
  // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
  // That way you can do whatever math and comparisons you want!
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  Serial.print("measure "); Serial.print(i * 0.1125); Serial.print(F(" "));       //degree
  Serial.print(millis()); Serial.print(F(" "));     //ms
  Serial.print(ir);  Serial.print(F(" "));    //ir
  Serial.print(full); Serial.print(F(" "));     //full
  Serial.print(full - ir); Serial.print(F(" "));     //visible
  Serial.println(tsl.calculateLux(full, ir), 6);          //lux




  
  
}

void unifiedSensorAPIRead(void)
{
  /* Get a new sensor event */ 
  sensors_event_t event;
  tsl.getEvent(&event);
 
  /* Display the results (light is measured in lux) */
  Serial.print(F("[ ")); Serial.print(event.timestamp); Serial.print(F(" ms ] "));
  if ((event.light == 0) |
      (event.light > 4294966000.0) | 
      (event.light <-4294966000.0))
  {
    /* If event.light = 0 lux the sensor is probably saturated */
    /* and no reliable data could be generated! */
    /* if event.light is +/- 4294967040 there was a float over/underflow */
    Serial.println(F("Invalid data (adjust gain or timing)"));
  }
  else
  {
    Serial.print(event.light); Serial.println(F(" lux"));
  }
}



void myFunction1(int steps) {
  if (i < max_steps) {
  for (int j = 0; j<steps ;j++){
      digitalWrite(STEP, HIGH);
  delay(100);
  digitalWrite(STEP, LOW);
  delay(100);
  //Serial.println(i);
  i++;
  }
  } else {
    stav = 1;
    digitalWrite(DIR, HIGH);
  }
}

void myFunction2(int steps) {
  for (int j = 0; j<steps ;j++){
      if (i < max_steps) {
          digitalWrite(STEP, HIGH);
          delay(delay_step);
          digitalWrite(STEP, LOW);
          delay(delay_step);
          //Serial.println(i);
          i++;
      }
      else {
          stav = 1;
          if (dir == 0) {
              digitalWrite(DIR, HIGH);
          } 
          else if (dir == 1) {
              digitalWrite(DIR, LOW);
          }
      }
  }
}



void restart() {
  for (int j = 0; j < max_steps; j++) {
   digitalWrite(STEP, HIGH);
  delay(10);
  digitalWrite(STEP, LOW);
  delay(10);
  }
   stav = 2;
}


void read() {
  
}


void loop() {
  // put your main code here, to run repeatedly:
  if (stav == 0) {
    advancedRead();
    myFunction2(number_of_steps_between);
  } 
}
