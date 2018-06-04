#NAME: Feilan Wang, Kana Yabuno
#EMAIL: wangfeilan@hotmail.com, kana0111@g.ucla.edu
#ID: 104796844, 404849285

#!/user/local/cs/bin/python
from __future__ import print_function
import csv
import sys

#initialization
blockDict_Isfree = dict()
blockDict_allcation = dict()

inodeDict_Isfree = dict()
inodeDict_allocated = dict()
inodeDict_linkCount = dict()
inodeDict_realParent = dict()
inodeDict_ReferenceNumber = dict()
inodeDict_children = dict()
inodeDict_name = dict()

mySuperBlock = None
myGroup = None

#container
class superBlockInfo:
	def __init__(self, totalNumberOfBlocks, totalNumberOfInodes, blockSize, inodeSize, blocksPerGroup, inodesPerGroup, firstNonReservedInode):
		self.totalNumberOfBlocks = totalNumberOfBlocks
		self.totalNumberOfInodes = totalNumberOfInodes
		self.blockSize = blockSize
		self.inodeSize = inodeSize
		self.blocksPerGroup = blocksPerGroup
		self.inodesPerGroup = inodesPerGroup
		self.firstNonReservedInode = firstNonReservedInode

class groupInfo:
	def __init__(self, groupNumber, totalNumberOfBlocks, totalNumberOfInodes, numberOfFreeBlocks, numberOfFreeInodes, freeBlockBitmap, freeInodeBitmap, firstBlockOfInodes):
		self.groupNumber = groupNumber
		self.totalNumberOfBlocks = totalNumberOfBlocks
		self.totalNumberOfInodes = totalNumberOfInodes
		self.numberOfFreeBlocks = numberOfFreeBlocks
		self.numberOfFreeInodes = numberOfFreeInodes
		self.freeBlockBitmap = freeBlockBitmap
		self.freeInodeBitmap = freeInodeBitmap
		self.firstBlockOfInodes = firstBlockOfInodes

class blockInfo:
	def __init__(self, blockNumber, inodeNumber, block, offset, level):
		self.blockNumber = blockNumber
		self.inodeNumber = inodeNumber
		self.block = block
		self.offset = offset
		self.level = level

def parseArgument():
	if len(sys.argv) != 2:
		print("Usage: lab3b XXXXXX.csv\n", file = sys.stderr)
		sys.exit(1)
	# try:
	# 	file myCSV = open(sys.argv[1], "r")
	# except IOError:
	# 	print("Fail to open csv file.\n")
	# 	sys.exit(1)

	with open(sys.argv[1], "r") as f:
		for line in f:
			x = line.split(",")
			if x[0] == "SUPERBLOCK":
				totalNumberOfBlocks = int(x[1])
				totalNumberOfInodes = int(x[2])
				blockSize = int(x[3])
				inodeSize = int(x[4])
				blocksPerGroup = int(x[5])
				inodesPerGroup = int(x[6])
				firstNonReservedInode = int(x[7])
				global mySuperBlock
				mySuperBlock = superBlockInfo(totalNumberOfBlocks, totalNumberOfInodes, blockSize, inodeSize, blocksPerGroup, inodesPerGroup, firstNonReservedInode)
				initContainer()

			elif x[0] == "GROUP":
				groupNumber = int(x[1])
				totalNumberOfBlocks = int(x[2])
				totalNumberOfInodes = int(x[3])
				numberOfFreeBlocks = int(x[4])
				numberOfFreeInodes = int(x[5])
				freeBlockBitmap = int(x[6])
				freeInodeBitmap = int(x[7])
				firstBlockOfInodes = int(x[8])
				global myGroup
				myGroup = groupInfo(groupNumber, totalNumberOfBlocks, totalNumberOfInodes, numberOfFreeBlocks, numberOfFreeInodes, freeInodeBitmap, freeInodeBitmap, firstBlockOfInodes)

			elif x[0] == "BFREE":
				blockNumber = int(x[1])
				blockDict_Isfree[blockNumber] = True	#true == free, false == not free


			elif x[0] == "IFREE":
				inodeNumber = int(x[1])
				inodeDict_Isfree[inodeNumber] = True

			elif x[0] == "INDIRECT":
				blockNumber = int(x[5])
				myBlock = blockInfo(blockNumber, int(x[1]), "INDIRECT BLOCK", int(x[3]), int(x[2]))
				if blockNumber not in blockDict_allcation:
					blockDict_allcation[blockNumber] = set()
				blockDict_allcation[blockNumber].add(myBlock)

			elif x[0] = "INODE":
				inodeNumber = int(x[1])
				inodeDict_allocated[inodeNumber] = True
				inodeDict_linkCount[inodeNumber] = int(x[6])

				for k in range(15)
					blockNumber = int(x[12] + k)
					level = 0
					block = "BLOCK"
					offset = k

					if k == 12:
						level = 1
						block = "INDIRECT BLOCK"
						offset = 12
					elif k == 13:
						level = 2
						block = "DOUBLE INDIRECT BLOCK"
						offset = 268
					elif k == 14:
						level = 3
						block = "TRIPLE INDIRECT BLOCK"
						offset = 65804

					if blockNumber > 0:
						myBlock = blockInfo(blockNumber, inodeNumber, block, offset, level)
						if blockNumber not in blockDict_allcation:
							blockDict_allcation[blockNumber] = set()
						blockDict_allcation[blockNumber].add(myBlock)

			elif x[0] == "DIRENT":
				inodeNumber = x[3]
				if inodeNumber in inodeDict_ReferenceNumber.keys():
					inodeDict_ReferenceNumber[inodeNumber] += 1
				else:
					inodeDict_ReferenceNumber[inodeNumber] = 1

				inodeDict_children[x[1]].append((int(x[6]), int(x[3])))
				inodeDict_name[inodeNumber] = name

				#TODO: Can a file have two real parent?
				inodeDict_realParent[inodeNumber] = int(x[1])

def inodeAllocationAudit():
	for inodeNumber in inodeDict_allocated.keys():
		isAllocated = inodeDict_allocated[inodeNumber]
		isFree = inodeDict_Isfree[inodeNumber]
		if isAllocated and isFree:
			print("ALLOCATED INODE " + inodeNumber + " ON FREELIST\n")
		if not isAllocated and not isFree:
			print("UNALLOCATED INODE " + inodeNumber + " NOT ON FREELIST\n")

def directoryConsistencyAudit():
	for inodeNumber in inodeDict_linkCount.keys():
		linkCount = inodeDict_linkCount[inodeNumber]
		referencedNumber = inodeDict_ReferenceNumber[inodeNumber]
		if inodeNumber < 1 or inodeNumber > mySuperBlock.totalNumberOfInodes:
			print("DIRECTORY INODE " + inodeDict_realParent(inodeNumber) + " NAME '" + inodeDict_name[inodeNumber] + "' INVALID INODE " + inodeNumber + "\n")
		if inodeDict_Isfree[inodeNumber] and linkCount != 0:
			print("DIRECTORY INODE " + inodeDict_realParent(inodeNumber) + " NAME '" + inodeDict_name[inodeNumber] + "' UNALLOCATED INODE " + inodeNumber + "\n")
		if linkCount != referencedNumber:
			print("INODE " + inodeNumber + " HAS " + referencedNumber + " LINKS BUT LINKCOUNT IS " + linkCount + "\n")

def block_consistency_audits():
	blockNumbers = blockDictIsfree.keys() 
	for k, v in blockNumbers.items(): #check allocated block on freelist
		if v and k in blockDict_allcation:
			print("ALLOCATED " list(blockDict_allcation[k])[0].block + list(blockDict_allcation[k])[0].blockNumber + " ON FREELIST" + "\n")
		#elif not v and not k in blockDict_allcation:
		#	print("UNREFERENCED BLOCK" + list(blockDict_allcation[k])[0].block + list(blockDict_allcation[k])[0].blockNumber)

	blockNumbers = blockDict_allcation.keys()
	for i in blockNumbers:
		# check invalid
		if i < 0 or i >= mySuperBlock.totalNumberOfBlocks:
			print("INVALUD" + list(blockDict_allcation[k])[0].block " " + list(blockDict_allcation[k])[0].blockNumbers +" IN INODE " + list(blockDict_allcation[k])[0].inodeNumber + " AT OFFSET " + list(blockDict_allcation[k])[0].offset + "\n")
		elif i < mySuperBlock.firstNonReservedInode: #check reserved block
			print("RESERVED " + list(blockDict_allcation[k])[0].block + " " + list(blockDict_allcation[k])[0].blockNumbers +" IN INODE " + list(blockDict_allcation[k])[0].inodeNumber + " AT OFFSET " + list(blockDict_allcation[k])[0].offset + "\n")
		elif len(blockDict_allcation[k]) > 1: #duplicate
			print("DUPLICATE" + list(blockDict_allcation[k])[0].block " " + list(blockDict_allcation[k])[0].blockNumbers +" IN INODE " + list(blockDict_allcation[k])[0].inodeNumber + " AT OFFSET " + list(blockDict_allcation[k])[0].offset + "\n")

	# a number of group = total number of blocks/blocks per group
	# number of block in each group = blocksPerGroup
	# a last block in inodes = first block in inodes + 
	# a = (inodes_size * inodes_per_group)
	# b = size_of_block
	# last block in inodes = first block in inodes + (a-1)//(b+1)

	# scann all blocks
	numberOfGroup = mySuperBlock.totalNumberOfBlocks / mySuperBlock.blocksPerGroup
	blocksPerGroup = mySuperBlock.blocksPerGroup
	a = (mySuperBlock.inodes_size * mySuperBlock.inodes_per_group)
	b = mySuperBlock.blockSize
	lastBlockInInodes = myGroup.firstBlockOfInodes + ((a - 1) // (b + 1))
	for k in range(mySuperBlock.totalNumberOfBlocks, lastBlockInInodes):
		if blockDict_Isfree[k]:
			continue
		elif k in blockDict_allcation.keys()
			continue
		else
			print("UNREFERENCED BLOCK" + list(blockDict_allcation[k])[0].block + list(blockDict_allcation[k])[0].blockNumber)


def initContainer():
	for num in range(0, mySuperBlock.totalNumberOfBlocks):
		blockDictIsfree[num] = False




def main():
	parseArgument()
	inodeAllocationAudit()
	directoryConsistencyAudit()
	block_consistency_audits()

if __name__ == "__main__":
	main()