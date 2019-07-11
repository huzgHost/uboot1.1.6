/*
 * (C) Copyright 2001-2004
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2002
 * David Mueller, ELSOFT AG, d.mueller@elsoft.ch
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

/* This code should work for both the S3C2400 and the S3C2410
 * as they seem to have the same PLL and clock machinery inside.
 * The different address mapping is handled by the s3c24xx.h files below.
 */

#include <common.h>
#if defined(CONFIG_S3C2400) || defined (CONFIG_S3C2410) || defined (CONFIG_TRAB)

#if defined(CONFIG_S3C2400)
#include <s3c2400.h>
#elif defined(CONFIG_S3C2410)
#include <s3c2410.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

#define MPLL 0
#define UPLL 1

/* ------------------------------------------------------------------------- */
/* NOTE: This describes the proper use of this file.
 *
 * CONFIG_SYS_CLK_FREQ should be defined as the input frequency of the PLL.
 *
 * get_FCLK(), get_HCLK(), get_PCLK() and get_UCLK() return the clock of
 * the specified bus in HZ.
 */
/* ------------------------------------------------------------------------- */

static ulong get_PLLCLK(int pllreg)
{
    S3C24X0_CLOCK_POWER * const clk_power = S3C24X0_GetBase_CLOCK_POWER();
    ulong r, m, p, s;

    if (pllreg == MPLL)
		r = clk_power->MPLLCON;										//clk_power->MPLLCON的赋值在board_init()初如化函数实现
    else if (pllreg == UPLL)
		r = clk_power->UPLLCON;										//clk_power->UPLLCON的赋值在board_init()初如化函数实现
    else
		hang();

    m = ((r & 0xFF000) >> 12) + 8;								//计算出 m (MDIV+8)
    p = ((r & 0x003F0) >> 4) + 2;								//计算出 p (PDIV+2)
    s = r & 0x3;												//计算出 s (SDIV)

	//这是算出 2410的PLLCLK，我们是2440 ，这里需要修改
    //return((CONFIG_SYS_CLK_FREQ * m) / (p << s));				//得到 MPLLCON 或 UPLLCON，得到系统运行时钟

    return ((CONFIG_SYS_CLK_FREQ * m * 2) / (p << s));			//得到s3c2440的时钟
}

/* return FCLK frequency */
ulong get_FCLK(void)
{
    return(get_PLLCLK(MPLL));
}

/* return HCLK frequency */
ulong get_HCLK(void)
{
    S3C24X0_CLOCK_POWER * const clk_power = S3C24X0_GetBase_CLOCK_POWER();

    //return((clk_power->CLKDIVN & 0x2) ? get_FCLK()/2 : get_FCLK());	//是否为系统时钟的1/2

	return ((clk_power->CLKDIVN & 0x4) ? get_FCLK()/4 : get_FCLK());
}

/* return PCLK frequency */
ulong get_PCLK(void)
{
    S3C24X0_CLOCK_POWER * const clk_power = S3C24X0_GetBase_CLOCK_POWER();

    //return((clk_power->CLKDIVN & 0x1) ? get_HCLK()/2 : get_HCLK());//是否为HCLK的1/2

	return((clk_power->CLKDIVN & 0x1) ? get_HCLK()/2 : get_HCLK());//是否为HCLK的1/2
}

/* return UCLK frequency */
ulong get_UCLK(void)
{
    return(get_PLLCLK(UPLL));
}

#endif /* defined(CONFIG_S3C2400) || defined (CONFIG_S3C2410) || defined (CONFIG_TRAB) */
