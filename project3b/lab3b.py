#NAME: Feilan Wang, Kana Yabuno
#EMAIL: wangfeilan@hotmail.com, kana0111@g.ucla.edu
#ID: 104796844, 404849285

#heello
#!/user/local/cs/bin/python
import sys

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
	def __init__(self, blockNumber, inodeNumber, offset, level):
		self.blockNumber = blockNumber
		self.inodeNumber = inodeNumber
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
				totalNumberOfBlocks = x[1]
				totalNumberOfInodes = x[2]
				blockSize = x[3]
				inodeSize = x[4]
				blocksPerGroup = x[5]
				inodesPerGroup = x[6]
				firstNonReservedInode = x[7]
				mySuperBlock = superBlockInfo(totalNumberOfBlocks, totalNumberOfInodes, blockSize, inodeSize, blocksPerGroup, inodesPerGroup, firstNonReservedInode)
				initContainer()

			elif x[0] = "GROUP":
				groupNumber = x[1]
				totalNumberOfBlocks = x[2]
				totalNumberOfInodes = x[3]
				numberOfFreeBlocks = x[4]
				numberOfFreeInodes = x[5]
				freeBlockBitmap = x[6]
				freeInodeBitmap = x[7]
				firstBlockOfInodes = x[8]

			elif x[0] = "BFREE":
				blockNumber = x[1]
				blockDict_Isfree[blockNumber] = True	#true == free, false == not free


			elif x[0] = "IFREE":
				inodeNumber = x[1]
				inodeDict_Isfree[inodeNumber] = True

			elif x[0] = "INDIRECT":
				blockNumber = x[5]
				myBlock = blockInfo(blockNumber, x[1], x[3], x[2])
				if blockNumber not in blockDict_allcation:
					blockDict_allcation[blockNumber] = set()
				blockDict_allcation[blockNumber].add(myBlock)

			elif x[0] = "INODE":
				inodeNumber = x[1]
				inodeDict_allocated[inodeNumber] = True
				inodeDict_linkCount[inodeNumber] = x[6]

			elif x[0] = "DIRENT":
				inodeNumber = x[3]
				if inodeNumber in inodeDict_ReferenceNumber.keys():
					inodeDict_ReferenceNumber[inodeNumber] += 1
				else:
					inodeDict_ReferenceNumber[inodeNumber] = 1

				inodeDict_children[x[1]].append((x[6], x[3]))

				#TODO: Can a file have two real parent?
				inodeDict_realParent[inodeNumber] = x[1]

def initContainer():
	for num in range(0, mySuperBlock.totalNumberOfBlocks):
		blockDictIsfree[num] = False


def main():
	parseArgument()


if __name__ == "__main__":
	main()