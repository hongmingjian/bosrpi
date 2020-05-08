#define XMODEM_SOH 0x01
#define XMODEM_EOT 0x04
#define XMODEM_ACK 0x06
#define XMODEM_NAK 0x15
#define XMODEM_CAN 0x18
#define XMODEM_DATA_SIZE 128

struct frame
{
  char   blknum;
  char   blknumcompl;
  char   data[XMODEM_DATA_SIZE];
  char   cksum;
};

static char
cksum(struct frame *f)
{
  int i;
  int sum = 0;
  for(i = 0; i < XMODEM_DATA_SIZE; i++) {
    sum += f->data[i];
  }
  return (sum & 0xff);
}

static void
get_frame(struct frame *rframe)
{
  int i;
  rframe->blknum = sys_getchar();
  rframe->blknumcompl = sys_getchar();
  for(i=0; i < XMODEM_DATA_SIZE; i++) {
      rframe->data[i] = sys_getchar();
    }
  rframe->cksum = sys_getchar();
}

int
xmodem(unsigned char *p)
{
  struct frame aframe;
  int i, len = 0;

  do {
		int loop = 900000; /*XXX*/
		sys_putchar(XMODEM_NAK);

 		while(--loop && (!sys_haschar()))
			;

		if(loop)
			break;

  } while(1);

  while(1) {
      switch(sys_getchar()) {
      case XMODEM_SOH:
				get_frame(&aframe);

				/*
				 * XXX - It seems that SecureCRT can only send ASCII chars.
				 *       Don't verify block number and checksum.
				 */
				/*if((aframe.blknumcompl == 255 - aframe.blknum) &&
				   (cksum(&aframe) == aframe.cksum))*/ {
				    len += XMODEM_DATA_SIZE;

				    for(i = 0; i < XMODEM_DATA_SIZE; i++)
				      *p++ = aframe.data[i];

				    sys_putchar(XMODEM_ACK);
				}/* else {
				    sys_putchar(XMODEM_NAK);
				}*/
				break;

      case XMODEM_EOT:
				sys_putchar(XMODEM_ACK);
				return len;

      case XMODEM_CAN:
				return -1;
      }
  }

  return -1;
}
