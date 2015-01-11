KitProg — programmer/debugger for Cyperss PSoC 4 family devices running on generic PSoC 5 device.

Now it is configured to run on CY8C5868LTI-LP032 (more generally speaking, on freeSoC development board),
but you can simply reconfigure it to run on virtually any PSoC 5 device with USB you have on hands.

This source code is intended for Cypress' PSoC Creator IDE, but you can export it and then build with any
supported compiler/IDE you want (raw GCC for example is supported).

This project is heavily simplified, reworked and refactored original KitProg programmer source code, that
you can get for free here: http://www.element14.com/community/thread/27067/l/psoc-5lp-programmer-firmware
Original sources were cut down and modified that way to only serve as programmer and debugger. Other functionality
(I2C and USART bridge, voltage mesurements, etc.) was excluded for simplicity. Other parts like source code
structure was heavily reworked and refactored. Even some communication protocol commands, that do not affect
programming and debugging, was thrown out.

At all, this project is a great preparation for writing firmware for stm32f103 platform, based on ideas from
this source code, that everyone will be able to use to build really cheap (about $4 USD) programmer and debugger
for great Cypress' PSoC 4 family.
