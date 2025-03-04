#include "Adafruit_TLC59711.h"
#include <SPI.h>
#include <XInput.h>

// PCB Rev1 Button/Switch mapping
#define BTN0 5
#define BTN1 6
#define BTN2 7
#define BTN3 8
#define BTN4 9
#define BTN5 10
#define BTN6 11
#define BTN7 12
#define SW0  19
#define SW1  20
#define SW2  21
#define SW3  22
#define SW4  23
#define SW5  24
#define SW6  25
#define SW7  26

#define TLC59711_CHAINED 1
#define TLC59711_DATA    11
#define TLC59711_CLK     13
Adafruit_TLC59711 tlc = Adafruit_TLC59711(TLC59711_CHAINED, TLC59711_CLK, TLC59711_DATA);

bool ledStates[24]{};

void setup() {
  pinMode(BTN0, INPUT_PULLDOWN);
  pinMode(BTN1, INPUT_PULLDOWN);
  pinMode(BTN2, INPUT_PULLDOWN);
  pinMode(BTN3, INPUT_PULLDOWN);
  pinMode(BTN4, INPUT_PULLDOWN);
  pinMode(BTN5, INPUT_PULLDOWN);
  pinMode(BTN6, INPUT_PULLDOWN);
  pinMode(BTN7, INPUT_PULLDOWN);
  pinMode(SW0 , INPUT_PULLDOWN);
  pinMode(SW1 , INPUT_PULLDOWN);
  pinMode(SW2 , INPUT_PULLDOWN);
  pinMode(SW3 , INPUT_PULLDOWN);
  pinMode(SW4 , INPUT_PULLDOWN);
  pinMode(SW5 , INPUT_PULLDOWN);
  pinMode(SW6 , INPUT_PULLDOWN);
  pinMode(SW7 , INPUT_PULLDOWN);

  pinMode(A4, INPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);

  tlc.begin();
  tlc.write();

	XInput.setReceiveCallback(rumbleCallback);
	XInput.begin();
}

void loop() {
  for(int on_i=0; on_i<24; on_i++) {
    for(int i=0; i<24; i++) {
      if (i == on_i) {
        tlc.setPWM(i, 65535); // on
      } else {
        tlc.setPWM(i, 0); // off
      }

      // tlc.setPWM(i, ledStates[i] * 65535);
    }
    tlc.write();
    delay(500);
  }
}

void rumbleCallback(uint8_t packetType) {
	// If we have an LED packet
	if (packetType == (uint8_t) XInputReceiveType::LEDs) {
		return;
	}

	// If we have a rumble packet
	if (packetType == (uint8_t) XInputReceiveType::Rumble) {
		return;
	}
}
