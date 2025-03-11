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
#define LED0 11
#define LED1 10
#define LED2 9
#define LED3 8
#define LED4 7
#define LED5 6
#define LED6 5
#define LED7 4

const int ledArray[8] = {LED0, LED1, LED2, LED3, LED4, LED5, LED6, LED7};

#define TLC59711_CHAINED 1
#define TLC59711_DATA    3
#define TLC59711_CLK     2
Adafruit_TLC59711 tlc = Adafruit_TLC59711(TLC59711_CHAINED, TLC59711_CLK, TLC59711_DATA);

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

  delay(10);
  for (int i=0; i<8; i++) {
    tlc.setPWM(ledArray[i], 65535);
    tlc.write();
    delay(120);
  }

  XInput.setAutoSend(false);
	XInput.setReceiveCallback(rumbleCallback);
	XInput.begin();
}

uint8_t rumbleValue = 0;

void loop() {
  XInput.releaseAll();

  if (digitalRead(BTN0)) {
    XInput.press(BUTTON_A);
  }

  if (digitalRead(BTN1)) {
    XInput.press(BUTTON_B);
  }

  if (digitalRead(BTN2)) {
    XInput.press(BUTTON_X);
  }

  if (digitalRead(BTN3)) {
    XInput.press(BUTTON_Y);
  }

  if (digitalRead(BTN4) || digitalRead(BTN5)) {
    XInput.press(BUTTON_LB);
  }

  if (digitalRead(BTN6) || digitalRead(BTN7)) {
    XInput.press(BUTTON_RB);
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

  // Top Left
  if ((rumbleValue & 0b1) > 0) {
    tlc.setPWM(LED0, 65535);
  } else {
    tlc.setPWM(LED0, 0);
  }

  if ((rumbleValue & 0b10) > 0) {
    tlc.setPWM(LED1, 65535);
  } else {
    tlc.setPWM(LED1, 0);
  }

  if ((rumbleValue & 0b100) > 0) {
    tlc.setPWM(LED2, 65535);
  } else {
    tlc.setPWM(LED2, 0);
  }

  // Top Right
  if ((rumbleValue & 0b1000) > 0) {
    tlc.setPWM(LED3, 65535);
  } else {
    tlc.setPWM(LED3, 0);
  }

  // Bottom Left
  if ((rumbleValue & 0b10000) > 0) {
    tlc.setPWM(LED4, 65535);
  } else {
    tlc.setPWM(LED4, 0);
  }

  if ((rumbleValue & 0b100000) > 0) {
    tlc.setPWM(LED5, 65535);
  } else {
    tlc.setPWM(LED5, 0);
  }

  if ((rumbleValue & 0b1000000) > 0) {
    tlc.setPWM(LED6, 65535);
  } else {
    tlc.setPWM(LED6, 0);
  }

  // Bottom Right
  if ((rumbleValue & 0b10000000) > 0) {
    tlc.setPWM(LED7, 65535);
  } else {
    tlc.setPWM(LED7, 0);
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
