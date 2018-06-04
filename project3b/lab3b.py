#!/usr/bin/python
#NAME: Feilan Wang, Kana Yabuno
#EMAIL: wangfeilan@hotmail.com, kana0111@g.ucla.edu
#ID: 104796844, 404849285

import sys
import os
from time import sleep

#initialization
returnCode = 0

blockDict_Isfree = dict()		#initialize all to false
blockDict_allcation = dict()

inodeDict_Isfree = dict()		#initialize all to false
inodeDict_allocated = dict()
inodeDict_mode = dict()
inodeDict_linkCount = dict()	#initialize all to 0
inodeDict_realParent = dict()
inodeDict_ReferenceNumber = dict()
inodeDict_children = dict()
inodeDict_name = dict()

inodeNumbers = set()
blockNumbers = set()
direntEntries = set()

#containers
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


class childInfo:
	def __init__(self, name, inodeNumber):
		self.name = name
		self.inodeNumber = inodeNumber

class blockInfo:
	def __init__(self, blockNumber, inodeNumber, offset, level):
		self.blockNumber = blockNumber
		self.inodeNumber = inodeNumber
		self.offset = offset
		self.level = level

class direntInfo:
	def __init__(self, parentInodeNumber, inodeNumber, name):
		self.parentInodeNumber = parentInodeNumber
		self.inodeNumber = inodeNumber
		self.name = name

def parseArgument():
	global mySuperBlock
	global myGroup

	if len(sys.argv) != 2:
		sys.stderr.write("Usage: lab3b XXX.csv\n")
		sys.exit(1)
	if not os.path.isfile(sys.argv[1]):
		sys.stderr.write("Usage: lab3b XXX.csv\n")
		sys.exit(1)
	with open(sys.argv[1], "r") as f:
		for line in f:
			x = line.split(",")
			if x[0] == "SUPERBLOCK":
				mySuperBlock = superBlockInfo(int(x[1]), int(x[2]), int(x[3]), int(x[4]), int(x[5]), int(x[6]), int(x[7]))

			elif x[0] == "GROUP":
				myGroup = groupInfo(int(x[1]), int(x[2]), int(x[3]), int(x[4]), int(x[5]), int(x[6]), int(x[7]), int(x[8]))

			elif x[0] == "BFREE":
				if not x[1] in blockDict_Isfree.keys():
					blockDict_Isfree[int(x[1])] = True

			elif x[0] == "IFREE":
				if not x[1] in inodeDict_Isfree.keys():
					inodeDict_Isfree[int(x[1])] = True

			elif x[0] == "INODE":
				inodeNumber = int(x[1])
				inodeNumbers.add(inodeNumber)
				if not inodeNumber in inodeDict_allocated.keys():
					inodeDict_allocated[inodeNumber] = True
				if not inodeNumber in inodeDict_linkCount.keys():
					inodeDict_linkCount[inodeNumber] = int(x[6])
				if not inodeNumber in inodeDict_mode.keys():
					inodeDict_mode[inodeNumber] = int(x[3])
				for num in range(0, 15):
					blockNumber = int(x[12 + num])
					level = num - 11
					if level <= 0:
						level = 0
					offset = num
					if level == 0:
						offset = num
					elif level == 1:
						offset = 12
					elif level == 2:
						offset = 268
					elif level == 3:
						offset = 65804

					if blockNumber > 0:
						myBlock = blockInfo(blockNumber, inodeNumber, offset, level)
						if not blockNumber in blockDict_allcation.keys():
							blockDict_allcation[blockNumber] = set()
						blockDict_allcation[blockNumber].add(myBlock)

			elif x[0] == "DIRENT":
				myDirent = direntInfo(x[1], x[3], x[6])
				direntEntries.add(myDirent)
				inodeNumber = int(x[3])
				inodeNumbers.add(inodeNumber)
				if not inodeNumber in inodeDict_ReferenceNumber.keys():
					inodeDict_ReferenceNumber[inodeNumber] = 1
				else:
					inodeDict_ReferenceNumber[inodeNumber] += 1
				if not int(x[1]) in inodeDict_children.keys():
					myChild = childInfo(x[6], int(x[3]))
					inodeDict_children[x[1]] = set()
					inodeDict_children[x[1]].add(myChild)
				else:
					myChild = childInfo(x[6], int(x[3]))
					inodeDict_children[x[1]].add(myChild)
				if not inodeNumber in inodeDict_name.keys():
					inodeDict_name[inodeNumber] = x[6]
				if x[6].rstrip('\n') != "'..'" and x[6].rstrip('\n') != "'.'":
					inodeDict_realParent[inodeNumber] = int(x[1])
					print(str(inodeNumber) + "'S PARENT IS " + str(x[1]))

			elif x[0] == "INDIRECT":
				blockNumber = int(x[5])
				inodeNumber = int(x[1])
				offset = int(x[3])
				level = int(x[2]) - 1
				myBlock = blockInfo(blockNumber, inodeNumber, offset, level)
				if not blockNumber in blockDict_allcation.keys():
					blockDict_allcation[blockNumber] = list()
				blockDict_allcation[blockNumber].append(myBlock)

def blockConsistencyAudits():
	for blockNumber in blockDict_allcation.keys():
		myBlocks = blockDict_allcation[blockNumber]
		block = list(myBlocks)[0]
		blockType = "BLOCK"
		if block.level == 1:
			blockType = "INDIRECT BLOCK"
		elif block.level == 2:
			blockType = "DOUBLE INDIRECT BLOCK"
		elif block.level == 3:
			blockType = "TRIPLE INDIRECT BLOCK"
		
		if blockNumber < 0 or blockNumber >= mySuperBlock.totalNumberOfBlocks:
			print("INVALID " + blockType + " " + str(blockNumber) + " IN INODE " + str(block.inodeNumber) + " AT OFFSET " + str(block.offset))
			returnCode = 2
		
		if len(myBlocks) >= 2:
			for myBlock in myBlocks:
				block_Type = "BLOCK"
				if myBlock.level == 1:
					block_Type = "INDIRECT BLOCK"
				if myBlock.level == 2:
					block_Type = "DOUBLE INDIRECT BLOCK"
				if myBlock.level == 3:
					block_Type = "TRIPLE INDIRECT BLOCK"
				print("DUPLICATE " + block_Type + " " + str(blockNumber) + " IN INODE " + str(myBlock.inodeNumber) + " AT OFFSET " + str(myBlock.offset))
				returnCode = 2

		if blockNumber in blockDict_Isfree.keys():
			print("ALLOCATED BLOCK " + str(blockNumber) + " ON FREELIST")
			returnCode = 2
		if blockNumber < 8:
			print("RESERVED " + blockType + " " + str(blockNumber) + " IN INODE " + str(block.inodeNumber) + " AT OFFSET " + str(block.offset))
			returnCode = 2
	for blockNumber in range(8, mySuperBlock.totalNumberOfBlocks):
		if not blockNumber in blockDict_Isfree.keys() and not blockNumber in blockDict_allcation.keys():
			print("UNREFERENCED BLOCK " + str(blockNumber))
			returnCode = 2

def inodeAllocationAudits():
	for inodeNumber in inodeNumbers:
		if not inodeNumber in inodeDict_Isfree.keys():
			isFree = False
		else:
			isFree = True
		if not inodeNumber in inodeDict_allocated.keys():
			isAllocated = False
		else:
			isAllocated = True
		if isFree and isAllocated:
			print("ALLOCATED INODE " + str(inodeNumber) + " ON FREELIST")
			returnCode = 2
	for inodeNumber in range(mySuperBlock.firstNonReservedInode, mySuperBlock.totalNumberOfInodes + 1):
		if not inodeNumber in inodeDict_Isfree.keys():
			isFree = False
		else:
			isFree = True
		if not inodeNumber in inodeDict_allocated.keys():
			isAllocated = False
		else:
			isAllocated = True
		if not isFree and not isAllocated:
			print("UNALLOCATED INODE " + str(inodeNumber) + " NOT ON FREELIST")
			returnCode = 2
		
def directoryConsistencyAudits():
	for inodeNumber in inodeNumbers:
		if inodeNumber < 1 or inodeNumber > mySuperBlock.totalNumberOfInodes:
			print("DIRECTORY INODE " + str(inodeDict_realParent[inodeNumber]) + " NAME " + inodeDict_name[inodeNumber].rstrip('\n') + " INVALID INODE " + str(inodeNumber))
			returnCode = 2

		elif not inodeNumber in inodeDict_allocated.keys():
			print("DIRECTORY INODE " + str(inodeDict_realParent[inodeNumber]) + " NAME " + inodeDict_name[inodeNumber].rstrip('\n') + " UNALLOCATED INODE " + str(inodeNumber))
			returnCode = 2

		linkCount = 0
		if inodeNumber in inodeDict_linkCount.keys():
			#print(linkCount)
			linkCount = inodeDict_linkCount[inodeNumber]
			#print(linkCount)

		#print(linkCount)
		referenceNumber = 0
		if inodeNumber in inodeDict_ReferenceNumber.keys():
			referenceNumber = inodeDict_ReferenceNumber[inodeNumber]

		if inodeDict_mode > 0 and linkCount != referenceNumber:
			print("INODE " + str(inodeNumber) + " HAS " + str(referenceNumber) + " LINKS BUT LINKCOUNT IS " + str(linkCount))
			returnCode = 2

	for dirent in direntEntries:
		parentInodeNumber = dirent.parentInodeNumber
		inodeNumber = dirent.inodeNumber
		name = dirent.name.rstrip('\n')
		if name == "'.'":
			if parentInodeNumber != inodeNumber:
				print("DIRECTORY INODE " + str(parentInodeNumber) + " NAME " + name + " LINK TO INODE " + str(inodeNumber) + " SHOULD BE " + str(parentInodeNumber))
				returnCode = 2
		elif name == "'..'":
			print("parent id: "+parentInodeNumber)
			print("inode id: "+inodeNumber)
			for key in inodeDict_realParent.keys():
				print(key)
			print("========")
			comp = 0
			if inodeNumber == 2 or parentInodeNumber == 2:
				continue
			if inodeNumber != inodeDict_realParent.get(parentInodeNumber, ):
				print("DIRECTORY INODE " + str(parentInodeNumber) + " NAME " + name + " LINK TO INODE " + str(inodeNumber) + " SHOULD BE " + str(inodeDict_realParent.get(parentInodeNumber, )))
				returnCode = 2


if __name__ == "__main__":
	parseArgument()
	blockConsistencyAudits()
	inodeAllocationAudits()
	directoryConsistencyAudits()

	sys.exit(returnCode)
