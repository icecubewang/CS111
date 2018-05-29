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

#include "ext2_fs.h"

#define ANALYSIS_SUCCESSFUL 0
#define PROCESSING_ERRORSS       1
#define PROCESSING_ERRORS	2

int fd = -1;

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
	fprintf(file, "%s,%u,%u,%u,%u,%u,%u,%u\n", 
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
g_data* myGroups;

void group() {
	/* Open csv file */
	FILE* file = fopen("summary.csv", "a");	//"a": append mode
	if (file == NULL) {
		fprintf(stderr, "Error: fopen in group().\n");
		free(myGroups);
		exit(PROCESSING_ERRORS);
	}

	/* Note: "In the image we give you, there will be only a single group." */

	/* Calculate groups count */
	mySuperBlock.s_groups_count = 1 + (mySuperBlock.s_blocks_count / mySuperBlock.s_blocks_per_group);	//进一法

	/* Create group buffer array */
	myGroups = malloc(sizeof(g_data) * mySuperBlock.s_groups_count);
	if (myGroups == NULL) {
		fprintf(stderr, "Error: malloc in group().\n");
		fclose(file);
		exit(PROCESSING_ERRORS);
	}

	/* Create and read into data array */
	g group_descriptor[mySuperBlock.s_groups_count];
	int ret;
	ret = pread(fd, group_descriptor, sizeof(group_descriptor) * mySuperBlock.s_groups_count, 1024 * 2);	/* The block group descriptor table starts on the first block following the superblock. */
	if (ret < 0) {
		fprintf(stderr, "Error: pread in group().\n");
		free(myGroups);
		fclose(file);
		exit(PROCESSING_ERRORS);
	}

	/* Iteration to read data into myGroups */
	int i;
	for (i = 0; i < mySuperBlock.s_groups_count; i++) {
		/* blocks count and inodes count */
		if (i == mySuperBlock.s_groups_count - 1) {	/* Last group */
			/* blocks count = remainder */
			myGroups[i].g_blocks_count = mySuperBlock.s_blocks_count - (mySuperBlock.s_blocks_per_group * i);
			/* inodes count = remainder */
			myGroups[i].g_inodes_count = mySuperBlock.s_inodes_count - (mySuperBlock.s_inodes_per_group * i);
		}
		else {	/* Not last group */
			/* block count = read from superblock */
			myGroups[i].g_blocks_count = mySuperBlock.s_blocks_per_group;
			myGroups[i].g_inodes_count = mySuperBlock.s_inodes_per_group;
		}

		/* 啊啊啊啊拿到offer了心情瞬间就好了压力也瞬间消失嘻嘻 */
		/* free blocks count and free inodes count : just read from data array */
		myGroups[i].g_free_blocks_count = group_descriptor[i].bg_free_blocks_count;
		myGroups[i].g_free_inodes_count = group_descriptor[i].bg_free_inodes_count;

		/* first free block bitmap and first free inode bitmap : just read from data array */
		myGroups[i].g_first_free_block_bitmap = group_descriptor[i].bg_block_bitmap;
		myGroups[i].g_first_free_inode_bitmap = group_descriptor[i].bg_inode_bitmap;

		/* first block of inodes : just read from data array */
		myGroups[i].g_first_block_of_inodes = group_descriptor[i].bg_inode_table;

		/* Write to file */
		fprintf(file, "%s,%u,%u,%u,%u,%u,%u,%u,%u\n", 
				"GROUP", 
				(__u32)i, 
				myGroups[i].g_blocks_count, 
				myGroups[i].g_inodes_count, 
				myGroups[i].g_free_blocks_count, 
				myGroups[i].g_free_inodes_count, 
				myGroups[i].g_first_free_block_bitmap, 
				myGroups[i].g_first_free_inode_bitmap, 
				myGroups[i].g_first_block_of_inodes);

		/* Clean up */
		fclose(file);
	}
}

/* Free block entires (for each group) */
/*
1. BFREE
2. number of the free block (decimal)
*/

void free_block_entries() {
	/* Open csv file */
	FILE* file = fopen("summary.csv", "a");	//"a": append mode
	if (file == NULL) {
		fprintf(stderr, "Error: fopen in free_block_entries().\n");
		free(myGroups);
		exit(PROCESSING_ERRORS);
	}

	/* Iteration to read each group's free blocks */
	__u32 curr_number_of_blocks = 0;
	int i;
	for (i = 0; i < mySuperBlock.s_groups_count; i++) {
		__u32 first_free_block_bitmap = myGroups[i].g_first_free_block_bitmap;
		__u32 number_of_blocks = myGroups[i].g_blocks_count;

		//Create a buffer and Read data into buffer
		__u8* bitmap = malloc(mySuperBlock.s_block_size);
		int ret;
		ret = pread(fd, bitmap, mySuperBlock.s_block_size, first_free_block_bitmap * mySuperBlock.s_block_size);
		if (ret < 0) {
			fprintf(stderr, "Error: pread in free_block_entires().\n");
			fclose(file);
			free(myGroups);
			free(bitmap);
			exit(PROCESSING_ERRORS);
		}

		//Loop through the bitmap block to check every bit
		int j;
		for (j = 0; j < mySuperBlock.s_block_size; j++) {
			__u8 mask = 1;
			int k;
			for (k = 0; k < 8; k++) {
				if (curr_number_of_blocks >= number_of_blocks) {
					free(bitmap);
					fclose(file);
					return;
				}
				else {
					curr_number_of_blocks += 1;
					//free = 0, in-use = 1
					if ((bitmap[j] & mask) == 0) {	//free bit
						fprintf(file, "%s,%u\n", "BFREE", curr_number_of_blocks + mySuperBlock.s_blocks_per_group * i);
					}
					mask <<= 1;
				}
			}
		}
		free(bitmap);
	}
	fclose(file);
}

/* Free i-node entries (for each group) */
/*
1. IFREE
2. number of the free I-node (decimal)
*/

void free_inode_entries() {
	/* Open csv file */
	FILE* file = fopen("summary.csv", "a");	//"a": append mode
	if (file == NULL) {
		fprintf(stderr, "Error: fopen in free_inode_entries().\n");
		free(myGroups);
		exit(PROCESSING_ERRORS);
	}

	/* Iteration to read each group's free inodes */
	__u32 curr_number_of_inodes = 0;
	int i;
	for (i = 0; i < mySuperBlock.s_groups_count; i++) {
		__u32 first_free_inode_bitmap = myGroups[i].g_first_free_inode_bitmap;
		__u32 number_of_inodes = myGroups[i].g_inodes_count;

		/* Create a buffer and Read data into buffer */
		__u8* bitmap = malloc(mySuperBlock.s_block_size);
		int ret;
		ret = pread(fd, bitmap, mySuperBlock.s_block_size, first_free_inode_bitmap * mySuperBlock.s_block_size);
		if (ret < 0) {
			fprintf(stderr, "Error: pread in free_inode_entries().\n");
			fclose(file);
			free(myGroups);
			free(bitmap);
			exit(PROCESSING_ERRORS);
		}

		/* Loop through the bitmap block to check every bit */
		int j;
		for (j = 0; j < mySuperBlock.s_inode_size; j++) {
			__u8 mask = 1;
			int k;
			for (k = 0; k < 8; k++) {
				if (curr_number_of_inodes >= number_of_inodes) {
					free(bitmap);
					fclose(file);
					return;
				}
				else {
					curr_number_of_inodes += 1;
					//free = 0, in-use = 1
					if ((bitmap[j] & mask) == 0) {	//free bit
						fprintf(file, "%s,%u\n", "IFREE", curr_number_of_inodes + mySuperBlock.s_inodes_per_group * i);
					}
					mask <<= 1;
				}
			}
		}
		free(bitmap);
	}
	fclose(file);
}

/* I-node Summary */
/*
1. INODE
2. inode number (decimal)
3. file type ('f' for file, 'd' for directory, 's' for symbolic link, "?" for anything else)
4. mode (low order 12-bits, octal... suggested format "%o")
5. owner (decimal)
6. group (decimal)
7. link count (decimal)
8. time of last I-node change (mm/dd/yy hh:mm:ss, GMT)
9. modification time (mm/dd/yy hh:mm:ss, GMT)
10. time of last access (mm/dd/yy hh:mm:ss, GMT)
11. file size (decimal)
12. number of (512 byte) blocks of disk space (decimal) taken up by this file
*/

void inode() {
	/* Open csv file */
	FILE* file = fopen("summary.csv", "a");	//"a": append mode
	if (file == NULL) {
		fprintf(stderr, "Error: fopen in inode().\n");
		free(myGroups);
		exit(PROCESSING_ERRORSS);
	}

	/* Iteration to read each group's non-free inodes */
	__u32 curr_number_of_inodes = 0;
	int i;
	for (i = 0; i < mySuperBlock.s_groups_count; i++) {
		__u32 first_free_inode_bitmap = myGroups[i].g_first_free_inode_bitmap;
		__u32 number_of_inodes = myGroups[i].g_inodes_count;
		__u32 first_block_of_inodes = myGroups[i].g_first_block_of_inodes;

		/* Create a buffer and Read data into buffer */
		__u8* bitmap = malloc(mySuperBlock.s_block_size);
		int ret;
		ret = pread(fd, bitmap, mySuperBlock.s_block_size, first_free_inode_bitmap * mySuperBlock.s_block_size);
		if (ret < 0) {
			fprintf(stderr, "Error: pread in inode().\n");
			fclose(file);
			free(myGroups);
			free(bitmap);
			exit(PROCESSING_ERRORSS);
		}

		/* Loop through the bitmap block to check every bit */
		int j;
		for (j = 0; j < mySuperBlock.s_inode_size; j++) {
			__u8 mask = 1;
			int k;
			for (k = 0; k < 8; k++) {
				if (curr_number_of_inodes >= number_of_inodes) {
					free(bitmap);
					fclose(file);
					return;
				}
				else {
					curr_number_of_inodes += 1;
					if ((bitmap[j] & mask) != 0) {	//in-use
						struct ext2_inode curr_inode;
						ret = pread(fd, &curr_inode, mySuperBlock.s_inode_size, first_block_of_inodes * mySuperBlock.s_block_size + mySuperBlock.s_inode_size * ((curr_number_of_inodes - 1) % mySuperBlock.s_inodes_per_group));
						if (ret < 0) {
							fprintf(stderr, "Error: pread in inode().\n");
							fclose(file);
							free(myGroups);
							free(bitmap);
							exit(PROCESSING_ERRORSS);
						}
						
						/* Read data */
						__u16 i_mode = curr_inode.i_mode;					//4
						__u16 i_uid = curr_inode.i_uid;						//5
						__u16 i_gid = curr_inode.i_gid;						//6
						__u16 i_links_count = curr_inode.i_links_count;		//7
						time_t i_ctime = curr_inode.i_ctime;				//8
						time_t i_mtime = curr_inode.i_mtime;				//9
						time_t i_atime = curr_inode.i_atime;				//10
						__u32 i_size = curr_inode.i_size;					//11
						__u32 i_blocks = curr_inode.i_blocks;				//12

						__u32 i_block[EXT2_N_BLOCKS];						//13...27
						int l;
						for (l = 0; l < EXT2_N_BLOCKS; l++) {
							i_block[l] = curr_inode.i_block[l];
						}

						//EXT2_S_IFREG	0x8000	regular file
						//EXT2_S_IFDIR	0x4000	directory
						//EXT2_S_IFLNK	0xA000	symbolic link
						char file_type;
						if (i_mode & 0x8000) {
							file_type = 'f';
						}
						else if (i_mode & 0x4000) {
							file_type = 'd';
						}
						else if (i_mode & 0xA000) {
							file_type = 's';
						}
						else {
							file_type = '?';
						}

						//Time conversion
						struct tm* atime;
						struct tm* mtime;
						struct tm* ctime;
						char a_time[100], m_time[100], c_time[100];
						atime = gmtime(&i_atime);
						mtime = gmtime(&i_mtime);
						ctime = gmtime(&i_ctime);
						strftime(a_time, sizeof(a_time), "%m/%d/%y %H:%M:%S", atime);
						strftime(m_time, sizeof(m_time), "%m/%d/%y %H:%M:%S", mtime);
						strftime(c_time, sizeof(c_time), "%m/%d/%y %H:%M:%S", ctime);

						if (i_links_count > 0 && (i_mode & 0x0FFF) != 0) {
							fprintf(file, "%s,%u,%c,%o,%d,%d,%d,%s,%s,%s,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u\n", 
									"INODE",
									curr_number_of_inodes,
									file_type,
									i_mode & 0xFFF,
									i_uid,
									i_gid,
									i_links_count,
									c_time,
									m_time,
									a_time,
									i_size,
									i_blocks,
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
						}
						mask <<= 1;
					}
				}
			}
		}
		free(bitmap);
	}
	fclose(file);
}

int main(int argc, char* argv[]) {
	//Check arguments
	if (argc != 2) {
		fprintf(stderr, "Usage: ./lab3a EXT2_test.img\n");
		exit(PROCESSING_ERRORSS);
	}

	//Read file
	const char* file_input = argv[1];
	fd = open(file_input, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Error: open file.\n");
		exit(PROCESSING_ERRORS);
	}

	superblock();
	group();
	free_block_entries();
	free_inode_entries();
	inode();

	/* Clean up */
	free(myGroups);

	exit(ANALYSIS_SUCCESSFUL);
}