# u-boot for Lichee pi Nano 

Goal
=
1. Manually patch the bug (patch file done by thirtythreeforty, but I already done it for you <3)
2. Add support for the Winbond W25N01GV, I took a portion of the code from u-boot main repo to driver/mtd/nand/spi/winbond.c
3. Add support for LS027B7DH01, Sharp SPI Memory LCD

Guide (For NAND flash)
=

0. install swig python-dev python3-dev libncurse5-dev zlib1g-dev

1. make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- licheepi_nano_spinand_defconfig

2. make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- -j4
