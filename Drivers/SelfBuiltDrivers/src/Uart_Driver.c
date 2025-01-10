#include "../inc/Uart_Driver.h"

void uartInit(void){

}
void uartWrite(uint8_t data){

}
uint8_t uartRead(void){
    uint32_t baudRate;      // rate at which data bits will be sent. Even though this is an Asynchronous communication (meaning no clk line), 
                            // the transmitter and reciever still need to agree on the same baud rate so the bits can be sampled properly
    uint8_t wordLength;     // even though word length can be 7 to9 bits since this is just there to represent the size of word length then 
                            // represented in an 8-bit integer easily, there is no need to waste memory on a 32bit int 
    uint8_t stopBits;       // 1 is default 2 is for USART normal mode, single-wire and modem mode 1.5 is for smartmode 

    uint8_t
}
bool uartIsDataAvailable(void){

}