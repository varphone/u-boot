
setenv my_env_ver   "Info: Linux-4.9.88  MYIMXA7  eMMC"
setenv my_env_flag  "set"

setenv image        "zImage-myimx6a7"
setenv mmcroot      "/dev/mmcblk1p2"
setenv panel        "MY-WVGA"

setenv update_ubot  'if tftpboot ${loadaddr} ${file_ubot}; then '\
'mmc dev ${mmcdev} 0; mmc write ${loadaddr} 0x2 0x3fe; fi;'
setenv update_dtb   'if tftpboot ${loadaddr} ${fdt_file}; then '\
'fatwrite mmc ${mmcdev}:${mmcpart} ${loadaddr} ${fdt_file} ${filesize}; fi;'
setenv update_kern  'if tftpboot ${loadaddr} ${image}; then '\
'fatwrite mmc ${mmcdev}:${mmcpart} ${loadaddr} ${image} ${filesize}; fi;'
setenv update_scr   'if tftpboot ${loadaddr} ${file_scr}; then '\
'fatwrite mmc ${mmcdev}:${mmcpart} ${loadaddr} ${file_scr} ${filesize}; source ${loadaddr}; saveenv; fi;'

setenv bootargs_mmc 'setenv bootargs console=${console},${baudrate} root=${mmcroot} rootwait rw'
setenv bootargs_net 'setenv bootargs console=${console},${baudrate} root=/dev/nfs ip=dhcp nfsroot=${serverip}:${nfsroot},v3,tcp'

setenv load_kern    'fatload mmc ${mmcdev}:${mmcpart} ${kernel_addr_r} ${image}'
setenv load_dtb     'fatload mmc ${mmcdev}:${mmcpart} ${fdt_addr_r} ${fdt_file}'

setenv bootcmd_mmc  'run bootargs_mmc; run load_dtb; run load_kern; run bootzcmd;'
setenv bootcmd_nfs  'run bootargs_net; run load_dtb; run load_kern; run bootzcmd;'

setenv bootzcmd    'bootz ${kernel_addr_r} - ${fdt_addr_r}'
setenv bootcmd_my  'echo $my_env_ver; run bootcmd_mmc'
