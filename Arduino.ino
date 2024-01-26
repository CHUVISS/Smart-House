#include <SoftwareSerial.h>
#include <math.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#define MQ2_PIN A3
#define LED_PIN 7
#define ZUMMER_PIN 10
#define REL_PIN 12
#define TEMP_PIN A4
#define TEMPOUT_PIN A0
#define GAZD_PIN 6

SoftwareSerial esp(4, 5);

OneWire oneWire(TEMPOUT_PIN);
DallasTemperature sensor(&oneWire);

void setup() {
  esp.begin(9600);
  pinMode(REL_PIN, OUTPUT);
  pinMode(TEMP_PIN, INPUT);
  pinMode(MQ2_PIN, INPUT);
  pinMode(ZUMMER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(GAZD_PIN, OUTPUT);

  sensor.begin();
  sensor.setResolution(12);
}
void loop() {
  String command = esp.readStringUntil('\n');
  command.trim();
  float tempout;
  float temp;
  int MQ2 = analogRead (MQ2_PIN);

  float v_temp = 1023.0 / analogRead(TEMP_PIN) - 1.0;
  temp = 1.0 / (-log(v_temp)/3977.0 + 1.0 / 295.0) - 273.0;
  temp = temp * 100;
  int temp1 = int(temp);
  
  sensor.requestTemperatures();
  tempout = sensor.getTempCByIndex(0);
  tempout = tempout * 100;
  int tempout1 = int(tempout);
  if (command == "T"){
    esp.println(temp1);
  }
  if (command == "TO"){
    esp.println(tempout1);
  }
  if (command == "On"){
    digitalWrite(REL_PIN, HIGH);
  }
  if (command == "Off"){
    digitalWrite(REL_PIN, LOW);
  }
  //газ
  if (MQ2 > 450){
    tone(ZUMMER_PIN, 500);
    digitalWrite(LED_PIN, LOW);
    delay(500);
    tone(ZUMMER_PIN, 1000);
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(GAZD_PIN, HIGH);
  }
  else{
    noTone(ZUMMER_PIN);
    digitalWrite(LED_PIN, LOW);
    digitalWrite(GAZD_PIN, LOW);
  }
  delay(100);
}
