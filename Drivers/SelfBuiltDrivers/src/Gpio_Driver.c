#include "../inc/Gpio_Driver.h"
#include "assert.h"

// MACROS // 
#define REGISTER_MASK (0xFFFFFFFF)

// we will set up a basic driver with 4 main functions 

/*
1. GPIO_init: initialize peripherals based on specified parameter 
2. GPIO_readPin: return state of inputted pin 
3. GPIO_writePin: set or clear specified pin 
4. GPIO_toggle: invert whatever state the pin is in 
*/

void selfBuilt_gpioInit(GPIO_TypeDef *gpio_port, GpioConfig *config)
{

    // assert all key components to ensure correct configuration inputs. Ensuring minimization of unexpected behaviour
    assert_param(IS_GPIO_ALL_INSTANCE(gpio_port));
    assert_param(ISPIN(config->pin));
    assert_param(ISMODE(config->mode));
    assert_param(ISPUPD(config->pull));
    assert_param(ISSPEED(config->speed));

    // go through all the set pins in config->pin and set them for the desired config 

    uint8_t position = 0U; // specifying this to an unsigned 8 as it can at most be 31
    uint8_t curr_pin = 0;     // specifying to unsigned 8 as it can only be 1 or 0 and unsigned gives us uncertainity protection 
    if(position <= 15U){

        while((config->pin >> position) != 0){  // checks if the current 32 bit holds any more pins to be configured
         
            curr_pin = config->pin & (1U << position); // selects the lowest pin 

            if(curr_pin != 0U){ // if the pin is 0 that means its not been selected to be configured

                // MODE SETTING
                gpio_port->MODER &= (REGISTER_MASK & ~(3U << (position * 2))); // clear bits assocaited to selected pin 3 = 11 ~3 = other bits 00 other bits
                gpio_port->MODER |= (config->mode << (position * 2)); // since we only clears 2 bits and we are ORing with our mode, the Output type will not be carried through
                
                //PULLUP/PULLDOWN SETTING
                gpio_port->PUPDR &= (REGISTER_MASK & ~(3U << (position * 2))); // clear bits assocaited to selected pin 3 = 11 ~3 = other bits 00 other bits
                gpio_port->PUPDR |= (config->pull << (position * 2)); // since we only clears 2 bits and we are ORing with our mode, the Output type will not be carried through

                if(config->mode == GPIO_MODE_ALT_PP || config->mode == GPIO_MODE_ALT_OD){ // checking if the pin is to be configured to an alt function

                    gpio_port->OTYPER &= ((uint16_t)REGISTER_MASK & ~(1U << (position))); // restting bits associated to specified pins only using 16 bits of our mask as the entire type register is not available for our pins (only 1 bit for a pin)
                    gpio_port->OTYPER |= ((config->mode & 0x000F0) << (position));
                    
                    if(position <= 7){
                        gpio_port->AFR[0] &= (REGISTER_MASK & ~(15U << (position * 4))); // reset register
                        gpio_port->AFR[0] |=  ((0x000F & config->alternateFunction) << (position * 4)); // reset register
                    }
                    else{
                        gpio_port->AFR[1] &= (REGISTER_MASK & ~(15U << (position * 4))); // reset register
                        gpio_port->AFR[1] |=  ((0x000F & config->alternateFunction) << (position * 4)); // reset register
                    }
                }

                // OUTPUT TYPE SETTING AND SPEED SETTING 
                if(config->mode == GPIO_MODE_OUTPUT_PP || config->mode == GPIO_MODE_OUTPUT_OD){

                    gpio_port->OTYPER &= ((uint16_t)REGISTER_MASK & ~(1U << (position))); // restting bits associated to specified pins only using 16 bits of our mask as the entire type register is not available for our pins (only 1 bit for a pin)
                    gpio_port->OTYPER |= ((config->mode & 0x000F0) << (position)); 

                    gpio_port->OSPEEDR &= (REGISTER_MASK & ~(3U << (position * 2))); // restting bits associated to specified pins
                    gpio_port->OSPEEDR |= ((config->speed & 0x000F0) << (position * 2)); 
                }
            }
            position++; 
        }
    }
}

void selfBuilt_gpioDeinit(GPIO_TypeDef *gpio_port, uint16_t pin)
{
    // we want to de-initalize a specified pin to its resset state

    /* data sheet tells us 
    MODER : default pin value is analog 11 
    TYPER : default output type is push-pull (0)
    SPEEDR: default value was very low speed... found from stm32 driver 
    PUPDR: default value is no PUPD, 00
    */

   uint16_t position = 0U; 
   uint16_t currPin; 

   if(position <= 15U)
   {
    while(pin >> position != 0U){  

        currPin = (pin << position) & 1U; 

        if(currPin != 0U){

        gpio_port->MODER |= 3U << (position * 2);

        gpio_port->OTYPER &= ~(1U << position); 

        gpio_port->OSPEEDR &= ~(3U << (position * 2)); 

        gpio_port->PUPDR &= ~(3U << (position * 2));

        // learnt this neat trick from HAL.. since position represents the bit we are currently at position >> 3U 
        // can give us which index of the AFR array we want to write to... this is because pos 0 to 7 are at most 3 bits this if we
        // shift it 3U then the output will be 0 where as 8 to 15 is 4 bits thus after the shift outputting 1  
        gpio_port->AFR[position >> 3U] &=  ~(0xFUL << (position & 7UL)); 
        // diregard anything more in position than 7 and shift only by that amount
        }
    }
   }

}

// we only want to read if the pin is set or not set.. This has nothing to do with if it is an input or an output pin.
// It only concerns the voltage IE is the pin driven high or low

GpioPinState selfBuilt_gpioRead(GPIO_TypeDef *gpio_port, uint16_t pin)
{
    assert_param(ISPIN(pin));

    GpioPinState pinState; 

    if(gpio_port->IDR & pin != 0U){ // this will check if the selected pin and the associated bit in the input data register are both set to 1 and if so return that the pin is set as then the output of the & cannot be 0, if not that means the pin isnt set thus the output will be reset 
        pinState = GPIO_PIN_SET; 
    }
    else {
        pinState = GPIO_PIN_RESET;
    }

    return pinState; 
}

void selfBuilt_gpioSet(GPIO_TypeDef *gpio_port, uint16_t pin, GpioPinState state)
{
    assert_param(ISPIN(pin)); 

    uint32_t tempPin = pin; 

    if(gpio_port->MODER & pin == 0U){
        printf("pin is in input mode , needs to be in output mode to be set");
    }

    // now we can write directly to the output data register (ODR), but the controller docs introduce a layer of abstraction 
    // where they allow the ODR bit to be set through the Bit Set Reset Register (BSRR)
    // BSRR is split into BRx -- Top 16 bits and BSx -- bottom 16 bits
    // to reset set coressponding BRR pin to 1 and to set, BSR pin to 1 

    else if(state == GPIO_PIN_SET){
        gpio_port->BSRR != tempPin; // set the pin in BSR 
        gpio_port->BSRR &= ~(tempPin << 16); // unset the pin in BRR  
    }
    else if(state == GPIO_PIN_RESET){
        gpio_port->BSRR != (tempPin << 16); // set the pin in BRR 
        gpio_port->BSRR &= ~tempPin;
    }
}

void selfBuilt_gpioToggle(GPIO_TypeDef *gpio_port, uint16_t pin)
{
    assert_param(ISPIN(pin));

    // for output pins the IDR (input data register) should only read the value that the pin is set to... so its value in the 
    // ODR (output data register) which as we know from the write function can be written to by the BSRR

    GpioPinState currPinState = selfBuilt_gpioRead(gpio_port, pin); 

    if(currPinState == 0U){
        selfBuilt_gpioSet(gpio_port, pin, GPIO_PIN_SET);
    }
    else{
        selfBuilt_gpioSet(gpio_port, pin, GPIO_PIN_RESET);
    }
}
