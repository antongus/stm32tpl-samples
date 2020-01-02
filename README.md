stm32tpl-tests
==============

This repository contains sample projects for my [stm32tpl][stm32tpl] library.
All projects also use [scmRTOS][scmrtos] embedded operating system.

To clone the repository with submodules use

`git clone --recursive https://github.com/antongus/stm32tpl-samples.git`

### Prerequisites:
- GCC cross-compiler for ARM Cortex-M controllers (I use [gcc-arm-embedded][toolchain]);
- openocd tool ([openocd][openocd])

### Build and flash instructions:
- enter to a directory of the project you interested;
- type `make` on shell prompt to build the project;
- type `make program` to load program into microcontroller.

Instead of typing commands on shell prompt one can use Eclipse CDT IDE to build project and flash microcontroller.
Just import the Eclipse project and select `build` and `program` from "Build Targets" window.

[scmrtos]: https://github.com/scmrtos/scmrtos
[stm32tpl]: https://github.com/antongus/stm32tpl
[openocd]: http://openocd.org/
[toolchain]: https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads