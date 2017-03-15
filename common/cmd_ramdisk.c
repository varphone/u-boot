/*
 * (C) Copyright 2009 mGine co.
 * unsik Kim <donari75@gmail.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <command.h>
#include <ram_disk.h>

int do_ram_disk_cmd (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	u32 start = RAM_DISK_START;
	u32 size = RAM_DISK_SIZE;
	u32 sector = RAM_DISK_SECTOR_SIZE;
	u32 mode = RAM_DISK_MODE;

	switch (argc) {
	case 1:
		/* use defaults */
		break;
	case 2:
		start = simple_strtoul(argv[1], NULL, 0);
		break;
	case 3:
		start = simple_strtoul(argv[1], NULL, 0);
		size = simple_strtoul(argv[2], NULL, 0);
		break;
	case 4:
		start = simple_strtoul(argv[1], NULL, 0);
		size = simple_strtoul(argv[2], NULL, 0);
		sector = simple_strtoul(argv[3], NULL, 0);
		break;
	case 5:
		start = simple_strtoul(argv[1], NULL, 0);
		size = simple_strtoul(argv[2], NULL, 0);
		sector = simple_strtoul(argv[3], NULL, 0);
		if (strcmp(argv[4], "rw") == 0)
			mode = RAM_DISK_MODE_RDWR;
		break;
	default:
		cmd_usage(cmdtp);
		return 1;
	}

	printf("Ram Disk: 0x%x 0x%x %u %u\n", start, size, sector, mode);

	ram_disk_init(0, start, size, sector, mode);

	return 0;
}

U_BOOT_CMD(
	rmd,	5,	0,	do_ram_disk_cmd,
	"rmd <start> <size> [sector] [ro|rw] - ram disk command\n",
	": simulate disk on memory command\n"
	"    - start : memory start address\n"
	"    - size : ram disk size in bytes\n"
	"    - sector : sector size, default [512]\n"
	"    - ro|rw : read only or writable access mode, default [ro]"
);
