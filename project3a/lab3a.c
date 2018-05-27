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
#define BAD_ARGUMENTS       1
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
	/* Note: "In the image we give you, there will be only a single group." */

	/* Calculate groups count */
	mySuperBlock.s_groups_count = 1 + (mySuperBlock.s_blocks_count / mySuperBlock.s_blocks_per_group);	//进一法

	/* Create group buffer array */
	myGroups = malloc(sizeof(g_data) * mySuperBlock.s_groups_count);
	if (myGroups == NULL) {
		fprintf(stderr, "Error: malloc in group().\n");
		exit(PROCESSING_ERRORS);
	}

	/* Create and read into data array */
	g group_descriptor[mySuperBlock.s_groups_count];
	int ret;
	ret = pread(fd, group_descriptor, sizeof(group_descriptor) * mySuperBlock.s_groups_count, 1024 * 2);	/* The block group descriptor table starts on the first block following the superblock. */
	if (ret < 0) {
		fprintf(stderr, "Error: pread in group().\n");
		free(myGroups);
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

		/* Open csv file */
		FILE* file = fopen("summary.csv", "a");	//"a": append mode
		if (file == NULL) {
			fprintf(stderr, "Error: fopen in group().\n");
			free(myGroups);
			exit(PROCESSING_ERRORS);
		}

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

int main(int argc, char* argv[]) {
	//Check arguments
	if (argc != 2) {
		fprintf(stderr, "Usage: ./lab3a EXT2_test.img\n");
		exit(BAD_ARGUMENTS);
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

	/* Clean up */
	free(myGroups);

	exit(ANALYSIS_SUCCESSFUL);
}