  #include <Adafruit_NeoPixel.h>
#include <AudioAnalyzer.h>

//LED data pin output
const int data_pin = 6;

//audio analyzer pin inputs
const int strobe_pin = 4;
const int rst_pin = 5;
const int analog_pin = 5;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(150, data_pin, NEO_GRB + NEO_KHZ800);

Analyzer Audio = Analyzer(strobe_pin,rst_pin, analog_pin);
int FreqVal[7];//

void setup() {
  Serial.begin(57600);
  pinMode(strobe_pin, INPUT);
  pinMode(rst_pin, INPUT);
  pinMode(data_pin, OUTPUT);
  Audio.Init();
  strip.begin();
  strip.setBrightness(50);
  strip.show();
}

void loop() {
  Audio.ReadFreq(FreqVal);//Return 7 values of 7 bands pass filiter
                          //Frequency(Hz):63  160  400  1K  2.5K  6.25K  16K
                          //FreqVal[]:      0    1    2    3    4    5    6  
  for(int i=0;i<7;i++) {
    //Serial.print(max((FreqVal[i]-100),0));
    Serial.print(FreqVal[i]);
    if(i<6) Serial.print(",");
    else Serial.println();
  }
  if (FreqVal[1] > 600 || FreqVal[2] > 600) {
    strip.fill(strip.Color(255, 0, 0));
  } else if (FreqVal[2] > 600 || FreqVal[3] > 600 || FreqVal[4] > 600) {
    strip.fill(strip.Color(0, 255, 0));
  } else if (FreqVal[2] > 600 || FreqVal[3] > 600) {
    strip.fill(strip.Color(0, 0, 255));
  }
}
