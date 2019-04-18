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


#define MAX_FILES 40
#define MAX_NAME 32
#define DEPTH 3
#define FILE_SIZE 2048
#define BLOCK_SIZE 2048
#define OPEN 1
#define CLOSE 0

struct INode;
struct superBlock;

typedef struct INode
{
  char *name; //File Name
  unsigned int sizeFile; //File size
  unsigned int blockNumber; //Block number (ID??)
  unsigned int pointer; //File pointer 
  unsigned int open; //Indicate if the file is open or not

}INode;

typedef struct superBlock
{
  unsigned long deviceSize; //Size of th edevice
  struct INode iNodos[MAX_FILES]; //i-nodes array
}superBlock;
