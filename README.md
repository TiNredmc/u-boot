# u-boot for Lichee pi Nano 

Goal
=
1. Manually patch the bug (patch file done by thirtythreeforty, but I already done it for you <3)
2. Add support for the Winbond W25N01G, I took a portion of the code from u-boot main repo to driver/mtd/nand/spi/winbond.c
3. Made the (by far) the best building instruction that will make you see absolutely 0 error

Guide (For stock NOR flash)
=
0. install swig python-dev python3-dev libncurse5-dev zlib1g-dev

1. make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- licheepi_nano_spiflash_defconfig

2. make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- menuconfig

3. Go to Device Drivers -> [Tick This] SPI Support -> [Tick THis] Enable Driver Model for SPI drivers
    -> [Tick This] Allwinner SoC SPI controllers
    
4. Go back to Device Drivers -> MTD Support -> SPI Flash Support -> [Tick This] Enable Driver Model for SPI flash

5. Exit menuconfig and save changes

6[PATCHED YOU CAN SKIP]. add "dtb-$(CONFIG_MACH_SUNIV) += suniv-f1c100s-licheepi-nano.dtb" to arch/arm/dts/Makefile
   and comment out the in the file arch/arm/dts/sunxi-u-boot.dtsi
```
 	aliases {
 		mmc1 = &mmc2;
 	};
```

7. make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- -j4

Guide (For NAND flash)
=

0. install swig python-dev python3-dev libncurse5-dev zlib1g-dev

1. make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- licheepi_nano_spinand_defconfig

2. make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- -j4
