#ifndef _CPU_H
#define _CPU_H

#include "config.h"

#define LOADADDR        0x8000
#define SYS_CLOCK_FREQ	250000000UL

extern uint32_t cpuid;
#define CPUID_BCM2835 0x410FB767
#define CPUID_BCM2836 0x410FC075
#define CPUID_BCM2837 0x410FD034

extern uint32_t MMIO_BASE_PA;

#define AUX_REG (0x215000)
typedef struct {
	volatile unsigned int irq;
	volatile unsigned int enables;
	volatile unsigned int UNUSED1[14];
	volatile unsigned int mu_io;
	volatile unsigned int mu_ier;
	volatile unsigned int mu_iir;
	volatile unsigned int mu_lcr;
	volatile unsigned int mu_mcr;
	volatile unsigned int mu_lsr;
	volatile unsigned int mu_msr;
	volatile unsigned int mu_scratch;
	volatile unsigned int mu_cntl;
	volatile unsigned int mu_stat;
	volatile unsigned int mu_baud;
	volatile unsigned int UNUSED2[5];
	volatile unsigned int spi0_cntl0;
	volatile unsigned int spi0_cntl1;
	volatile unsigned int spi0_stat;
	volatile unsigned int UNUSED3[1];
	volatile unsigned int spi0_io;
	volatile unsigned int spi0_peek;
	volatile unsigned int UNUSED4[10];
	volatile unsigned int spi1_cntl0;
	volatile unsigned int spi1_cntl1;
	volatile unsigned int spi1_stat;
	volatile unsigned int UNUSED5[1];
	volatile unsigned int spi1_io;
	volatile unsigned int spi1_peek;
} aux_reg_t;

#define GPIO_REG (0x200000)
typedef struct {
	volatile unsigned int gpfsel0;
	volatile unsigned int gpfsel1;
	volatile unsigned int gpfsel2;
	volatile unsigned int gpfsel3;
	volatile unsigned int gpfsel4;
	volatile unsigned int gpfsel5;
	volatile unsigned int reserved1;
	volatile unsigned int gpset0;
	volatile unsigned int gpset1;
	volatile unsigned int reserved2;
	volatile unsigned int gpclr0;
	volatile unsigned int gpclr1;
	volatile unsigned int reserved3;
	volatile unsigned int gplev0;
	volatile unsigned int gplev1;
	volatile unsigned int reserved4;
	volatile unsigned int gpeds0;
	volatile unsigned int gpeds1;
	volatile unsigned int reserved5;
	volatile unsigned int gpren0;
	volatile unsigned int gpren1;
	volatile unsigned int reserved6;
	volatile unsigned int gpfen0;
	volatile unsigned int gpfen1;
	volatile unsigned int reserved7;
	volatile unsigned int gphen0;
	volatile unsigned int gphen1;
	volatile unsigned int reserved8;
	volatile unsigned int gplen0;
	volatile unsigned int gplen1;
	volatile unsigned int reserved9;
	volatile unsigned int gparen0;
	volatile unsigned int gparen1;
	volatile unsigned int reserved10;
	volatile unsigned int gpafen0;
	volatile unsigned int gpafen1;
	volatile unsigned int reserved11;
	volatile unsigned int gppud;
	volatile unsigned int gppudclk0;
	volatile unsigned int gppudclk1;
} gpio_reg_t;

#define SYSTIMER_REG (0x3000)
typedef struct {
	volatile unsigned int cs;
	volatile unsigned int clo;
	volatile unsigned int chi;
	volatile unsigned int c0;
	volatile unsigned int c1;
	volatile unsigned int c2;
	volatile unsigned int c3;
} systimer_reg_t;

#define EMMC_REG (0x300000)
typedef struct {
	volatile unsigned int arg2;
	volatile unsigned int blksizecnt;
	volatile unsigned int arg1;
	volatile unsigned int cmdtm;
	volatile unsigned int resp0;
	volatile unsigned int resp1;
	volatile unsigned int resp2;
	volatile unsigned int resp3;
	volatile unsigned int data;
	volatile unsigned int status;
	volatile unsigned int control0;
	volatile unsigned int control1;
	volatile unsigned int interrupt;
	volatile unsigned int irpt_mask;
	volatile unsigned int irpt_en;
	volatile unsigned int control2;
	volatile unsigned int capabilities_0;
	volatile unsigned int capabilities_1;
	volatile unsigned int UNUSED1[2];
	volatile unsigned int force_irpt;
	volatile unsigned int UNUSED2[7];
	volatile unsigned int boot_timeout;
	volatile unsigned int dbg_sel;
	volatile unsigned int UNUSED3[2];
	volatile unsigned int exrdfifo_cfg;
	volatile unsigned int exrdfifo_en;
	volatile unsigned int tune_step;
	volatile unsigned int tune_steps_std;
	volatile unsigned int tune_steps_ddr;
	volatile unsigned int UNUSED4[23];
	volatile unsigned int spi_int_spt;
	volatile unsigned int UNUSED5[2];
	volatile unsigned int slotisr_ver;
} emmc_reg_t;

#endif /* _CPU_H */
