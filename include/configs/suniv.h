/*
 * Configuration settings for new Allwinner F-series (suniv) CPU
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * Include common sunxi configuration where most the settings are
 */
#include <configs/sunxi-common.h>

//#define CONFIG_BOOTCOMMAND "mtd list; mtd read.raw spi-nand0 0x80000000 0x0 0x62000"

#endif /* __CONFIG_H */
