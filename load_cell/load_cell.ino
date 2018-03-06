/*
  Using single SparkFun HX711 breakout board with a scale
  for calibration with force sensor (referred by Nathan Seidle's code)
  Author: JPark 
  Date: 2018-03-06
  HX711 does one thing well: read load cells. The breakout board is compatible with any wheat-stone bridge
  based load cell which should allow a user to measure everything from a few grams to tens of tons.
  pin 2 -> CLK
  pin 3 -> DAT
  5V    -> VCC
  GND   -> GND

  The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.
*/

#include "HX711.h"

#define calibration_factor 8900.0 // obtained from load_cell_calibration.ino

#define CLK  2
#define DOUT  3

HX711 scale(DOUT, CLK);
float raw_data, prev;
int initialize = 0;
unsigned long t_cur, t_prev, t_loop;

void setup() {
  Serial.begin(9600);
  Serial.println("HX711 data print begins");
  
  scale.set_scale(calibration_factor);
}

void loop() {
  t_prev = t_cur;
  t_cur = millis();
  t_loop = t_cur - t_prev;
  
  if (initialize == 0)
  {
    raw_data = scale.get_units() * 10000.0;
    prev = raw_data;
    Serial.print(raw_data, 0);
    Serial.print("  ");
    initialize = 1;
  }
  else
  {
    raw_data = scale.get_units() * 10000.0;
      if (abs(raw_data - prev)/t_loop > 800)
        raw_data = prev;
      else
        prev = raw_data;

      Serial.print(raw_data, 0);
  }
  Serial.println();
}
