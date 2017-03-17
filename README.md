stm32tpl-tests
==============

This repository contains sample projects for my [stm32tpl][stm32tpl] library.
All projects also use [scmRTOS][scmrtos] embedded operating system.

To clone project with submodules use

`git clone --recursive https://github.com/antongus/stm32tpl-samples.git`

### Prerequisites:
- GCC cross-compiler for ARM Cortex-M3 controllers (I use [gcc-arm-embedded][toolchain]);
- st-flash tool from [texane/stlink][stlink] on linux, or
- ST-LINK_CLI utility from ST on Windows.

### Build and flash instructions:
- enter to a directory of project you interested;
- type `make` on shell prompt to build;
- `make program` to load program into microcontroller.

Instead of typing commands on shell prompt one can use Eclipse CDT IDE to build project and flash microcontroller.
Just import Eclipse project and select `build` and `program` from "make targets" window.

[scmrtos]: https://github.com/scmrtos/scmrtos
[stm32tpl]: https://github.com/antongus/stm32tpl
[stlink]: https://github.com/texane/stlink
[toolchain]: https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads