#include <map>
#include <list>
#include <MIDI.h>
#include <USBHost_t36.h> // access to USB MIDI devices (plugged into 2nd USB port)
std::map<int, std::list<int>> MidiMap = {
  { 0, {0} },
  { 1, {1} },
  { 2, {2} },
  { 3, {3} },
  { 4, {4} },
  { 5, {5} },
  { 6, {6} },
  { 7, {7} },
  { 8, {8} },
  { 9, {9} },
  { 10, {10} },
  { 11, {11} },
  { 12, {12} },
  { 13, {13} },
  { 14, {14} },
  { 15, {15} },
  { 16, {16} },
  { 17, {17} },
  { 18, {18} },
  { 19, {19} },
  { 20, {20} },
  { 21, {21} },
  { 22, {22} },
  { 23, {23} },
  { 24, {24} },
  { 25, {25} },
  { 26, {26} },
  { 27, {27} },
  { 28, {28} },
  { 29, {29} },
  { 30, {30} },
  { 31, {31} },
  { 32, {32} },
  { 33, {33} },
  { 34, {34} },
  { 35, {35} }
};
// Create the Serial MIDI ports
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI1);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI2);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial3, MIDI3);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial4, MIDI4);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial5, MIDI5);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial6, MIDI6);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial7, MIDI7);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial8, MIDI8);

midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> *hwMidis[8] = { &MIDI1, &MIDI2, &MIDI3, &MIDI4, &MIDI5, &MIDI6, &MIDI7, &MIDI8 };
// Create the ports for USB devices plugged into Teensy's 2nd USB port (via hubs)
USBHost myusb;
USBHub hub1(myusb);
USBHub hub2(myusb);
USBHub hub3(myusb);
USBHub hub4(myusb);
MIDIDevice midi01(myusb);
MIDIDevice midi02(myusb);
MIDIDevice midi03(myusb);
MIDIDevice midi04(myusb);
MIDIDevice midi05(myusb);
MIDIDevice midi06(myusb);
MIDIDevice midi07(myusb);
MIDIDevice midi08(myusb);
MIDIDevice * midilist[8] = {
  &midi01, &midi02, &midi03, &midi04, &midi05, &midi06, &midi07, &midi08,
};


// A variable to know how long the LED has been turned on
elapsedMillis ledOnMillis;
