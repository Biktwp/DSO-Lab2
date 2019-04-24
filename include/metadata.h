/*
 * OPERATING SYSTEMS DESING - 16/17
 *
 * @file 	metadata.h
 * @brief 	Definition of the structures and data types of the file system.
 * @date	01/03/2017
 */

#define bitmap_getbit(bitmap_, i_) (bitmap_[i_ >> 3] & (1 << (i_ & 0x07)))
static inline void bitmap_setbit(char *bitmap_, int i_, int val_) {
  if (val_)
    bitmap_[(i_ >> 3)] |= (1 << (i_ & 0x07));
  else
    bitmap_[(i_ >> 3)] &= ~(1 << (i_ & 0x07));
}


#define MAX_TOTAL_FILES 40
#define MAX_LOCAL_FILES 10
#define MAX_NAME 32
#define DEPTH 3
#define FILE_SIZE 2048
#define BLOCK_SIZE 2048
#define OPEN 1
#define CLOSE 0
#define FILE 0
#define DIR 1

struct INode;
struct superBlock;

typedef struct INode
{
  char name[MAX_NAME]; //File Name
  unsigned int sizeFile; //File size
  unsigned int depth; //Directory depth
  unsigned int blockNumber; //Position in the superblock
  unsigned int parent; //The pointer of the parent directory
  unsigned int isDirectory; //Check if it is a directory or a file
  unsigned int pointer; //File pointer to write and read
  unsigned int open; //Indicate if the file is open or not
  struct INode iNodes[MAX_LOCAL_FILES]; //An array of inodes in case we want to create a directory

}INode;

typedef struct superBlock
{
  unsigned long deviceSize; //Size of the edevice
  struct INode iNodos[MAX_TOTAL_FILES]; //i-nodes array
}superBlock;
