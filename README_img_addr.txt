Image     RAM-Addr       NAND-ADDR      SIZE
=================================================
U-boot    0x80000000     0              0x7D000
Device3   0x80C00000     0x80000        0x2800
zImage    0x80008000     0xA0000        0x400000
rootfs    0x?		 0x4A0000	0x7C0000 (will automatically expand via mtdparts partitioning)