/*
 * operations on IDE disk.
 */

#include "serv.h"
#include <drivers/dev_disk.h>
#include <lib.h>
#include <mmu.h>

// Overview:
//  read data from IDE disk. First issue a read request through
//  disk register and then copy data from disk buffer
//  (512 bytes, a sector) to destination array.
//
// Parameters:
//  diskno: disk number.
//  secno: start sector number.
//  dst: destination for data read from IDE disk.
//  nsecs: the number of sectors to read.
//
// Post-Condition:
//  Panic if any error occurs. (you may want to use 'panic_on')
//
// Hint: Use syscalls to access device registers and buffers.
// Hint: Use the physical address and offsets defined in 'include/drivers/dev_disk.h':
//  'DEV_DISK_ADDRESS', 'DEV_DISK_ID', 'DEV_DISK_OFFSET', 'DEV_DISK_OPERATION_READ',
//  'DEV_DISK_START_OPERATION', 'DEV_DISK_STATUS', 'DEV_DISK_BUFFER'
void ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs) {
	u_int begin = secno * BY2SECT;
	u_int end = begin + nsecs * BY2SECT;
	
	for (u_int off = 0; begin + off < end; off += BY2SECT) {
		uint32_t temp = diskno;
		/* Exercise 5.3: Your code here. (1/2) */
		u_int read = 0;
		u_int offset = begin + off;
		if(syscall_write_dev(&diskno, 0x13000010, 4) < 0) panic_on("ide_read failed to write 0x13000010!");
		if(syscall_write_dev(&offset, 0x13000000, 4) < 0) panic_on("ide_read failed to write 0x13000000!");
		if(syscall_write_dev(&read, 0x13000020, 4) < 0) panic_on("ide_read failed to write 0x13000020!");
		u_int res;
		if(syscall_read_dev(&res, 0x13000030, 4) < 0) panic_on("ide_read failed to read 0x13000030!");
		if(!res) panic_on("ide_read unsuccess");
		if(syscall_read_dev((dst + off), 0x13004000, DEV_DISK_BUFFER_LEN) < 0) panic_on("ide_read failed to read 0x13004000!");
	}
}

// Overview:
//  write data to IDE disk.
//
// Parameters:
//  diskno: disk number.
//  secno: start sector number.
//  src: the source data to write into IDE disk.
//  nsecs: the number of sectors to write.
//
// Post-Condition:
//  Panic if any error occurs.
//
// Hint: Use syscalls to access device registers and buffers.
// Hint: Use the physical address and offsets defined in 'include/drivers/dev_disk.h':
//  'DEV_DISK_ADDRESS', 'DEV_DISK_ID', 'DEV_DISK_OFFSET', 'DEV_DISK_BUFFER',
//  'DEV_DISK_OPERATION_WRITE', 'DEV_DISK_START_OPERATION', 'DEV_DISK_STATUS'
void ide_write(u_int diskno, u_int secno, void *src, u_int nsecs) {
	u_int begin = secno * BY2SECT;
	u_int end = begin + nsecs * BY2SECT;

	for (u_int off = 0; begin + off < end; off += BY2SECT) {
		uint32_t temp = diskno;
		/* Exercise 5.3: Your code here. (2/2) */
		u_int write = 1;
		u_int offset = begin + off;
		if(syscall_write_dev(&diskno, 0x13000010, 4) < 0) panic_on("ide_write failed to write 0x13000010!");
		if(syscall_write_dev(&offset, 0x13000000, 4) < 0) panic_on("ide_write failed to write 0x13000000!");
		if(syscall_write_dev((src+off), 0x13004000, DEV_DISK_BUFFER_LEN) < 0) panic_on("ide_write failed to write 0x1300400!");
		if(syscall_write_dev(&write, 0x13000020, 4) < 0) panic_on("ide_write failed to write 0x13000020!");
		u_int res;
		if(syscall_read_dev(&res, 0x13000030, 4) < 0) panic_on("ide_write failed to read 0x13000030!");
		if(!res) panic_on("ide_write unsuccess");
	}
}
