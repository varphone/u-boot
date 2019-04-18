
setenv my_env_ver   "Info: Linux-4.9.88  MYIMXA9  eMMC"
setenv my_env_flag  "set"

setenv image        "zImage-myimx6a9"
setenv mmcroot      "/dev/mmcblk3p2"

setenv disp_fb0_lvds1   "video=mxcfb0:dev=ldb,if=RGB666 ldb=sin1"
setenv disp_fb1_lvds1   "video=mxcfb1:dev=ldb,if=RGB666 ldb=sin1"
setenv disp_fb0_lvds0   "video=mxcfb0:dev=ldb,if=RGB666 ldb=sin0"
setenv disp_fb1_lvds0   "video=mxcfb1:dev=ldb,if=RGB666 ldb=sin0"
setenv disp_fb0_hdmi    "video=mxcfb0:dev=hdmi,1920x1080M@60,if=RGB24"
setenv disp_fb1_hdmi    "video=mxcfb1:dev=hdmi,1920x1080M@60,if=RGB24"
setenv disp_fb0_lcd     "video=mxcfb0:dev=lcd,SEIKO-WVGA,if=RGB24"
setenv disp_fb1_lcd     "video=mxcfb1:dev=lcd,SEIKO-WVGA,if=RGB24"
setenv disp_lvds_dul    "video=mxcfb0:dev=ldb,if=RGB666 ldb=dul1"
setenv disp_lvds_sep    "video=mxcfb0:dev=ldb,if=RGB666 ldb=sep1"

setenv display      "${disp_fb0_lvds1}"
setenv set_disp     'setenv bootargs_disp ${display}'

setenv update_scr   'if tftpboot ${loadaddr} ${file_scr}; then '\
'fatwrite mmc ${mmcdev}:${mmcpart} ${loadaddr} ${file_scr} ${filesize}; source ${loadaddr}; saveenv; fi;'
setenv update_ubot  'if tftpboot ${loadaddr} ${file_ubot}; then '\
'sf probe; sf erase 0 0x200000; sf write ${loadaddr} 0x400 ${filesize}; fi;'
setenv update_dtb   'if tftpboot ${loadaddr} ${fdt_file}; then '\
'fatwrite mmc ${mmcdev}:${mmcpart} ${loadaddr} ${fdt_file} ${filesize}; fi;'
setenv update_kern  'if tftpboot ${loadaddr} ${image}; then '\
'fatwrite mmc ${mmcdev}:${mmcpart} ${loadaddr} ${image} ${filesize}; fi;'

setenv bootargs_mmc 'if run set_disp; then '\
'setenv bootargs console=${console},${baudrate} ${smp} root=${mmcroot} rootwait rw ${bootargs_disp}; fi;'
setenv bootargs_net 'if run set_disp; then '\
'setenv bootargs console=${console},${baudrate} ${smp} root=/dev/nfs ip=dhcp nfsroot=${serverip}:${nfsroot},v3,tcp; fi;'

setenv load_kern   'fatload mmc ${mmcdev}:${mmcpart} ${kernel_addr_r} ${image}'
setenv load_dtb    'fatload mmc ${mmcdev}:${mmcpart} ${fdt_addr_r} ${fdt_file}'

setenv bootcmd_mmc 'run bootargs_mmc; run load_dtb; run load_kern; run bootzcmd;'
setenv bootcmd_nfs 'run bootargs_net; run load_dtb; run load_kern; run bootzcmd;'
setenv bootzcmd    'bootz ${kernel_addr_r} - ${fdt_addr_r};'
setenv bootcmd_my  'echo $my_env_ver; run bootcmd_mmc'
