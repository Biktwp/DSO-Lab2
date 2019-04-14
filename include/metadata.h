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

struct INode;
struct superBlock;

typedef struct INode
{
  char *name;
  unsigned int sizeFile;
  unsigned int blockNumber;
  unsigned int pointer;
}INode;

typedef struct superBlock
{
  unsigned long deviceSize;
  struct INode iNodos[MAX_FILES];
}superBlock;




