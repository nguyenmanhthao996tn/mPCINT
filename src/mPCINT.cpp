/**
 * @file Simple_PCINT.ino
 * @author nguyenmanhthao996tn (manhthao.spm@gmail.com/thaonm@rfthings.com.vn)
 * @brief This library is created for RFThings UCA21 Boards to use Pin Change Interrupt with edge (RISING/FALLING) detection and seperated handlers for each pins.
 * @version 0.0.1
 * @date 2022-04-03
 *
 */

#include "mPCINT.h"

#define BIT_MASK(_reg, _bit_num) (_reg & ((uint32_t)1 << _bit_num))

static void nop_handler(void) {}
static volatile voidFuncPtr interrupt_list[27] = {
    nop_handler,
    nop_handler,
    nop_handler,
    nop_handler,
    nop_handler,
    nop_handler,
    nop_handler,
    nop_handler,
    nop_handler,
    nop_handler,
    nop_handler,
    nop_handler,
    nop_handler,
    nop_handler,
    nop_handler,
    nop_handler,
    nop_handler,
    nop_handler,
    nop_handler,
    nop_handler,
    nop_handler,
    nop_handler,
    nop_handler,
    nop_handler,
    nop_handler,
    nop_handler,
    nop_handler};
static mPCINT_data_t last_state, enable_mask, mode_mask;

#if defined(__AVR_ATmega328PB__)
uint8_t digitalPinToPCInterrupt(int pin)
{
  switch (pin)
  {
    // 16 - 23
  case 0:
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case 7:
    return (pin + 16);
    break;

    // 0 - 7
  case 8:
  case 9:
  case 10:
  case 11:
  case 12:
  case 13:
    return (pin - 8);
    break;

    // 8 - 13
  case PIN_A0:
    return 8;
    break;

  case PIN_A1:
    return 9;
    break;

  case PIN_A2:
    return 10;
    break;

  case PIN_A3:
    return 11;
    break;

  case PIN_A4:
    return 12;
    break;

  case PIN_A5:
    return 13;
    break;

  default:
    // 14: RESET PIN
    // 15: NOT EXIST
    // 24-27: PORTE (Not support yet)
    return -1;
    break;
  }
}

void attachPinChangeInterrupt(int interruptNum, voidFuncPtr handlerFunction, mPCINT_Mode_t mode)
{
  if ((interruptNum < 0) || (interruptNum > 27))
    return; // Todo: Return ERROR State
  if (handlerFunction == NULL)
    return; // Todo: Return ERROR State
  if (!((mode == mPCINT_FALLING) || (mode == mPCINT_RISING)))
    return; // Todo: Return ERROR State

  enable_mask.data32 |= ((uint32_t)1 << interruptNum);
  mode_mask.data32 = (mode_mask.data32 & ~((uint32_t)1 << interruptNum)) | ((uint32_t)mode << interruptNum);
  interrupt_list[interruptNum] = handlerFunction;

  if ((interruptNum >= 0) && (interruptNum <= 7))
  {
    // PORTB
    PCMSK0 = enable_mask.data8.portb;
    PCICR |= (1 << PCIE0);
  }
  else if ((interruptNum >= 8) && (interruptNum <= 14))
  {
    // PORTC
    PCMSK1 = enable_mask.data8.portc;
    PCICR |= (1 << PCIE1);
  }
  else if ((interruptNum >= 16) && (interruptNum <= 23))
  {
    // PORTD
    PCMSK2 = enable_mask.data8.portd;
    PCICR |= (1 << PCIE2);
  }
  else if ((interruptNum >= 24) && (interruptNum <= 27))
  {
    // PORTE
    PCMSK3 = enable_mask.data8.porte;
    PCICR |= (1 << PCIE3);
  }
  else
  {
    // Todo: Return ERROR State
  }
}

void detachPinChangeInterrupt(int interruptNum)
{
  if ((interruptNum < 0) || (interruptNum > 27))
    return; // Todo: Return ERROR State

  if ((interruptNum < 0) || (interruptNum > 27))
    return;

  enable_mask.data32 &= ~((uint32_t)1 << interruptNum);
  interrupt_list[interruptNum] = nop_handler;

  if ((interruptNum >= 0) && (interruptNum <= 7))
  {
    // PORTB
    PCMSK0 = enable_mask.data8.portb;
    if (PCMSK0 == 0)
      PCICR &= ~(1 << PCIE0);
  }
  else if ((interruptNum >= 8) && (interruptNum <= 14))
  {
    // PORTC
    PCMSK1 = enable_mask.data8.portc;
    if (PCMSK1 == 0)
      PCICR |= ~(1 << PCIE1);
  }
  else if ((interruptNum >= 16) && (interruptNum <= 23))
  {
    // PORTD
    PCMSK2 = enable_mask.data8.portd;
    if (PCMSK2 == 0)
      PCICR |= ~(1 << PCIE2);
  }
  else if ((interruptNum >= 24) && (interruptNum <= 27))
  {
    // PORTE
    PCMSK3 = enable_mask.data8.porte;
    if (PCMSK3 == 0)
      PCICR |= ~(1 << PCIE3);
  }
  else
  {
    // Todo: Return ERROR State
  }
}

ISR(PCINT0_vect)
{
  uint8_t input = PINB;

  for (uint8_t index = 0; index < 8; index++)
  {
    if ((BIT_MASK(enable_mask.data8.portb, index) != 0) && (BIT_MASK(input, index) != BIT_MASK(last_state.data8.portb, index)) && (BIT_MASK(input, index) == BIT_MASK(mode_mask.data8.portb, index)))
    {
      interrupt_list[index]();
    }
  }

  last_state.data8.portb = input;
}

ISR(PCINT1_vect)
{
  uint8_t input = PINC;

  for (uint8_t index = 0; index < 8; index++)
  {
    if ((BIT_MASK(enable_mask.data8.portc, index) != 0) && (BIT_MASK(input, index) != BIT_MASK(last_state.data8.portc, index)) && (BIT_MASK(input, index) == BIT_MASK(mode_mask.data8.portc, index)))
    {
      interrupt_list[index + 8]();
    }
  }

  last_state.data8.portc = input;
}

ISR(PCINT2_vect)
{
  uint8_t input = PIND;

  for (uint8_t index = 0; index < 8; index++)
  {
    if ((BIT_MASK(enable_mask.data8.portd, index) != 0) && (BIT_MASK(input, index) != BIT_MASK(last_state.data8.portd, index)) && (BIT_MASK(input, index) == BIT_MASK(mode_mask.data8.portd, index)))
    {
      interrupt_list[index + 16]();
    }
  }

  last_state.data8.portd = input;
}

ISR(PCINT3_vect)
{
  uint8_t input = PINE;

  for (uint8_t index = 0; index < 8; index++)
  {
    if ((BIT_MASK(enable_mask.data8.porte, index) != 0) && (BIT_MASK(input, index) != BIT_MASK(last_state.data8.porte, index)) && (BIT_MASK(input, index) == BIT_MASK(mode_mask.data8.porte, index)))
    {
      interrupt_list[index + 24]();
    }
  }

  last_state.data8.porte = input;
}

#else
#error This library is created for ATMega328PB only. Check your board configuration and try again!
uint8_t digitalPinToPCInterrupt(int pin)
{
}
void attachPinChangeInterrupt(int interruptNum, voidFuncPtr handlerFunction, mPCINT_Mode_t mode) {}
void detachPinChangeInterrupt(int interruptNum) {}
#endif