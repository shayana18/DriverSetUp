#ifndef UART_H

#define UART_H

/* Include Files -----------------------------------------------------*/
#include "../../STM32C0xx_HAL_Driver/Inc/stm32c0xx_hal_def.h"
#include <stdint.h>

/**
 * Structs and Unions
 */

/**
 * @brief: Uart Init structure definition.
 */

typedef struct
{
    uint32_t baudRate;      // rate at which data bits will be sent. Even though this is an Asynchronous communication (meaning no clk line), 
                            // the transmitter and reciever still need to agree on the same baud rate so the bits can be sampled properly
    uint8_t wordLength;     // even though word length can be 7 to 9 bits since this is just there to represent the size of word length then 
                            // represented in an 8-bit integer easily, there is no need to waste memory on a 32bit int 
    uint8_t stopBits;       // 1 is default 2 is for USART normal mode, single-wire and modem mode 1.5 is for smartmode 

    uint8_t parity;         // Even (0) or odd partity (1), there for error detection... if even parity and 4 bits set, parity bit will be 0 but if odd parity then it will be 1.
                            // reciever can check to see if parity bit matches what the transmitter says it set it to
    uint8_t mode;           // recieve or transmit mode

    uint8_t overSampling;   // 16 or 8 bit sampling

    uint32_t clkPrescaler  

} UartInitConfig;



/**
 * Function Prototypes
 */

#


#endif