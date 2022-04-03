/**
 * @file Simple_PCINT.ino
 * @author nguyenmanhthao996tn (manhthao.spm@gmail.com/thaonm@rfthings.com.vn)
 * @brief This library demonstrates Pin Change Interrupt on UCA21 Board by attaching 2 seperated handler functions to Button0 & Button1 to control on-board LED. Button0 turns ON the LED and Button1 turns it OFF.
 * @version 0.0.1
 * @date 2022-04-03
 *
 */

#include <mPCINT.h>

#define PIN_LED 13
#define PIN_BTN0 2
#define PIN_BTN1 3

uint8_t led_state = LOW;

void setup(void)
{
  Serial.begin(115200);
  Serial.println("PCINT on UCA21 Demo");

  pinMode(PIN_LED, OUTPUT);

  pinMode(PIN_BTN0, INPUT_PULLUP);
  attachPinChangeInterrupt(digitalPinToPCInterrupt(PIN_BTN0), button0_handler, mPCINT_RISING);

  pinMode(PIN_BTN1, INPUT_PULLUP);
  attachPinChangeInterrupt(digitalPinToPCInterrupt(PIN_BTN1), button1_handler, mPCINT_RISING);
}

void loop(void)
{
  digitalWrite(PIN_LED, led_state);
}

void button0_handler(void)
{
  led_state = HIGH;
}

void button1_handler(void)
{
  led_state = LOW;
}
