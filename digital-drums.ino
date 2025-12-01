#include <MIDI.h>

#define PIEZO_BASS A0
#define PIEZO_SNARE1 A1
#define PIEZO_SNARE2 A2
#define PIEZO_SNARE3 A3
#define PIEZO_HI_HAT1 A4
#define PIEZO_HI_HAT2 A5
#define PIEZO_HI_HAT3 A6
#define GND1 53
#define GND2 52
#define GND3 51
#define GND4 50
#define GND5 49
#define GND6 48
#define GND7 47

const int minimalSignal = 900;
int defaultSignal = 1500;
bool snareBeat = false;
bool bassBeat = false;
bool hiHatBeat = false;

int maxSumSignal = 0;

int minSignalBass = 1000;
int minSignalSnare = 2000;
int minSignalHiHat = 1500;
int time = 0;

void setup() {
  Serial.begin(31250);

  pinMode(GND1, OUTPUT);
  pinMode(GND2, OUTPUT);
  pinMode(GND3, OUTPUT);
  pinMode(GND4, OUTPUT);
  pinMode(GND5, OUTPUT);
  pinMode(GND6, OUTPUT);
  pinMode(GND7, OUTPUT);
  digitalWrite(GND1, LOW);
  digitalWrite(GND2, LOW);
  digitalWrite(GND3, LOW);
  digitalWrite(GND4, LOW);
  digitalWrite(GND5, LOW);
  digitalWrite(GND6, LOW);
  digitalWrite(GND7, LOW);
}

void loop() {
  // Calibration();
  CheckBass();
  CheckSnare();
  CheckHiHat();
  
}

// void Calibration() {
//   if (time < 1000) {
//     int sensorValueBass = analogRead(PIEZO_BASS);
//     int sumSignalBass = sensorValueBass;
//     minSignalBass = (minSignalBass + sensorValueBass + 500)/2;
//     time++;
//   }
//   if (time == 1000) {
//     Serial.println(minSignalBass);
//     time++;
//   }

  
// }

void CheckBass() {
  int sensorValueBass = analogRead(PIEZO_BASS);
  int sumSignalBass = sensorValueBass;

  if (sumSignalBass > minSignalBass & !bassBeat) {
    bassBeat = true;
    noteOn(0x90, 0x24, 45);
    delay(1);
    noteOn(0x90, 0x24, 0x00);
    // Serial.println(sumSignalBass);
  }
  if (sumSignalBass < minSignalBass & bassBeat) {
    bassBeat = false;
    // float midiSignal = float(maxSumSignal)*127/(1024*3);
    // Serial.println(midiSignal);
  } 
}

void CheckSnare() {
  int sensorValueSnare1 = analogRead(PIEZO_SNARE1);
  int sensorValueSnare2 = analogRead(PIEZO_SNARE2);
  int sensorValueSnare3 = analogRead(PIEZO_SNARE3);

  int sumSignalSnare = sensorValueSnare1 + sensorValueSnare2 + sensorValueSnare3;

  if (sumSignalSnare > minSignalSnare & !snareBeat) {
    snareBeat = true;

    float deltaForce = sumSignalSnare - minSignalSnare;

    int force = 127 * (deltaForce / (3072-minSignalSnare));
    int note = 0x28;
    // if (sensorValueSnare1 + sensorValueSnare2 > sensorValueSnare3 * 3) {
    //   note = 37;
    // }

    noteOn(0x90, note, force);
    delay(1);
    noteOn(0x90, note, 0x00);
    // Serial.println(sumSignalBass);
  }
  if (sumSignalSnare < minSignalSnare & snareBeat) {
    snareBeat = false;
    // float midiSignal = float(maxSumSignal)*127/(1024*3);
    // Serial.println(midiSignal);
  } 
}

void CheckHiHat() {
  int sensorValueHiHat1 = analogRead(PIEZO_HI_HAT1);
  int sensorValueHiHat2 = analogRead(PIEZO_HI_HAT2);
  int sensorValueHiHat3 = analogRead(PIEZO_HI_HAT3);
  
  int sumSignalHiHat = sensorValueHiHat1 + sensorValueHiHat1 + sensorValueHiHat1;

  if (sumSignalHiHat > minSignalHiHat & !hiHatBeat) {
    float deltaForce = sumSignalHiHat - minSignalHiHat;

    int force = 127 * (deltaForce / (3072-minSignalHiHat));
    int note = 0x28;


    hiHatBeat = true;
    noteOn(0x90, 0x2C, force);
    delay(1);
    noteOn(0x90, 0x2C, 0x00);
    // Serial.println(sumSignalBass);
  }
  if (sumSignalHiHat < minSignalHiHat & hiHatBeat) {
    hiHatBeat = false;
    // float midiSignal = float(maxSumSignal)*127/(1024*3);
    // Serial.println(midiSignal);
  } 
}


// Отправка MIDI-ноты.
// Здесь отсутствует проверка валидности данных.
// MIDI-команда должна быть больше 127, а номер ноты и сила нажатия - меньше, либо равна 127
void noteOn(int cmd, int pitch, int velocity) {
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}