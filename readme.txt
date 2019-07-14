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



