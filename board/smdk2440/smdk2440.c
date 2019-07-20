/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * David Mueller, ELSOFT AG, <d.mueller@elsoft.ch>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <s3c2410.h>

DECLARE_GLOBAL_DATA_PTR;

#define FCLK_SPEED 1

#if 0
#if FCLK_SPEED==0		/* Fout = 203MHz, Fin = 12MHz for Audio */
#define M_MDIV	0xC3
#define M_PDIV	0x4
#define M_SDIV	0x1
#elif FCLK_SPEED==1		/* Fout = 202.8MHz */
#define M_MDIV	0xA1
#define M_PDIV	0x3
#define M_SDIV	0x1
#endif

#define USB_CLOCK 1

#if USB_CLOCK==0
#define U_M_MDIV	0xA1
#define U_M_PDIV	0x3
#define U_M_SDIV	0x1
#elif USB_CLOCK==1
#define U_M_MDIV	0x48
#define U_M_PDIV	0x3
#define U_M_SDIV	0x2
#endif
#endif

#define S3C2440_MPLL_400MHZ			((0x5c<<12)|(0x01<<4)|(0x01))
#define S3C2440_UPLL_48MHZ			((0x38<<12)|(0x02<<4)|(0x02))
#define S3C2440_CLKDIV				0x05

static inline void delay (unsigned long loops)
{
	__asm__ volatile ("1:\n"
	  "subs %0, %1, #1\n"
	  "bne 1b":"=r" (loops):"0" (loops));
}

/*
 * Miscellaneous platform dependent initialisations
 */

int board_init (void)
{
	S3C24X0_CLOCK_POWER * const clk_power = S3C24X0_GetBase_CLOCK_POWER();
	S3C24X0_GPIO * const gpio = S3C24X0_GetBase_GPIO();

	/* set up the I/O ports */
	gpio->GPACON = 0x007FFFFF;												//配置GPIOA(0000 0000 0111 1111 1111 1111 1111 1111)
	gpio->GPBCON = 0x00044555;
	gpio->GPBUP = 0x000007FF;
	gpio->GPCCON = 0xAAAAAAAA;
	gpio->GPCUP = 0x0000FFFF;
	gpio->GPDCON = 0xAAAAAAAA;
	gpio->GPDUP = 0x0000FFFF;
	gpio->GPECON = 0xAAAAAAAA;
	gpio->GPEUP = 0x0000FFFF;
	gpio->GPFCON = 0x000055AA;
	gpio->GPFUP = 0x000000FF;
	gpio->GPGCON = 0xFF95FFBA;
	gpio->GPGUP = 0x0000FFFF;
	gpio->GPHCON = 0x002AFAAA;
	gpio->GPHUP = 0x000007FF;

	clk_power->CLKDIVN = S3C2440_CLKDIV;

	__asm__( "mrc	p15, 0, r1, c1, c0, 0\n"
			 "orr	r1, r1, #0xc0000000\n"   //R1_nF:OR:R1_iA
			 "mcr	p15, 0, r1, c1, c0, 0\n"
			 :::"r1"
			 );

	/* to reduce PLL lock time, adjust the LOCKTIME register */
	clk_power->LOCKTIME = 0xFFFFFF;											//设置lockTime, locktime时间内，CPU停止工作，完成后，HCLK会切换到我们设置的时钟

	/* configure MPLL */
	//clk_power->MPLLCON = ((M_MDIV << 12) + (M_PDIV << 4) + M_SDIV);			//设置CPU时钟FCLK(FCLK:HCLK:PCLK:1:2:4)
	clk_power->MPLLCON = S3C2440_MPLL_400MHZ;

	/* some delay between MPLL and UPLL */
	delay (4000);

	/* configure UPLL */
	//clk_power->UPLLCON = ((U_M_MDIV << 12) + (U_M_PDIV << 4) + U_M_SDIV);	//设置USB时钟频率
	clk_power->UPLLCON = S3C2440_UPLL_48MHZ;

	/* some delay between MPLL and UPLL */
	delay (8000);

	
	/* arch number of SMDK2410-Board */
	gd->bd->bi_arch_number = MACH_TYPE_S3C2440;

	/* adress of boot parameters */
	gd->bd->bi_boot_params = 0x30000100;

	icache_enable();
	dcache_enable();
	
	return 0;
}

int dram_init (void)
{
	// sdram的起始地址
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	// sdram的大小
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;

	return 0;
}
#if 0
ulong board_flash_get_legacy(ulong base, int banknum, flash_info_t *info)
{	
	info->portwidth = CFG_FLASH_CFI_WIDTH;
	info->chipwidth = CFG_FLASH_CFI_WIDTH;
	info->interface = FLASH_CFI_X16;
	return 1;
}
#endif

