/**
 * @file Simple_PCINT.ino
 * @author nguyenmanhthao996tn (manhthao.spm@gmail.com/thaonm@rfthings.com.vn)
 * @brief This library is created for RFThings UCA21 Boards to use Pin Change Interrupt with edge (RISING/FALLING) detection and seperated handlers for each pins.
 * @version 0.0.1
 * @date 2022-04-03
 *
 */

#ifndef __MPCINT_H__
#define __MPCINT_H__

#include <Arduino.h>
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/********************* Data types *********************/
typedef void (*voidFuncPtr)(void);
typedef union
{
  uint32_t data32;
  struct
  {
    uint8_t portb;
    uint8_t portc;
    uint8_t portd;
    uint8_t porte;
  } data8;
} mPCINT_data_t;

typedef enum
{
  mPCINT_FALLING = 0,
  mPCINT_RISING
} mPCINT_Mode_t;

/********************* Methods *********************/
/**
 * @brief Convert a Arduino digital pin number to the corresponding PCINT number
 *
 * @param pin Arduino digital pin. Ex: 0, 1, 2, ..., A0, A1, ....
 * @return uint8_t The corresponding PCINT number
 */
uint8_t digitalPinToPCInterrupt(int pin);

/**
 * @brief Enable PCINT for one digital pin
 *
 * @param interruptNum PCINT number to active
 * @param handlerFunction Correspongding handler function
 * @param mode Dectect mode: mPCINT_RISING or mPCINT_FALLING
 */
void attachPinChangeInterrupt(int interruptNum, voidFuncPtr handlerFunction, mPCINT_Mode_t mode);

/**
 * @brief Disable PCINT for one digital pin
 *
 * @param interruptNum PCINT number to disable
 */
void detachPinChangeInterrupt(int interruptNum);

#endif /* __MPCINT_H__ */
