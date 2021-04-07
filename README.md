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

Useful info
=

1. There's a file named "README_img_addr.txt" this txt file contain U-boot, DTB, Kernel and root file system memory address / NAND offset / and the image size (plus some extra space in order to align with 128K block erase)

2. the f1c100s_uboot_spinand.sh from aodzip is the script that will convert the fresh-compiled u-boot into the actual bootable image.
