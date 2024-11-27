#pragma once
/*
implementing a header inclusion gaurd so a particular header file is only included once in a build 
*/

#ifndef GPIO_H
#define GPIO_H

// includes 
#include <stdint.h>
#include "../../STM32C0xx_HAL_Driver/Inc/stm32c0xx_hal_def.h"
#include "../../CMSIS/Device/ST/STM32C0xx/Include/stm32c011xx.h"




/* to do define macros for
1. different modes (based on type)
2. pins 
*/ 

////////// MACROS //////////

// remember 8 in hexa decimal is 1000 and 16 is 10000 //

// remember all pin registers are 16 bit registers A,B,C,D,F

// PINS

#define GPIO_PIN_0          ((uint16_t)0x0001) // Select Pin 1 
#define GPIO_PIN_1          ((uint16_t)0x0002) // Select Pin 2 (remember 2 in hex is 10 in binary)
#define GPIO_PIN_2          ((uint16_t)0x0004) // Select Pin 3 (remember 4 in hex is 100 in binary)
#define GPIO_PIN_3          ((uint16_t)0x0008) // Select Pin 4 (remember 8 in hex is 1000 in binary)
#define GPIO_PIN_4          ((uint16_t)0x0010) // Select Pin 5 (remember 10 in hex 16 in decnimal which is 10000 in binary)
#define GPIO_PIN_5          ((uint16_t)0x0020) // Select Pin 6 (remember 20 in hex 32 in decnimal which is 100000 in binary)
#define GPIO_PIN_6          ((uint16_t)0x0040) // follow the pattern
#define GPIO_PIN_7          ((uint16_t)0x0080)
#define GPIO_PIN_8          ((uint16_t)0x0100)
#define GPIO_PIN_9          ((uint16_t)0x0200)
#define GPIO_PIN_10         ((uint16_t)0x0400)
#define GPIO_PIN_11         ((uint16_t)0x0800)
#define GPIO_PIN_12         ((uint16_t)0x1000)
#define GPIO_PIN_13         ((uint16_t)0x2000)
#define GPIO_PIN_14         ((uint16_t)0x4000)
#define GPIO_PIN_15         ((uint16_t)0x8000)
#define GPIO_PIN_SELECT_ALL ((uint16_t)0xFFFF)

#define GPIO_PIN_MASK       ((uint16_t)0xFFFF)

// MODE TYPES //

/*
Config Encode 0x00000XYZ
                Z: mode (input, output, alternate, analog)
                Y: OutputType (push-pull, open-drain)
*/

#define GPIO_MODE_INPUT         (0x00000000U)
#define GPIO_MODE_OUTPUT_PP     (0x00000001U)
#define GPIO_MODE_OUTPUT_OD     (0x00000011U)
#define GPIO_MODE_ALT_PP        (0x00000002U)
#define GPIO_MODE_ALT_PP        (0x00000002U)
#define GPIO_MODE_ALT_OD        (0x00000012U)
#define GPIO_MODE_ANA           (0x00000003U)

// PIN SPEED //

#define GPIO_SPEED_FREQ_VERY_LOW    (0x00000000U)
#define GPIO_SPEED_FREQ_LOW         (0x00000001U)
#define GPIO_SPEED_FREQ_HIGH        (0x00000002U)
#define GPIO_SPEED_FREQ_VERY_HIGH   (0x00000003U)

// PULLUP OR PULLDOWN // 

#define GPIO_NO_PULL    (0x00000000U)
#define GPIO_PULL_UP    (0x00000001U)
#define GPIO_PULL_DOWN  (0x00000002U)
#define GPIO_RESERVED   (0x00000003U)

// ENSURING PIN CONFIG MEETS SPECIFICATON // 

#define ISPIN(PIN) ((PIN & ~GPIO_PIN_MASK) == 0U || \
                    (PIN & GPIO_PIN_MASK) != 0U)

#define ISMODE(MODE) ((MODE == GPIO_MODE_INPUT)     || \
                      (MODE == GPIO_MODE_OUTPUT_PP) || \
                      (MODE == GPIO_MODE_OUTPUT_OD) || \
                      (MODE == GPIO_MODE_ALT_PP)    || \
                      (MODE == GPIO_MODE_ALT_OD)    || \
                      (MODE == GPIO_MODE_ANA)          \
                      )

#define ISSPEED(SPEED) ((SPEED == GPIO_SPEED_FREQ_VERY_LOW) || \
                        (SPEED == GPIO_SPEED_FREQ_LOW)      || \
                        (SPEED == GPIO_SPEED_FREQ_HIGH)     || \
                        (SPEED == GPIO_SPEED_FREQ_VERY_HIGH)   \
                        )

#define ISPUPD(PULL) ((PULL == GPIO_NO_PULL)   || \
                      (PULL == GPIO_PULL_UP)   || \
                      (PULL == GPIO_PULL_DOWN) || \
                      (PULL == GPIO_RESERVED)     \
                      )


#define GPIO_PIN_MASK       0xFFFFU //used to assert that only a valid pin number is selected if anything more is selected then it will be set to 0

////////// Data Types //////////


// ENUMERATIONS // 

typedef enum{
    RESET_STATE = 0U,
    SET_STATE,
}GpioPinState; 

// STRUCTS //

typedef struct{

    uint32_t pin; //pin number in pin map 

    uint32_t mode; //mode to which this pin is to be set (input, output, alt, analog)

    uint32_t pull; // pull up or pull down

    uint32_t speed;  // speed at which it ticks

    uint32_t alternateFunction; 

}GpioConfig; 





#endif