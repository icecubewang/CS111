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
	__u32	s_first_ino;	/* First non-reserved inode */
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

	exit(ANALYSIS_SUCCESSFUL);
}