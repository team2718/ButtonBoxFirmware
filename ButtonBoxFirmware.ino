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
#define TLC59711_DATA    3
#define TLC59711_CLK     2
Adafruit_TLC59711 tlc = Adafruit_TLC59711(TLC59711_CHAINED, TLC59711_CLK, TLC59711_DATA);

int elePosition = 4;
bool isLeft = true;

const int NumButtons = 10;
const int Buttons[NumButtons] = {
	BUTTON_A,
	BUTTON_B,
	BUTTON_X,
	BUTTON_Y,
	BUTTON_LB,
	BUTTON_RB,
	BUTTON_BACK,
	BUTTON_START,
	BUTTON_L3,
	BUTTON_R3,
};

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

  XInput.setAutoSend(false);
	XInput.setReceiveCallback(rumbleCallback);
	XInput.begin();
}

uint8_t rumbleValue = 0;

void loop() {
  XInput.releaseAll();

  if (digitalRead(BTN0)) {
    XInput.press(BUTTON_A);
    elePosition = 1;
  }

  if (digitalRead(BTN1)) {
    XInput.press(BUTTON_B);
    elePosition = 2;
  }

  if (digitalRead(BTN2)) {
    XInput.press(BUTTON_X);
    elePosition = 3;
  }

  if (digitalRead(BTN3)) {
    XInput.press(BUTTON_Y);
    elePosition = 4;
  }

  if (digitalRead(BTN4) || digitalRead(BTN5)) {
    XInput.press(BUTTON_LB);
    isLeft = true;
  }

  if (digitalRead(BTN6) || digitalRead(BTN7)) {
    XInput.press(BUTTON_RB);
    isLeft = false;
  }

  int32_t sw_axis = 0;

  if (digitalRead(SW0)) {
    sw_axis += 128;
  }

  if (digitalRead(SW1)) {
    sw_axis += 64;
  }

  if (digitalRead(SW2)) {
    sw_axis += 32;
  }

  if (digitalRead(SW3)) {
    sw_axis += 16;
  }

  if (digitalRead(SW4)) {
    sw_axis += 8;
  }

  XInput.setTrigger(TRIGGER_LEFT, sw_axis);

  XInput.send();

  for (int i=0; i<16; i++) {
    tlc.setPWM(i, 0);
  }

  // switch (elePosition) {
  //   case 1:
  //     tlc.setPWM(11, 65535); break;
  //   case 2:
  //     tlc.setPWM(10, 65535); break;
  //   case 3:
  //     tlc.setPWM(9, 65535); break;
  //   case 4:
  //     tlc.setPWM(8, 65535); break;
  // }

  // if (!isLeft) {
  //   tlc.setPWM(4, 65535);
  //   tlc.setPWM(5, 65535);
  // } else {
  //   tlc.setPWM(6, 65535);
  //   tlc.setPWM(7, 65535);
  // }

  // Top Left
  if ((rumbleValue & 0b1) > 0) {
    tlc.setPWM(11, 65535);
  } else {
    tlc.setPWM(11, 0);
  }

  if ((rumbleValue & 0b10) > 0) {
    tlc.setPWM(10, 65535);
  } else {
    tlc.setPWM(10, 0);
  }

  if ((rumbleValue & 0b100) > 0) {
    tlc.setPWM(9, 65535);
  } else {
    tlc.setPWM(9, 0);
  }

  // Top Right
  if ((rumbleValue & 0b1000) > 0) {
    tlc.setPWM(8, 65535);
  } else {
    tlc.setPWM(8, 0);
  }

  // Bottom Left
  if ((rumbleValue & 0b10000) > 0) {
    tlc.setPWM(7, 65535);
  } else {
    tlc.setPWM(7, 0);
  }

  if ((rumbleValue & 0b100000) > 0) {
    tlc.setPWM(6, 65535);
  } else {
    tlc.setPWM(6, 0);
  }

  if ((rumbleValue & 0b1000000) > 0) {
    tlc.setPWM(5, 65535);
  } else {
    tlc.setPWM(5, 0);
  }

  // Bottom Right
  if ((rumbleValue & 0b10000000) > 0) {
    tlc.setPWM(4, 65535);
  } else {
    tlc.setPWM(4, 0);
  }


  tlc.write();
  delay(20);
}

void rumbleCallback(uint8_t packetType) {
	// If we have an LED packet
	if (packetType == (uint8_t) XInputReceiveType::LEDs) {
		return;
	}

	// If we have a rumble packet
	if (packetType == (uint8_t) XInputReceiveType::Rumble) {
    if (XInput.getRumbleRight() > 0) {
      rumbleValue = XInput.getRumbleLeft();
    }
	}
}
