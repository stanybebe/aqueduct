

#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();


#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <tables/sin2048_int8.h>
//#include <tables/buchla2048_int8.h>// sine table for oscillator

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);
const int buttonPin = 2;     // the number of the pushbutton pin
const int outPin =  2;
byte commandByte;
byte noteByte;
byte velocityByte;
byte noteOn = 144;
const int sensorMin = 0;      // sensor minimum, discovered through experiment
const int sensorMax = 600;    // sensor maximum, discovered through experiment
int stepi = 0;
boolean timer = false;
int t;
int currentPitch;
float tempo;
float mtofarray[] = {0, 8.661957, 9.177024, 9.722718, 10.3, 10.913383, 11.562325, 12.25, 12.978271, 13.75, 14.567617, 15.433853, 16.351599, 17.323914, 18.354048, 19.445436, 20.601723, 21.826765, 23.124651, 24.5, 25.956543, 27.5, 29.135235, 30.867706, 32.703197, 34.647827, 36.708096, 38.890873, 41.203445, 43.65353, 46.249302, 49., 51.913086, 55., 58.27047, 61.735413, 65.406395, 69.295654, 73.416191, 77.781746, 82.406891, 87.30706, 92.498604, 97.998856, 103.826172, 110., 116.540939, 123.470825, 130.81279, 138.591309, 146.832382, 155.563492, 164.813782, 174.61412, 184.997208, 195.997711, 207.652344, 220., 233.081879, 246.94165, 261.62558, 277.182617, 293.664764, 311.126984, 329.627563, 349.228241, 369.994415, 391.995422, 415.304688, 440., 466.163757, 493.883301, 523.25116, 554.365234, 587.329529, 622.253967, 659.255127, 698.456482, 739.988831, 783.990845, 830.609375, 880., 932.327515, 987.766602, 1046.502319, 1108.730469, 1174.659058, 1244.507935, 1318.510254, 1396.912964, 1479.977661, 1567.981689, 1661.21875, 1760., 1864.655029, 1975.533203, 2093.004639, 2217.460938, 2349.318115, 2489.015869, 2637.020508, 2793.825928, 2959.955322, 3135.963379, 3322.4375, 3520., 3729.31, 3951.066406, 4186.009277, 4434.921875, 4698.63623, 4978.031738, 5274.041016, 5587.651855, 5919.910645, 6271.926758, 6644.875, 7040., 7458.620117, 7902.132812, 8372.018555, 8869.84375, 9397.272461, 9956.063477, 10548.082031, 11175.303711, 11839.821289, 12543.853516, 13289.75};
float melody[] = {
  261.6, 293.6, 329.6, 349.2, 391.9, 440, 493.8, 523.2
};

float melodyB[] = {
  524.5, 600.3, 688, 803, 918.6, 1055.5, 918.6, 803
};

float melodyC[] = {
  261.6, 294.33, 313.96, 348.84, 392.52, 418.61, 470.93, 523.26
};

// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 64 // Hz, powers of 2 are most reliable

int buttonState = 0;
void setup() {
  Serial.begin(9600);
  startMozzi(CONTROL_RATE); // :)
  pinMode(outPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  currentPitch = 60;
  //  digitalWrite(13,LOW);
  MIDI.begin();


}

//void cMIDI() {
//  do{
//    if (Serial.available()){
//      commandByte = Serial.read();//read first byte
//      noteByte = Serial.read();//read next byte
//      velocityByte = Serial.read();//read final byte
//       if (commandByte == noteOn){//if note on message
//           currentPitch = noteByte;
//
//        }
//    }
//  }
//  while (Serial.available() > 2);//when at least three bytes available
//}
void updateControl() {

  int sensorValue = analogRead(5);

  float voltage  = map(sensorValue, 0, 1023, 1, 50);

  int sensorValueB = mozziAnalogRead(4);

  int range = map(sensorValueB, 0, 1023, 1, 50);
  int yep = round(random(0, 8));

  //  t++;
  //
  //  tempo = 2+ range;
  //  if ( t > tempo) {
  //    timer = true;
  //    t = 0;
  //  }else{timer=false;}
  //
  //  if (timer == true) {
  //    stepi = stepi + 1;
  //
  //    if (stepi > 7) {
  //      stepi = 0;
  //
  //    }
  //
  //
  //  }

  aSin.setFreq(mtofarray[currentPitch]);

}
int updateAudio() {
  return aSin.next(); // return an int signal centred around 0
}

void loop() {
  //  cMIDI();
  //  delay(100);
  //  digitalWrite(13,LOW);//turn led off
  //  int sensorValue = analogRead(5);
  //
  //  float voltage  = map(sensorValue, 0, 1023, 1, 50);
  audioHook(); // required here
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    digitalWrite(outPin, HIGH);
  } else {
    digitalWrite(outPin, LOW);
  }

  if (MIDI.read())                // Is there a MIDI message incoming ?
  {
    switch (MIDI.getType())     // Get the type of the message we caught
    {
      case midi::NoteOn:

        currentPitch = MIDI.getData1();

      default:
        break;
    }
  }

}
