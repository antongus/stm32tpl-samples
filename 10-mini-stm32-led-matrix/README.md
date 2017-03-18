mini-stm32-led-matrix
=====================

This sample project shows how to interface with chain of [MAX7219][max7219] led drivers.
Project uses [scmRTOS][scmrtos] embedded operating system and [stm32tpl][stm32tpl] library.

### Hardware used

   * mini-stm32 development board from aliexpress
   * LED matrix module from aliexpress (8x32 dots, 4xMAX7219 in chain)
   * st-link for flashing

### Module picture:

![mini-stm32][picture]

[scmrtos]: https://github.com/scmrtos/scmrtos
[stm32tpl]: https://github.com/antongus/stm32tpl
[max7219]: https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf
[picture]: https://cloud.githubusercontent.com/assets/6680984/24076095/f1478d9a-0c4a-11e7-9541-83e7c9c1b097.jpg
