#ifndef RAM_DISK_H
#define RAM_DISK_H

#ifdef CONFIG_RAM_DISK_START
#define RAM_DISK_START		CONFIG_RAM_DISK_START
#else
#define RAM_DISK_START		0x88000000
#endif

#ifdef CONFIG_RAM_DISK_SIZE
#define RAM_DISK_SIZE		CONFIG_RAM_DISK_SIZE
#else
#define RAM_DISK_SIZE		0x01000000
#endif

#ifdef CONFIG_RAM_DISK_SECTOR_SIZE
#define RAM_DISK_SECTOR_SIZE	CONFIG_RAM_DISK_SECTOR_SIZE
#else
#define RAM_DISK_SECTOR_SIZE	512
#endif

#define RAM_DISK_MODE		RAM_DISK_MODE_RDONLY
#define RAM_DISK_MODE_RDONLY	0
#define RAM_DISK_MODE_RDWR	1

unsigned int ram_disk_init(int dev, unsigned long start, unsigned long size,
			   unsigned long sector, unsigned int mode);

#endif /* RAM_DISK_H */
