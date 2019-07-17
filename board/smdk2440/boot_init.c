#include <common.h>
#include <s3c2410.h>

#define BUSY            1

#define NAND_SECTOR_SIZE    512
#define NAND_BLOCK_MASK     (NAND_SECTOR_SIZE - 1)

#define NAND_SECTOR_SIZE_LP    2048
#define NAND_BLOCK_MASK_LP     (NAND_SECTOR_SIZE_LP - 1)

static void nand_chip_select(void) {
	int i;
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;

    s3c2440nand->NFCONT &= ~(1<<1);
    for(i=0; i<10; i++);    
}

static void nand_chip_deselect(void) {
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;

    s3c2440nand->NFCONT |= 1<<1;
}

static void nand_write_cmd(int cmd) {
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;

	//NFCMD是8位，cmd是64位 
	volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFCMD;
	*p = cmd;
}

static void nand_write_addr(unsigned int addr) {
	int i;
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
	volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFADDR;

	*p = addr & 0xff;
	for(i=0; i<10; i++);
    *p = (addr >> 9) & 0xff;
    for(i=0; i<10; i++);
    *p = (addr >> 17) & 0xff;
    for(i=0; i<10; i++);
    *p = (addr >> 25) & 0xff;
    for(i=0; i<10; i++);
	
}

/* 发出地址 */
static void nand_write_addr_lp(unsigned int addr){
	int i;
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
	volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFADDR;
	int col, page;

	col = addr & NAND_BLOCK_MASK_LP;
	page = addr / NAND_SECTOR_SIZE_LP;

	*p = col & 0xff;			/* Column Address A0~A7 */
    for(i=0; i<10; i++);		
    *p = (col >> 8) & 0x0f;		/* Column Address A8~A11 */
    for(i=0; i<10; i++);
    *p = page & 0xff;			/* Row Address A12~A19 */
    for(i=0; i<10; i++);
    *p = (page >> 8) & 0xff;	/* Row Address A20~A27 */
    for(i=0; i<10; i++);
    *p = (page >> 16) & 0x03;	/* Row Address A28~A29 */
    for(i=0; i<10; i++);
}

static unsigned char nand_read_data(void) {
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
	volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFDATA;

	return *p;
}

static void nand_wait_idle(void) {
	 int i;
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
    volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFSTAT;

    while(!(*p & BUSY))
        for(i=0; i<10; i++);
}

static void nand_reset(void) {
	//选中
	nand_chip_select();
	//发送命令
	nand_write_cmd(0xff);
	//等待命令是否发送成功
	nand_wait_idle();
	//不选中
	nand_chip_deselect();
}

/* 初始化NAND Flash */
void nand_init_ll(void)
{
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
	
#define TACLS   0
#define TWRPH0  1
#define TWRPH1  0

    /* Set flash memory timing */
    s3c2440nand->NFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4);
    /* Initialize ECC, enable chip select, NAND flash controller enable */
    s3c2440nand->NFCONT = (1<<4)|(0<<1)|(1<<0);

	/* 复位NAND Flash */
	nand_reset();
}

int bBootFromNorFlash(void) {
	volatile unsigned int *p = (volatile unsigned int *)0;
	unsigned int val;

	val = *p;
	*p = 0x12345678;
	if(*p != 0x12345678) {
		return 1;
	} else {
		*p = val;
		return 0;
	}
}

/* 读函数 */
void nand_read_ll(unsigned char *buf, unsigned long start_addr, int size) {
	int i, j;
    
    if ((start_addr & NAND_BLOCK_MASK) || (size & NAND_BLOCK_MASK)) {
        return ;    /* 地址或长度不对齐 */
    }

    /* 选中芯片 */
    nand_chip_select();

    for(i=start_addr; i < (start_addr + size);) {
      /* 发出READ0命令 */
      nand_write_cmd(0);

      /* Write Address */
      nand_write_addr(i);
	  nand_write_cmd(0x30);
      nand_wait_idle();

      for(j=0; j < NAND_SECTOR_SIZE; j++, i++) {
          *buf = nand_read_data();
          buf++;
      }
    }

    /* 取消片选信号 */
    nand_chip_deselect();
    
    return ;
}

/* 读函数 
  * Large Page
  */
void nand_read_ll_lp(unsigned char *buf, unsigned long start_addr, int size) {
	 int i, j;
    
    if ((start_addr & NAND_BLOCK_MASK_LP) || (size & NAND_BLOCK_MASK_LP)) {
        return ;    /* 地址或长度不对齐 */
    }

    /* 选中芯片 */
    nand_chip_select();
	for(i = start_addr; i < (start_addr + size);) {
		/* 发出READ0命令 */
		nand_write_cmd(0);
		
		/* Write Address */
      	nand_write_addr_lp(i);
		
		nand_write_cmd(0x30);
		nand_wait_idle();

		for(j = 0; j < NAND_SECTOR_SIZE_LP; j++, i++) {
			*buf = nand_read_data();
			buf++;
		}
	}
	

	/* 取消片选信号 */
    nand_chip_deselect();

	return;
}

int CopyCode2Ram(unsigned long start_addr, unsigned char *buf, int size) {
	unsigned int *pdwDest;
	unsigned int *pdwSrc;
	int i;

	if(bBootFromNorFlash()){
		pdwSrc = (unsigned int *)start_addr;
		pdwDest = (unsigned int *)buf;

		for(i = 0; i < size; i++) {
			pdwDest[i] = pdwSrc[i];
		}

		return 0;
	} else {
		/* 初始化NAND Flash */
		nand_init_ll();
		/* 从 NAND Flash启动 */
        nand_read_ll_lp(buf, start_addr, (size + NAND_BLOCK_MASK_LP)&~(NAND_BLOCK_MASK_LP));

		return 0;
	}
}

