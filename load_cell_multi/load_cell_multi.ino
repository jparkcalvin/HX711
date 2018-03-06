/*
  Using multiple SparkFun HX711s breakout board with a scale (referred by Nathan Seidle's code)
  Author: JPark 
  Date: 2018-03-06
  HX711 does one thing well: read load cells. The breakout board is compatible with any wheat-stone bridge
  based load cell which should allow a user to measure everything from a few grams to tens of tons.
  pin 2 -> CLK
  pin 3, 4, 5, 6 -> DAT
  5V    -> VCC
  GND   -> GND

  The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.
*/

#include "HX711.h"

#define calibration_factor 8900.0 // obtained from load_cell_calibration.ino
#define n_Sensor 4

#define CLK  2
#define DOUT0  3
#define DOUT1  4
#define DOUT2  5
#define DOUT3  6

HX711 scale0(DOUT0, CLK);
HX711 scale1(DOUT1, CLK);
HX711 scale2(DOUT2, CLK);
HX711 scale3(DOUT3, CLK);

HX711 scale[n_Sensor];

float raw_data[n_Sensor], prev[n_Sensor];
int initialize = 0;
unsigned long t_cur, t_prev, t_loop;

void setup() {
  Serial.begin(9600);
  Serial.println("HX711 data print begins");

  scale[0] = scale0;
  scale[1] = scale1;
  scale[2] = scale2;
  scale[3] = scale3;

  for (int i = 0; i < n_Sensor; i++)
    scale[i].set_scale(calibration_factor);
}

void loop() {
  t_prev = t_cur;
  t_cur = millis();
  t_loop = t_cur - t_prev;
  
  if (initialize == 0)
  {
    for (int i = 0; i < n_Sensor; i++)
    {
      raw_data[i] = scale[i].get_units() * 10000.0;
      prev[i] = raw_data[i];
      Serial.print(raw_data[i], 0);
      Serial.print("  ");
    }
    initialize = 1;
  }
  else
  {
    for (int i = 0; i < n_Sensor; i++)
    {
      raw_data[i] = scale[i].get_units() * 10000.0;
      if (abs(raw_data[i] - prev[i])/t_loop > 800)
        raw_data[i] = prev[i];
      else
        prev[i] = raw_data[i];

     Serial.print(raw_data[i], 0);
      Serial.print("  ");
    }
  }
  Serial.println();
}
