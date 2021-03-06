#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "cpu.h"
#include "dosfs.h"

uint32_t cpuid;
uint32_t MMIO_BASE_PA;

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

/**
 * From https://github.com/dwelch67
 */
void puth ( unsigned int d )
{
    unsigned int ra = 0;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1) {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc || ra) {
			if(rc>9) rc+=0x37; else rc+=0x30;
			sys_putchar(rc);
			ra = 1;
        }
        if(rb==0) {
			if(ra == 0)
				sys_putchar('0');
			break;
		}
    }
}

void cstart(void)
{
    unsigned char *where = (unsigned char *)LOADADDR;
	uint32_t core;

    if(cpuid == CPUID_BCM2835)
		MMIO_BASE_PA = 0x20000000;
	else {
		__asm__ __volatile__("mrc p15, #0, %0, c0, c0, #5":"=r"(core));
		MMIO_BASE_PA = 0x3f000000;
	}

    init_uart(115200);
	
    puts("** Boot over Serial for Raspberry Pi\r\n");
    puts("** Built at " __DATE__ " " __TIME__ "\r\n");
	puts("** CPU");(cpuid==CPUID_BCM2835)?NULL:puth(core&3);
	puts(": 0x");puth(cpuid);
	puts("\r\n");

    VOLINFO volinfo;
    unsigned char scratch[512];
    int sdosfs = 0;

    if(0 == sdInitCard()) {
        uint32_t pstart;
        if((-1 != (pstart = DFS_GetPtnStart(0, scratch, 0, NULL, NULL, NULL))) &&
           (DFS_OK == DFS_GetVolInfo(0, scratch, pstart, &volinfo)))
            sdosfs = 1;
    }

    if(!sdosfs)
        puts("!! WARNING: cannot save files to FAT on SD card!\r\n");

    do {
        puts(">> Send a uuencoded file using Xmodem ...\r\n");

        while(sys_haschar())
            sys_getchar();

        if(xmodem(where) > 0) {
            char filename[16];
            int size = uudecode(where, filename);
            if(strncmp(filename, "kernel.img", 10) == 0) {
                break;
            } else {
                if(sdosfs) {
                    FILEINFO fi;
                    uint32_t nbyte = 0;
                    if((DFS_OK == DFS_OpenFile(&volinfo, filename, DFS_WRITE, scratch, &fi)) &&
                       (DFS_OK == DFS_WriteFile(&fi, scratch, where, &nbyte, size)) &&
                       (nbyte == size)) {
                        puts(">> Saved to ");
                        puts(filename);
                        puts(".\r\n");
                    } else {
                        puts(">> Failed to save ");
                        puts(filename);
                        puts(".\r\n");
                    }
                    DFS_Close(&fi);
                } else {
                    puts(">> ");
                    puts(filename);
                    puts(" discarded.\r\n");
                }
            }
        } else {
            puts(">> Failed to receive\r\n");
        }
    } while(1);
}
