//NAME: Feilan Wang
//EMAIL: wangfeilan@hotmail.com
//ID: 104796844

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#include "ext2_fs.h"

#define ANALYSIS_SUCCESSFUL 0
#define BAD_ARGUMENT  1
#define PROCESSING_ERRORS	2

int fd = -1;

typedef struct ext2_super_block sb;

typedef struct {
	__u32	s_blocks_count;				/* Blocks count */
	__u32	s_inodes_count;				/* Inodes count */
	__u32	s_block_size;				/* Block size */
	__u32	s_inode_size;				/* Inode size */
	__u32	s_blocks_per_group;			/* Blocks per group */
	__u32	s_inodes_per_group;			/* Inodes per group */
	__u32	s_first_ino;				/* First non-reserved inode */
	__u32 	s_groups_count;				/* Groups count */
} sb_data;
sb_data mySuperBlock;


typedef struct ext2_group_desc g;

typedef struct {
	__u32 g_blocks_count;				/* Blocks count */
	__u32 g_inodes_count;				/* Inodes count */
	__u32 g_free_blocks_count;			/* Free blocks */
	__u32 g_free_inodes_count;			/* Free inodes */
	__u32 g_first_free_block_bitmap;	/* Frist free block bitmap */
	__u32 g_first_free_inode_bitmap;	/* Frist free inode bitmap */
	__u32 g_first_block_of_inodes;		/* Frist block of inodes */
} g_data;
g_data myGroup;


/* Superblock */
/*
1. SUPERBLOCK
2. total number of blocks (decimal)
3. total number of i-nodes (decimal)
4. block size (in bytes, decimal)
5. i-node size (in bytes, decimal)
6. blocks per group (decimal)
7. i-nodes per group (decimal)
8. first non-reserved i-node (decimal)
*/

void superblock() {
	/* Create message buffer */
	sb* superBlock = malloc(sizeof(sb));
	if (superBlock == NULL) {
		fprintf(stderr, "Error: malloc.\n");
		exit(PROCESSING_ERRORS);
	}

	/* Read into buffer */
	int ret;
	ret = pread(fd, superBlock, sizeof(sb), 1024);	/* "The primary copy of the superblock is stored at an offset of 1024 bytes from the start of the device." */
	if (ret < 0) {
		fprintf(stderr, "Error: pread in superblock().\n");
		free(superBlock);
		exit(PROCESSING_ERRORS);
	}

	/* Read blocks count */
	mySuperBlock.s_blocks_count = superBlock->s_blocks_count;
	/* Read inodes count */
	mySuperBlock.s_inodes_count = superBlock->s_inodes_count;
	/* Read block size */
	mySuperBlock.s_block_size = EXT2_MIN_BLOCK_SIZE << superBlock->s_log_block_size;
	/* Read inode size */
	mySuperBlock.s_inode_size = superBlock->s_inode_size;
	/* Read blocks per group */
	mySuperBlock.s_blocks_per_group = superBlock->s_blocks_per_group;
	/* Read inodes per group */
	mySuperBlock.s_inodes_per_group = superBlock->s_inodes_per_group;
	/* Read first non reserved inode */
	mySuperBlock.s_first_ino = superBlock->s_first_ino;

	/* Open csv file */
	FILE* file = fopen("summary.csv", "a");	//"a": append mode
	if (file == NULL) {
		fprintf(stderr, "Error: fopen in superblock().\n");
		free(superBlock);
		exit(PROCESSING_ERRORS);
	}

	/* Write to file */
	/* %u: unsigned decimal value (unsigned int) */
	printf("%s,%u,%u,%u,%u,%u,%u,%u\n", 
			"SUPERBLOCK", 
			mySuperBlock.s_blocks_count, 
			mySuperBlock.s_inodes_count, 
			mySuperBlock.s_block_size, 
			mySuperBlock.s_inode_size, 
			mySuperBlock.s_blocks_per_group, 
			mySuperBlock.s_inodes_per_group, 
			mySuperBlock.s_first_ino);

	/* Clean up */
	free(superBlock);
	fclose(file);
}

/* Group */
/*
1. GROUP
2. group number (decimal, starting from zero)
3. total number of blocks in this group (decimal)
4. total number of i-nodes in this group (decimal) 
5. number of free blocks (decimal)
6. number of free i-nodes (decimal)
7. block number of free block bitmap for this group (decimal)
8. block number of free i-node bitmap for this group (decimal)
9. block number of first block of i-nodes in this group (decimal)
*/

void group() {
	/* Create message buffer */
	g* group = malloc(sizeof(g));
	if (group == NULL) {
		fprintf(stderr, "Error: malloc.\n");
		exit(PROCESSING_ERRORS);
	}

	/* Read into buffer */
	int ret;
	ret = pread(fd, group, sizeof(g), 1024 + sizeof(sb));
	if (ret < 0) {
		fprintf(stderr, "Error: pread in group().\n");
		free(group);
		exit(PROCESSING_ERRORS);
	}

	myGroup.g_blocks_count = mySuperBlock.s_blocks_count;
	myGroup.g_inodes_count = mySuperBlock.s_inodes_count;
	myGroup.g_free_blocks_count = group->bg_free_blocks_count;
	myGroup.g_free_inodes_count = group->bg_free_inodes_count;
	myGroup.g_first_free_block_bitmap = group->bg_block_bitmap;
	myGroup.g_first_free_inode_bitmap = group->bg_inode_bitmap;
	myGroup.g_first_block_of_inodes = group->bg_inode_table;

	/* Open csv file */
	FILE* file = fopen("summary.csv", "a");	//"a": append mode
	if (file == NULL) {
		fprintf(stderr, "Error: fopen in superblock().\n");
		free(group);
		exit(PROCESSING_ERRORS);
	}

	/* Write to file */
	/* %u: unsigned decimal value (unsigned int) */
	printf("%s,%d,%u,%u,%u,%u,%u,%u,%u\n", 
			"GROUP", 
			0,
			myGroup.g_blocks_count,
			myGroup.g_inodes_count,
			myGroup.g_free_blocks_count,
			myGroup.g_free_inodes_count,
			myGroup.g_first_free_block_bitmap,
			myGroup.g_first_free_inode_bitmap,
			myGroup.g_first_block_of_inodes);

	/* Clean up */
	free(group);
	fclose(file);
}

/* Free block entires (for each group) */
/*
1. BFREE
2. number of the free blocks (decimal)
*/

void free_block_entries() {
	/* Open csv file */
	FILE* file = fopen("summary.csv", "a");	//"a": append mode
	if (file == NULL) {
		fprintf(stderr, "Error: fopen in free_block_entries().\n");
		exit(PROCESSING_ERRORS);
	}

	unsigned int i;
	for (i = 0; i < mySuperBlock.s_blocks_count; i++) {
		char buffer;
		int ret;
		ret = pread(fd, &buffer, 1, myGroup.g_first_free_block_bitmap * mySuperBlock.s_block_size + (i >> 3));
		if (ret < 0) {
			fprintf(stderr, "Error: pread.\n");
			fclose(file);
			exit(PROCESSING_ERRORS);
		}
		if ((buffer >> (i & 7) & 1) == 0) {
			printf("%s,%u\n", "BFREE", i + 1);
		}
	}
	fclose(file);
}

/* Free inode entires (for each group) */
/*
1. IFREE
2. number of the free inodes (decimal)
*/

void free_inode_entries() {
	/* Open csv file */
	FILE* file = fopen("summary.csv", "a");	//"a": append mode
	if (file == NULL) {
		fprintf(stderr, "Error: fopen in free_inode_entries().\n");
		exit(PROCESSING_ERRORS);
	}

	unsigned int i;
	for (i = 0; i < mySuperBlock.s_inodes_count; i++) {
		char buffer;
		int ret;
		ret = pread(fd, &buffer, 1, myGroup.g_first_free_inode_bitmap * mySuperBlock.s_block_size + (i >> 3));
		if (ret < 0) {
			fprintf(stderr, "Error: pread.\n");
			fclose(file);
			exit(PROCESSING_ERRORS);
		}
		if ((buffer >> (i & 7) & 1) == 0) {
			printf("%s,%u\n", "IFREE", i + 1);
		}
	}
	fclose(file);
}


/* 3 Levels of indirect block process functions */
void indirect_block(int level, __u32 i_block, __u32 offset, __u32 curr_number_of_inodes, struct ext2_inode inode_buffer) {
	/* Open csv file */
	FILE* file = fopen("summary.csv", "a");	//"a": append mode
	if (file == NULL) {
		fprintf(stderr, "Error: fopen in double_indirect_block().\n");
		exit(PROCESSING_ERRORS);
	}

	if (curr_number_of_inodes == 0) return;
	__u32* ptrs;
	ptrs = malloc(sizeof(__u32) * (mySuperBlock.s_block_size / 4));
	pread(fd, ptrs, mySuperBlock.s_block_size, mySuperBlock.s_block_size * i_block);
	unsigned int i;
	for (i = 0; i < (mySuperBlock.s_block_size / 4); i++) {
		__u32 ptr = ptrs[i];
		if (ptr == 0) continue;
		printf("%s,%u,%d,%u,%u,%u\n", "INDIRECT", curr_number_of_inodes, level, offset + i, i_block, ptr);
		if (level > 1) {
			indirect_block(level-1, ptr, offset + i, curr_number_of_inodes, inode_buffer);
		}
	}
	fclose(file);
	free(ptrs);
}


/*get block number function*/
__u32 get_block_number(int* level, __u32* block, __u32 global_block_number) {
	__u32 local_block_number_0 = 0;
	__u32 local_block_number_1 = 0;
	__u32 local_block_number_2 = 0;
	__u32 local_block_number_3 = 0;	//at most triple indirect

	__u32 offset_0 = 12;
	__u32 offset_1 = offset_0 + mySuperBlock.s_block_size;
	__u32 offset_2 = offset_1 + mySuperBlock.s_block_size * mySuperBlock.s_block_size;
	__u32 offset_3 = offset_2 + mySuperBlock.s_block_size * mySuperBlock.s_block_size * mySuperBlock.s_block_size;

	//Determine level
	if (global_block_number > offset_2 && global_block_number <= offset_3) *level = 3;
	else if (global_block_number > offset_1 && global_block_number <= offset_2) *level = 2;
	else if (global_block_number > offset_0 && global_block_number <= offset_1) *level = 1;
	else if (global_block_number > 0 && global_block_number <= offset_0) *level = 0;
	else {
		fprintf(stderr, "Error: wrong global_block_number.\n");
		return (__u32)0;
	}

	//case: direct
	if (*level == 0) {
		local_block_number_0 = global_block_number;
		return block[local_block_number_0 - 1];
	}

	//case: indirect
	if (*level == 1) {
		__u32* indirect_block;
		indirect_block = malloc(mySuperBlock.s_block_size);
		local_block_number_1 = global_block_number - offset_0;
		local_block_number_0 = offset_0 + 1;
		int ret;
		ret = pread(fd, indirect_block, mySuperBlock.s_block_size, block[local_block_number_0 - 1] * mySuperBlock.s_block_size);
		if (ret < 0) {
			fprintf(stderr, "Error: pread in block_number().\n");
			free(indirect_block);
			exit(PROCESSING_ERRORS);
		}
		return indirect_block[local_block_number_1 - 1];
	}

	//case: double_indirect
	if (*level == 2) {
		__u32* double_indirect_block;
		double_indirect_block = malloc(mySuperBlock.s_block_size);
		__u32* indirect_block;
		indirect_block = malloc(mySuperBlock.s_block_size);
		local_block_number_0 = offset_0 + 2;
		local_block_number_1 = (global_block_number - offset_1 - 1) / mySuperBlock.s_block_size + 1;
		local_block_number_2 = (global_block_number - offset_1 - 1) % mySuperBlock.s_block_size + 1;
		int ret;
		ret = pread(fd, indirect_block, mySuperBlock.s_block_size, block[local_block_number_0 - 1] * mySuperBlock.s_block_size);
		if (ret < 0) {
			fprintf(stderr, "Error: pread in block_number().\n");
			free(indirect_block);
			free(double_indirect_block);
			exit(PROCESSING_ERRORS);
		}
		ret = pread(fd, double_indirect_block, mySuperBlock.s_block_size, indirect_block[local_block_number_1 - 1] * mySuperBlock.s_block_size);
		if (ret < 0) {
			fprintf(stderr, "Error: pread in block_number().\n");
			free(indirect_block);
			free(double_indirect_block);
			exit(PROCESSING_ERRORS);
		}
		return double_indirect_block[local_block_number_2 - 1];
	}

	//case: triple-indirect
	if (*level == 3) {
		__u32* triple_indirect_block;
		triple_indirect_block = malloc(mySuperBlock.s_block_size);
		__u32* double_indirect_block;
		double_indirect_block = malloc(mySuperBlock.s_block_size);
		__u32* indirect_block;
		indirect_block = malloc(mySuperBlock.s_block_size);
		__u32 number_of_blocks = global_block_number - offset_2 - 1;
		local_block_number_0 = offset_0 + 3;
		local_block_number_1 = number_of_blocks / (mySuperBlock.s_block_size * mySuperBlock.s_block_size) + 1;
		local_block_number_2 = number_of_blocks % (mySuperBlock.s_block_size * mySuperBlock.s_block_size) / mySuperBlock.s_block_size + 1;
		local_block_number_3 = number_of_blocks % mySuperBlock.s_block_size + 1;
		int ret;
		ret = pread(fd, indirect_block, mySuperBlock.s_block_size, block[local_block_number_0 - 1] * mySuperBlock.s_block_size);
		if (ret < 0) {
			fprintf(stderr, "Error: pread in block_number().\n");
			free(indirect_block);
			free(double_indirect_block);
			free(triple_indirect_block);
			exit(PROCESSING_ERRORS);
		}
		ret = pread(fd, double_indirect_block, mySuperBlock.s_block_size, block[local_block_number_1 - 2] * mySuperBlock.s_block_size);
		if (ret < 0) {
			fprintf(stderr, "Error: pread in block_number().\n");
			free(indirect_block);
			free(double_indirect_block);
			free(triple_indirect_block);
			exit(PROCESSING_ERRORS);
		}
		ret = pread(fd, triple_indirect_block, mySuperBlock.s_block_size, block[local_block_number_2 - 1] * mySuperBlock.s_block_size);
		if (ret < 0) {
			fprintf(stderr, "Error: pread in block_number().\n");
			free(indirect_block);
			free(double_indirect_block);
			free(triple_indirect_block);
			exit(PROCESSING_ERRORS);
		}
		return triple_indirect_block[local_block_number_3 - 1];
	}
	return (__u32)0;
}



/* I-node Summary */

// 1. INODE
// 2. inode number (decimal)
// 3. file type ('f' for file, 'd' for directory, 's' for symbolic link, "?" for anything else)
// 4. mode (low order 12-bits, octal... suggested format "%o")
// 5. owner (decimal)
// 6. group (decimal)
// 7. link count (decimal)
// 8. time of last I-node change (mm/dd/yy hh:mm:ss, GMT)
// 9. modification time (mm/dd/yy hh:mm:ss, GMT)
// 10. time of last access (mm/dd/yy hh:mm:ss, GMT)
// 11. file size (decimal)
// 12. number of (512 byte) blocks of disk space (decimal) taken up by this file

void inode() {
	/* Open csv file */
	FILE* file = fopen("summary.csv", "a");	//"a": append mode
	if (file == NULL) {
		fprintf(stderr, "Error: fopen in free_block_entries().\n");
		exit(PROCESSING_ERRORS);
	}

	unsigned int i;
	for (i = 0; i < mySuperBlock.s_inodes_per_group; i++) {
		struct ext2_inode inode_buffer;
		int ret;
		ret = pread(fd, &inode_buffer, sizeof(inode_buffer), myGroup.g_first_block_of_inodes * mySuperBlock.s_block_size + i * sizeof(struct ext2_inode));
		if (ret < 0) {
			fprintf(stderr, "Error: pread.\n");
			fclose(file);
			exit(PROCESSING_ERRORS);
		}
		if (inode_buffer.i_mode == 0 || inode_buffer.i_links_count == 0) {
			continue;
		}
		//EXT2_S_IFREG	0x8000	regular file
		//EXT2_S_IFDIR	0x4000	directory
		//EXT2_S_IFLNK	0xA000	symbolic link
		char file_type;
		if (inode_buffer.i_mode & 0x8000) {
			file_type = 'f';
		}
		else if (inode_buffer.i_mode & 0x4000) {
			file_type = 'd';
		}
		else if (inode_buffer.i_mode & 0xA000) {
			file_type = 's';
		}
		else {
			file_type = '?';
		}

		//time
		char  c_time[20], m_time[20], a_time[20];

		time_t i_ctime = inode_buffer.i_ctime;				//8
		struct tm* ctime;
		ctime = gmtime(&i_ctime);
		strftime(c_time, sizeof(c_time), "%m/%d/%y %H:%M:%S", ctime);

		time_t i_mtime = inode_buffer.i_mtime;				//9
		struct tm* mtime;
		mtime = gmtime(&i_mtime);
		strftime(m_time, sizeof(m_time), "%m/%d/%y %H:%M:%S", mtime);

		time_t i_atime = inode_buffer.i_atime;				//10
		struct tm* atime;
		atime = gmtime(&i_atime);
		strftime(a_time, sizeof(a_time), "%m/%d/%y %H:%M:%S", atime);



		//array
		__u32 i_block[EXT2_N_BLOCKS];						//13...27
		int l;
		for (l = 0; l < EXT2_N_BLOCKS; l++) {
			i_block[l] = inode_buffer.i_block[l];
		}

		printf("%s,%u,%c,%o,%d,%d,%d,%s,%s,%s,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u\n", 
									"INODE",
									i+1,
									file_type,
									inode_buffer.i_mode & 0xFFF,
									inode_buffer.i_uid,
									inode_buffer.i_gid,
									inode_buffer.i_links_count,
									c_time,
									m_time,
									a_time,
									inode_buffer.i_size,
									inode_buffer.i_blocks,
									i_block[0],
									i_block[1],
									i_block[2],
									i_block[3],
									i_block[4],
									i_block[5],
									i_block[6],
									i_block[7],
									i_block[8],
									i_block[9],
									i_block[10],
									i_block[11],
									i_block[12],
									i_block[13],
									i_block[14]);

		//dirent
		if (file_type == 'd') {
			int level = 0;
			__u32 global_block_number = 1;
			__u32 parent_inode_number = i + 1;
			__u32 block_offset = get_block_number(&level, i_block, global_block_number) * mySuperBlock.s_block_size;
			__u32 global_offset = 0;
			__u32 local_offset = 0;

			while(true) {
				struct ext2_dir_entry curr_dirent;
				if (local_offset >= mySuperBlock.s_block_size) {	//the last one
					global_block_number += 1;
					local_offset -= mySuperBlock.s_block_size;
					block_offset = get_block_number(&level, i_block, global_block_number) * mySuperBlock.s_block_size;
				}
				
				//read curr_dirent
				ret = pread(fd, &curr_dirent, sizeof(struct ext2_dir_entry), block_offset+local_offset);
				if (ret < 0) {
					fprintf(stderr, "Error: pread in dirent.\n");
					fclose(file);
					exit(PROCESSING_ERRORS);
				}

				//extract data from curr_dirent
				__u32 referenced_inode_number = curr_dirent.inode;
				if (!referenced_inode_number) break;
				__u16 entry_length = curr_dirent.rec_len;
				__u8 name_length = curr_dirent.name_len;
				char name[255];
				int t;
				for (t = 0; t < 255; t++) {
					name[t] = curr_dirent.name[t];
				}
				printf("%s,%u,%u,%u,%u,%u,'%s'\n", 
						"DIRENT",
						parent_inode_number,
						global_offset,
						referenced_inode_number,
						entry_length,
						name_length,
						name);

				//adjust offset
				local_offset += entry_length;
				global_offset += entry_length;
			}
		}

		//Handle indirect blocks
		__u32 indirect_offset = 12;
		__u32 double_indirect_offset = indirect_offset + 256;
		__u32 triple_indirect_offset = double_indirect_offset + 256 * 256;
		indirect_block(1, inode_buffer.i_block[12], indirect_offset, i + 1, inode_buffer);
		indirect_block(2, inode_buffer.i_block[13], double_indirect_offset, i + 1, inode_buffer);
		indirect_block(3, inode_buffer.i_block[14], triple_indirect_offset, i + 1, inode_buffer);
	}
}

int main(int argc, char* argv[]) {
	//Check arguments
	if (argc != 2) {
		fprintf(stderr, "Usage: ./lab3a EXT2_test.img\n");
		exit(BAD_ARGUMENT);
	}

	//Read file
	const char* file_input = argv[1];
	fd = open(file_input, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Error: open file.\n");
		exit(BAD_ARGUMENT);
	}

	superblock();
	group();
	free_block_entries();
	free_inode_entries();
	inode();

	/* Clean up */

	exit(ANALYSIS_SUCCESSFUL);
}