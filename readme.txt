============================= 第一次：支持串口  =============================
U-Boot 1.1.6 (Jul 11 2019 - 07:43:45)

DRAM:  64 MB
Flash: 512 kB
*** Warning - bad CRC, using default environment

In:    serial
Out:   serial
Err:   serial
SMDK2440 #

============================= 第二次：增加norflash识别，cfi模式  =============================
U-Boot 1.1.6 huzg-uboot(Jul 12 2019 - 07:57:42)

DRAM:  64 MB
Flash:  2 MB
*** Warning - bad CRC, using default environment

In:    serial
Out:   serial
Err:   serial
SMDK2440 #

============================= 第二次：增加loadx命令，增加DM9000的支持  =============================
U-Boot 1.1.6 huzg-uboot(Jul 14 2019 - 10:14:03)

DRAM:  64 MB
Flash:  2 MB
*** Warning - bad CRC, using default environment

In:    serial
Out:   serial
Err:   serial

SMDK2440 # print
bootdelay=3
baudrate=115200
ethaddr=08:00:3e:26:0a:5b
ipaddr=192.168.2.110
serverip=192.168.2.100
netmask=255.255.255.0
stdin=serial
stdout=serial
stderr=serial

Environment size: 161/65532 bytes

SMDK2440 # ping 192.168.2.123
ERROR: resetting DM9000 -> not responding
dm9000 i/o: 0x20000000, id: 0x90000a46
DM9000: running in 16 bit mode
MAC: 08:00:3e:26:0a:5b
could not establish link
checksum bad
checksum bad
checksum bad
checksum bad
checksum bad
host 192.168.2.123 is alive

============================= 第二次：增加nand命令，nandflash的支持  =============================
U-Boot 1.1.6 huzg-uboot(Jul 14 2019 - 18:03:20)

DRAM:  64 MB
Flash:  2 MB
NAND:  256 MiB
*** Warning - bad CRC, using default environment

In:    serial
Out:   serial
Err:   serial
SMDK2440 # nand
Usage:
nand    - NAND sub-system

============================= 2019/07/14 增加如下内容  =============================
1, nand命令支持yaffs文件读和写
2, 修改默认的配置参数
	一，支持向内核传递内存分布信息
	二，向内核内核传递命令行参数
	三，nand的内部空间分配
	四，自启动命令，启动参数(CONFIG_JFFS2_CMDLINE, CONFIG_JFFS2_NAND, MTDIDS_DEFAULT, MTDPARTS_DEFAULT, CFG_ENV_IS_IN_NAND, CFG_ENV_OFFSET 等)
	五，添加mtdparts的分区功能
	
3, 该修改可以直接正常启动kernel
====================================================================================
U-Boot 1.1.6 huzg-uboot(Jul 16 2019 - 07:11:53)

DRAM:  64 MB
Flash:  2 MB
NAND:  256 MiB
*** Warning - bad CRC or NAND, using default environment

In:    serial
Out:   serial
Err:   serial
Hit any key to stop autoboot:  0
SMDK2440 # print
bootargs=noinitrd root=/dev/mtdblock3 init=/linuxrc console=ttySAC0,115200
bootcmd=nand read.jffs2 0x30007FC0 kernel; bootm 0x30007FC0
bootdelay=3
baudrate=115200
ethaddr=08:00:3e:26:0a:5b
ipaddr=192.168.2.110
serverip=192.168.2.100
netmask=255.255.255.0
stdin=serial
stdout=serial
stderr=serial
mtdids=nand0=nandflash0
mtdparts=mtdparts=nandflash0:256k@0(bootloader),128k(params),2m(kernel),-(root)
partition=nand0,0
mtddevnum=0
mtddevname=bootloader

Environment size: 452/131068 bytes
SMDK2440 # mtdparts

device nand0 <nandflash0>, # parts = 4
 #: name                        size            offset          mask_flags
 0: bootloader          0x00040000      0x00000000      0
 1: params              0x00020000      0x00040000      0
 2: kernel              0x00200000      0x00060000      0
 3: root                0x0fda0000      0x00260000      0

active partition: nand0,0 - (bootloader) 0x00040000 @ 0x00000000

defaults:
mtdids  : nand0=nandflash0
mtdparts: mtdparts=nandflash0:256k@0(bootloader),128k(params),2m(kernel),-(root)
SMDK2440 # bootm
## Booting image at 33000000 ...
Bad Magic Number
SMDK2440 # boot

NAND read: device 0 offset 0x60000, size 0x200000

Bad block at 0xa0000 in erase block from 0xa0000 will be skipped
Reading data from 0x27f800 -- 100% complete.
 2097152 bytes read: OK
## Booting image at 30007fc0 ...
   Image Name:   Linux-2.6.22.6
   Created:      2018-05-15  11:57:26 UTC
   Image Type:   ARM Linux Kernel Image (uncompressed)
   Data Size:    1848728 Bytes =  1.8 MB
   Load Address: 30008000
   Entry Point:  30008000


============================= 2019/07/xx 增加如下内容  =============================


