=================================== nandflash 未添加分区造成的错误 ==============================================
U-Boot 1.1.6 huzg-uboot(Jul 15 2019 - 06:49:40)

DRAM:  64 MB
Flash:  2 MB
NAND:  256 MiB
*** Warning - bad CRC, using default environment

In:    serial
Out:   serial
Err:   serial
Hit any key to stop autoboot:  0

NAND read: 'kernel' is not a number
## Booting image at 30007fc0 ...
Bad Magic Number
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

Environment size: 296/131068 bytes
SMDK2440 #
===================================================================================================================

===================================nandflash 未添加分区造成的错误 =================================================
U-Boot 1.1.6 huzg-uboot(Jul 15 2019 - 08:01:01)

DRAM:  64 MB
Flash:  2 MB
NAND:  256 MiB
*** Warning - bad CRC, using default environment

In:    serial
Out:   serial
Err:   serial
Hit any key to stop autoboot:  0

NAND read: mtdparts variable not set, see 'help mtdparts'
incorrect device type in kernel
'kernel' is not a number
## Booting image at 30007fc0 ...
Bad Magic Number
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

Environment size: 320/131068 bytes
===================================================================================================================