/* Create a "class compliant " USB to 3 MIDI IN and 3 MIDI OUT interface.

   MIDI receive (6N138 optocoupler) input circuit and series resistor
   outputs need to be connected to Serial1, Serial2 and Serial3.

   You must select MIDIx4 from the "Tools > USB Type" menu

   This example code is in the public domain.
*/

// MIDI1-8 Serial Hardware, midiList 0-7 USB Host, USBMidi 0-15 Computer Host
#include "Router.h"

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT); // LED pin
  MIDI1.begin(MIDI_CHANNEL_OMNI);
  MIDI2.begin(MIDI_CHANNEL_OMNI);
  MIDI3.begin(MIDI_CHANNEL_OMNI);
  MIDI4.begin(MIDI_CHANNEL_OMNI);
  MIDI5.begin(MIDI_CHANNEL_OMNI);
  MIDI6.begin(MIDI_CHANNEL_OMNI);
  MIDI7.begin(MIDI_CHANNEL_OMNI);
  MIDI8.begin(MIDI_CHANNEL_OMNI);  
  MIDI1.turnThruOff();
  MIDI2.turnThruOff();
  MIDI3.turnThruOff();
  MIDI4.turnThruOff();
  MIDI5.turnThruOff();
  MIDI6.turnThruOff();
  MIDI7.turnThruOff();
  MIDI8.turnThruOff();
  
    digitalWriteFast(13, HIGH); // LED on
    delay(500);
    digitalWriteFast(13, LOW);

  myusb.begin();
  Serial.println("Startup complete.");
}


void loop() {
  bool activity = false;
  myusb.Task();
  for (int i = 0; i < 7; i++) {
    midi::MidiInterface<midi::SerialMIDI<HardwareSerial>>* lMIDI = hwMidis[i];
    if (lMIDI->read()) {
      byte type = lMIDI->getType();
      byte channel = lMIDI->getChannel();
      byte data1 = lMIDI->getData1();
      byte data2 = lMIDI->getData2();
      if (type != midi::SystemExclusive) {
      // MIDI1-8 Serial Hardware, midiList 0-7 USB Host, USBMidi 0-15 Computer Host
        for (int n : MidiMap[i]) {
          if (n < 8) {
            hwMidis[n]->send((midi::MidiType)type, data1, data2, channel);
          } else if (n < 16) {
            midilist[n - 8]->send(type, data1, data2, channel);
          } else {
            usbMIDI.send(type, data1, data2, channel, n-16);
          }
        } 
      } else {
        unsigned int SysExLength = data1 + data2 * 256;
        if (handleSysEx(SysExLength, lMIDI->getSysExArray())) {
          for (int n : MidiMap[i]) {
            if (n < 8) {
              hwMidis[n]->sendSysEx(SysExLength, lMIDI->getSysExArray(), true);
            } else if (n < 16) {
              midilist[n - 8]->sendSysEx(SysExLength, lMIDI->getSysExArray(), true);
            } else {
              usbMIDI.sendSysEx(SysExLength, lMIDI->getSysExArray(), true, n-16);
            }
          }
        }
      }
    }
  }


  // Next read messages arriving from the (up to) 10 USB devices plugged into the USB Host port
  for (int port=0; port < 8; port++) {
    if (midilist[port]->read()) {
      uint8_t type =       midilist[port]->getType();
      uint8_t data1 =      midilist[port]->getData1();
      uint8_t data2 =      midilist[port]->getData2();
      uint8_t channel =    midilist[port]->getChannel();
      const uint8_t *sys = midilist[port]->getSysExArray();
      Serial.print("USB MIDI: ");
      Serial.print("(");
      Serial.print(port);
      Serial.print(") ");
      Serial.print(type);
      Serial.print(" : ");
      Serial.print(data1);
      Serial.print(" ");
      Serial.print(data2);
      Serial.print(" - ");
      Serial.println(channel);
      if (type != midi::SystemExclusive) {
        for (int n : MidiMap[port+8]) {
          if (n < 8) {
            hwMidis[n]->send((midi::MidiType)type, data1, data2, channel);
          } else if (n < 16) {
            midilist[n - 8]->send(type, data1, data2, channel);
          } else {
            usbMIDI.send(type, data1, data2, channel, n-16);
          }
        } 
      } else {
        unsigned int SysExLength = data1 + data2 * 256;
        if (handleSysEx(SysExLength, sys)) {
          for (int n : MidiMap[port+8]) {
            if (n < 8) {
              hwMidis[n]->sendSysEx(SysExLength, sys, true);
            } else if (n < 16) {
              midilist[n - 8]->sendSysEx(SysExLength, sys, true);
            } else {
              usbMIDI.sendSysEx(SysExLength, sys, true, n-16);
            }
          }
        }
      }
      activity = true;
    }
  }




  if (usbMIDI.read()) {
    byte type = usbMIDI.getType();
    byte channel = usbMIDI.getChannel();
    byte data1 = usbMIDI.getData1();
    byte data2 = usbMIDI.getData2();
    byte cable = usbMIDI.getCable();
      Serial.print("Computer MIDI: ");
      Serial.print(type);
      Serial.print(" : ");
      Serial.print(data1);
      Serial.print(" ");
      Serial.print(data2);
      Serial.print(" - ");
      Serial.println(channel);
      if (type != midi::SystemExclusive) {
        for (int n : MidiMap[cable+16]) {
          if (n < 8) {
            hwMidis[n]->send((midi::MidiType)type, data1, data2, channel);
          } else if (n < 16) {
            midilist[n - 8]->send(type, data1, data2, channel);
          } else {
            usbMIDI.send(type, data1, data2, channel, n-16);
          }
        } 
      } else {
        unsigned int SysExLength = data1 + data2 * 256;
        Serial.println("Handling SysEx from USB");
        if (handleSysEx(SysExLength, usbMIDI.getSysExArray())) {
          for (int n : MidiMap[cable+16]) {
            if (n < 8) {
              hwMidis[n]->sendSysEx(SysExLength, usbMIDI.getSysExArray(), true);
            } else if (n < 16) {
              midilist[n - 8]->sendSysEx(SysExLength, usbMIDI.getSysExArray(), true);
            } else {
              usbMIDI.sendSysEx(SysExLength, usbMIDI.getSysExArray(), true, n-16);
            }
          }
        }
      }
    activity = true;
  }

  // blink the LED when any activity has happened
  if (activity) {
    digitalWriteFast(13, HIGH); // LED on
    ledOnMillis = 0;
  }
  if (ledOnMillis > 15) {
    digitalWriteFast(13, LOW);  // LED off
  }

}

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

bool to_hex(char* dest, size_t dest_len, const uint8_t* values, size_t val_len) {
    if(dest_len < (val_len*2+1)) /* check that dest is large enough */
        return false;
    *dest = '\0'; /* in case val_len==0 */
    while(val_len--) {
        /* sprintf directly to where dest points */
        sprintf(dest, "%02X", *values);
        dest += 2;
        ++values;
    }
    return true;
}

bool handleSysEx(unsigned int SysExLength, const uint8_t *SysExData) {
  Serial.println(SysExLength);
  char buf[SysExLength*2+1];
  if(to_hex(buf, sizeof(buf), SysExData, SysExLength))
    Serial.println(buf);
        
  if (SysExLength != 7)
    return true;
  if (SysExData[0] == 0xf0
  && SysExData[6] == 0xf7
  && SysExData[1] == 0x7D // 7D is private use (non-commercial)
  ) {
    int ID = SysExData[3];
    int Command = SysExData[4]; //0 = Clear, 1 = Add, 2 = Remove
    int Target = SysExData[5];
    if (Command == 0) {
      MidiMap[ID].clear();
      Serial.print("Cleared ");
      Serial.println(ID);
    } else if (Command == 1) {
      MidiMap[ID].push_back(Target);
      Serial.print("Added ");
      Serial.print(Target);
      Serial.print(" to ");
      Serial.println(ID);
    } else {
      MidiMap[ID].remove(Target);
      Serial.print("Removed ");
      Serial.print(Target);
      Serial.print(" from ");
      Serial.println(ID);

    }
    return false;
  }
  return true;
}
