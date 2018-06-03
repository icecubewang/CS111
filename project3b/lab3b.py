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

inodeDict_allocated = dict()
inodeDict_linkCount = dict()
inodeDict_realParent = dict()
inodeDict_ReferenceNumber = dict()
inodeDict_children = dict()

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
			if x[0] = "SUPERBLOCK":
				totalNumberOfBlocks = int(x[1])
				totalNumberOfInodes = int(x[2])
				blockSize = int(x[3])
				inodeSize = int(x[4])
				blocksPerGroup = int(x[5])
				inodesPerGroup = int(x[6])
				firstNonReservedInode = int(x[7])
				mySuperBlock = superBlockInfo(totalNumberOfBlocks, totalNumberOfInodes, blockSize, inodeSize, blocksPerGroup, inodesPerGroup, firstNonReservedInode)
				initContainer()

			elif x[0] = "GROUP":
				groupNumber = int(x[1])
				totalNumberOfBlocks = int(x[2])
				totalNumberOfInodes = int(x[3])
				numberOfFreeBlocks = int(x[4])
				numberOfFreeInodes = int(x[5])
				freeBlockBitmap = int(x[6])
				freeInodeBitmap = int(x[7])
				firstBlockOfInodes = int(x[8])

			elif x[0] = "BFREE":
				blockNumber = int(x[1])
				blockDict_Isfree[blockNumber] = True	#true == free, false == not free


			elif x[0] = "IFREE":
				inodeNumber = int(x[1])
				inodeDict_Isfree[inodeNumber] = True

			elif x[0] = "INDIRECT":
				blockNumber = int(x[5])
				myBlock = blockInfo(blockNumber, int(x[1]), "INDIRECT BLOCK",int(x[3]), int(x[2]))
				if blockNumber not in blockDict_allcation:
					blockDict_allcation[blockNumber] = set()
				blockDict_allcation[blockNumber].add(myBlock)

			elif x[0] = "INODE":
				inodeNumber = int(x[1])
				inodeDict_allocated[inodeNumber] = True
				inodeDict_linkCount[inodeNumber] = int(x[6])

				blockDict_allcation[inodeNumber] = []

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


			elif x[0] = "DIRENT":
				inodeNumber = x[3]
				if inodeNumber in inodeDict_ReferenceNumber.keys():
					inodeDict_ReferenceNumber[inodeNumber] += 1
				else:
					inodeDict_ReferenceNumber[inodeNumber] = 1

				inodeDict_children[x[1]].append((int(x[6]), int(x[3])))

				#TODO: Can a file have two real parent?
				inodeDict_realParent[inodeNumber] = int(x[1])

def initContainer():
	for num in range(0, mySuperBlock.totalNumberOfBlocks):
		blockDictIsfree[num] = False

def block_consistency_audits():
	#scan all block pointers in the I-node



def main():
	parseArgument()


if __name__ == "__main__":
	main()