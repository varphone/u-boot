
setenv my_env_ver   "Info: Linux-4.9.88  MYIMXA7  Nand "
setenv my_env_flag  "set"

setenv image        "zImage-myimx6a7"
setenv file_ubi     "rootfs.ubi"
setenv panel        "MY-WVGA"

setenv update_rtfs  'if tftpboot ${loadaddr} ${file_ubi}; then '\
'nand erase 0xf00000 0xf100000; ubi part rootfs; ubi create rootfs; ubi write ${loadaddr} rootfs ${filesize}; fi;'
setenv update_dtb   'if tftpboot ${loadaddr} ${fdt_file}; then '\
'nand erase 0xe00000 0x100000; nand write ${loadaddr} 0xe00000 ${filesize}; fi;'
setenv update_kern  'if tftpboot ${loadaddr} ${image}; then '\
'nand erase 0x400000 0xa00000; nand write ${loadaddr} 0x400000 ${filesize}; fi;'
setenv update_scr   'if tftpboot ${loadaddr} ${file_scr}; then '\
'nand erase 0x3c0000 0x20000; nand write ${loadaddr} 0x3c0000 ${filesize}; fi;'

setenv bootargs_nad 'setenv bootargs console=${console},${baudrate} root=ubi0:rootfs rootfstype=ubifs ubi.mtd=3 ${mtdparts}'
setenv bootargs_net 'setenv bootargs console=${console},${baudrate} root=/dev/nfs ip=dhcp nfsroot=${serverip}:${nfsroot},v3,tcp'

setenv load_kern    'nand read ${kernel_addr_r} 0x400000 0xa00000'
setenv load_dtb     'nand read ${fdt_addr_r} 0xe00000 0x100000'

setenv bootcmd_nad  'run bootargs_nad; run load_dtb; run load_kern; run bootzcmd;'
setenv bootcmd_nfs  'run bootargs_net; run load_dtb; run load_kern; run bootzcmd;'

setenv bootzcmd    'bootz ${kernel_addr_r} - ${fdt_addr_r};'
setenv bootcmd_my  'echo $my_env_ver; run bootcmd_nad'
