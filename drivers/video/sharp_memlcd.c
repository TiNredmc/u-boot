// SPDX-License-Identifier: GPL-2.0
/*
 * Sharp Memory Display Driver.
 *
 * In this case I'm testing with LS027B7DH01
 * But it should work with others by changing 
 * W x H Display resolution
 *
 * Copyright (c) 2020 Thipok Jiamjarapan
 * <thipok17@gmail.com> 
 *
 */
#include <common.h>
#include <dm.h>
#include <errno.h>
#include <spi.h>
#include <display.h>
#include <pwm.h>
#include <asm/io.h>
#include <asm/gpio.h>

//LSB First
#define MLCD_UD 0x80 // Display update command
#define MLCD_MC 0x20 // Dislay internal mem clear
#define MLCD_DM 0x00 // Dummy byte (sending after data update)

#ifndef CONFIG_SHARP_MLCD_W
#define CONFIG_SHARP_MLCD_W 240
#endif

#ifndef CONFIG_SHARP_MLCD_H
#define CONFIG_SHARP_MLCD_H 400
#endif

#define MLCD_RSIZE CONFIG_SHARP_MLCD_W / 8
#define MLCD_BUFSIZE MLCD_RSIZE * CONFIG_SHARP_MLCD_H

#ifndef CONFIG_SHARP_LCD_BUS
#define CONFIG_SHARP_LCD_BUS 1
#endif

#ifndef CONFIG_SHARP_LCD_CS
#define CONFIG_SHARP_LCD_CS 0
#endif


//VCOM inverter control, Required for display refreshing
#define EXTCOM_HI 0x40
#define EXTCOM_LO 0x00


struct memlcd_priv {
	struct udevice *dev;
	struct udevice *pwm;
	struct spi_slave *spi;
	struct gpio_desc scs; // scs pin for sharp display (Ignore hardware NSS)
	u32 max_freq; // SPI Max frequency (must be 1-2Mhz for display to sample serial data
	uint16_t pwm_period_ns;
	uint32_t pwm_channel;
};

static uint8_t memlcd_msb2lsb(uint8_t* msb){
	*msb = (*msb & 0xF0) >> 4 | (*msb & 0x0F) << 4 ;
	*msb = (*msb & 0xCC) >> 4 | (*msb & 0x33) << 4 ;
	*msb = (*msb & 0xAA) >> 4 | (*msb & 0x55) << 4 ;
	
	return *msb;
}

static void memlcd_pwm_on(struct udevice *dev){
	struct memlcd_priv *priv = dev_get_priv(dev);
	int ret;

	ret = pwm_set_config(priv->pwm, priv->pwm_channel, priv->pwm_period_ns, 5);
	if(ret) {
		debug("%s: memlcd PWM set config failed: %d\n",__func__,ret);
		return -1;
	}

	ret = pwm_set_enable(priv->pwm, priv->pwm_channel, true);
	if(ret) {
		debug("%s: memlcd PWM enable failed: %d\n",__func__,ret);
		return -1;
	}

	return ret;
}


static int memlcd_sendBlock(struct memlcd_priv *priv, uint8_t* val[]){
	unsigned long flags = SPI_XFER_BEGIN;
	int ret = 0;
	uint8_t bitLen = sizeof(*val) * 8;

	flags |= SPI_XFER_END;
	dm_gpio_set_value(&priv->scs, 1);
	ret = spi_xfer(priv->spi, bitLen, &val, NULL, flags);

	if (ret) debug("%s: memlcd transmit error: %d\n",__func__,ret);

	dm_gpio_set_value(&priv->scs, 0);
	return ret;
}

/* TODO */
static int memlcd_read_timing(struct udevice *dev){
	return 0;
};

static int memlcd_ofdata_to_platdata(struct udevie *dev){
	struct memlcd_priv *priv = dev_get_priv(dev);
	struct ofnode_phandle_args args;
	int ret,len;

	//GPIO for SCS pin
	ret = gpio_request_by_name(dev, "SCS-Pin", 0, &priv->scs,
				   GPIOD_IS_OUT);
	if (ret) {
		debug("%s: Warning: cannot get enable GPIO: ret=%d\n", __func__, ret);
		if (ret != -ENOENT)
			return log_ret(ret);
	}

	//SPI 
	priv->max_freq = dev_read_u32_default(dev, "spi-max-frequency", &len);

	//PWM0 setup
	ret = dev_read_phandle_with_args(dev, "memlcd-pwm", "#pwm-cells", 0, 0, &args);	
	if(ret) {
		debug("%s: memlcd unable to obtain PWM phandle: %d\n",__func__,ret);
		return -1;
	}

	ret = uclass_get_device_by_ofnode(UCLASS_PWM, args.node, &priv->pwm);
	if(ret) {
		debug("%s: memlcd unable to obtain PWM phandle: %d\n",__func__,ret);
		return -1;
	}

	if (args.args_count < 2){
		debug("Not enough arguments to pwm\n");
		return -1;
	}
	priv->pwm_channel = args.args[0];
	priv->pwm_period_ns = args.args[1];

	return 0;
}


static int memlcd_enable(struct udevice *dev){
	struct memlcd_priv *priv = dev_get_priv(dev);
	int ret;

	// Backlight is PWM, in this case it's EXTCOMIN signal. Fixed at 20Hz
	memlcd_pwm_on(dev);

	// Display mem clear
  	memlcd_sendBlock(priv, (unsigned int *)MLCD_MC);

	return 0;
 
}

static int memlcd_probe(struct udevice *dev){
	struct memlcd_priv *priv = dev_get_priv(dev);
	priv->spi = dev_get_parent_priv(dev);
	int ret;

	//SPI MODE 0
	priv->spi = spi_setup_slave(CONFIG_SHARP_LCD_BUS, CONFIG_SHARP_LCD_CS, priv->max_freq, 0);
	
	if(!priv->spi){
		debug("%s: memlcd SPI setup failed\n",__func__);
		return -1;
	}

	ret = spi_claim_bus(priv->spi);
	if (ret){
		debug("%s: memlcd unable to claim SPI bus: %d\n",__func__,ret);
		spi_free_slave(priv->spi);
		return -1;
	}
	

	return 0;

}


/* TODO */
static int memlcd_bind(struct udevice *dev){
	return 0;
}

static const struct dm_display_ops sharp_memlcd_ops = {
	.enable = memlcd_enable, 
	.read_timing = memlcd_read_timing,
};

static const struct udevice_id sharp_memlcd_ids[] = {
	{ .compatible = "sharp,sharp-memlcd" },
	{ }
};

U_BOOT_DRIVER(sharp_memlcd) = {
	.name	= "sharp_memlcd",
	.id	= UCLASS_DISPLAY,
	.of_match = sharp_memlcd_ids,
	.ops	= &sharp_memlcd_ops,
	.probe	= memlcd_probe,
	.bind	= memlcd_bind,
	.ofdata_to_platdata = memlcd_ofdata_to_platdata,
	.priv_auto_alloc_size = sizeof(struct memlcd_priv),
};



