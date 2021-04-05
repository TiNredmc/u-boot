/*
 * Configuration settings for new Allwinner F-series (suniv) CPU
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * Include common sunxi configuration where most the settings are
 * ; setenv mtdids 'spi-nand0=spi0.0' ; setenv mtdparts 'spi-nand0:512k(uboot)ro,128k(dtb)ro,4096K(kernel)ro,-@0x4A0000(rootfs)'
 */

#define CONFIG_BOOTCOMMAND \
"mtd list  ;" \  
"mtd read.raw spi-nand0 0x80C00000 0x80000 0x2800 ; mtd read.raw spi-nand0 0x80008000 0xa0000 0x400000 ; bootz 0x80008000 - 0x80C00000" 
			  
#define CONFIG_BOOTARGS \
"console=ttyS0,115200 panic=5 earlyprintk mtdids:spi-nand0=nand0.0 mtdparts:mtdparts=nand0.0:512k(uboot)ro,128k(dtb)ro,4096K(kernel)ro,-@0x4A0000(rootfs) rootwait root=/dev/mtdblock3 rw rootfstype=jffs2"

#include <configs/sunxi-common.h>



#endif /* __CONFIG_H */
