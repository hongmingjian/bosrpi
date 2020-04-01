#include <stdint.h>

#include "cpu.h"

static void init_uart(uint32_t baud)
{
    aux_reg_t *aux = (aux_reg_t *)(MMIO_BASE_PA+AUX_REG);
    gpio_reg_t *gpio = (gpio_reg_t *)(MMIO_BASE_PA+GPIO_REG);

    aux->enables = 1; //Enable Mini UART
    aux->mu_cntl = 0; //Disable transmitter & receiver
    aux->mu_lcr = 3;  //8 data bits
    aux->mu_baud = (SYS_CLOCK_FREQ/(8*baud))-1;

    uint32_t ra=gpio->gpfsel1;
    ra &= ~((7 << 12) | (7 << 15)); // gpio14/gpio15
    ra |= (2 << 12) | (2 << 15);    // alt5=TxD1/RxD1
    gpio->gpfsel1 = ra;

	gpio->gppud = 0; //Disable pull-up/down
    ra = 150; while(ra--) __asm__ __volatile__("nop");
    gpio->gppudclk0 = (1 << 14) | (1 << 15);
    ra = 150; while(ra--) __asm__ __volatile__("nop");
    gpio->gppudclk0 = 0;

    aux->mu_cntl = 3; //Enable transmitter & receiver
}

void sys_putchar ( int c )
{
  aux_reg_t *aux = (aux_reg_t *)(MMIO_BASE_PA+AUX_REG);
  while(1) {
    if(aux->mu_lsr&0x20/*transmitter empty*/)
      break;
  }
  aux->mu_io = c & 0xff;
}

int sys_haschar()
{
    aux_reg_t *aux = (aux_reg_t *)(MMIO_BASE_PA+AUX_REG);
    return aux->mu_lsr&0x1;
}

int sys_getchar()
{
    aux_reg_t *aux = (aux_reg_t *)(MMIO_BASE_PA+AUX_REG);
    while(1) {
        if(aux->mu_lsr&0x1)
            break;
    }
    return aux->mu_io & 0xff;
}

void puts(char *s)
{
  while(*s != 0) {
    sys_putchar(*s);
    s++;
  }
}

__attribute__((noreturn))
void cstart(void)
{
	char *where = (char *)LOADADDR;
	init_uart(115200);

	puts("** Boot over serial for Raspberry PI\r\n");
	puts("** Copyright (C) 2005,2020 Hong MingJian<hongmingjian@gmail.com>\r\n");
	puts("** All rights reserved.\r\n\r\n");

	puts("** Build at " __DATE__ " " __TIME__ "\r\n\r\n");

	puts("Please send uuencoded kernel.img via Xmodem ...\r\n");

	while(sys_haschar())
		sys_getchar();

	if(xmodem(where) > 0) {
		uudecode(where);
		((void (*)(void))where)();
	}

	puts("Failed to boot!\r\n");

	while(1)
		;
}
