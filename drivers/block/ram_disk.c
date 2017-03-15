#include <common.h>
#include <malloc.h>
#include <part.h>
#include <ata.h>
#include <asm/io.h>
#include <ram_disk.h>

/* Start address of the ram disk */
static unsigned long ram_disk_start = RAM_DISK_START;
/* Bytes limit of the ram disk */
static unsigned long ram_disk_size = RAM_DISK_SIZE;

static unsigned long ram_disk_sector = RAM_DISK_SECTOR_SIZE;
static unsigned int ram_disk_mode = RAM_DISK_MODE_RDONLY;

static block_dev_desc_t ram_disk_dev = {
	.if_type = IF_TYPE_ATAPI,
	.part_type = PART_TYPE_UNKNOWN,
	.type = DEV_TYPE_HARDDISK,
	.lba = RAM_DISK_SIZE / RAM_DISK_SECTOR_SIZE,
#ifdef CONFIG_LBA48
	.lba48 = RAM_DISK_SIZE / RAM_DISK_SECTOR_SIZE,
#endif
	.blksz = RAM_DISK_SECTOR_SIZE,
	.priv = NULL };

block_dev_desc_t *ram_disk_get_dev(int dev)
{
	return ((block_dev_desc_t *) &ram_disk_dev);
}

unsigned long ram_disk_block_read (int dev, unsigned long start,
				   lbaint_t blkcnt, void *buffer)
{
	if ((start + blkcnt) * ram_disk_dev.blksz > ram_disk_size)
		return 0;
	memcpy(buffer, ram_disk_start + (start * ram_disk_dev.blksz),
		blkcnt * ram_disk_dev.blksz);
	return blkcnt;
}

unsigned long ram_disk_block_write (int dev, unsigned long start,
				    lbaint_t blkcnt, const void *buffer)
{
	return 0;
}

unsigned int ram_disk_init(int dev, unsigned long start, unsigned long size,
			   unsigned long sector, unsigned int mode)
{
	unsigned int err;

	ram_disk_start = start;
	ram_disk_size = size;
	ram_disk_sector = sector;
	ram_disk_mode = mode;

#ifdef CONFIG_LBA48
	ram_disk_dev.lba48 = size / sector;
#else
	ram_disk_dev.lba = size / sector;
#endif
	ram_disk_dev.blksz = ram_disk_sector;
	ram_disk_dev.block_read = ram_disk_block_read;
	ram_disk_dev.block_write = ram_disk_block_write;

	init_part(&ram_disk_dev);

	dev_print(&ram_disk_dev);

	return err;
}
