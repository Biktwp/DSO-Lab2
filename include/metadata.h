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
#define MAX_DEPTH 3
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
  unsigned int isDirectory; //Check if it is a directory or a file

}INode;

typedef struct INode2
{
  unsigned int pointer; //File pointer to write and read
  unsigned int open; //Indicate if the file is open or not
  /* En los inodos directorio no hay un array de inodos sino que es un array de ints que almacenan las posiciones de los inodos en el superbloque.*/
  unsigned int iNodes[MAX_LOCAL_FILES]; //An array of inodes in case we want to create a directory
}INode2;

typedef struct superBlock1
{
  unsigned long deviceSize; //Size of the edevice
  unsigned int numinodes; //Number of total i-nodes
  unsigned int dataBlockNum; //Number of data blocks
  unsigned int inodeMapNumBlocks; //Number of i-node map blocks
  unsigned int dataMapNumBlocks; //Number of data map blocks
  unsigned int firstDataBlock; //Number of the first data block
  struct INode iNodos[MAX_TOTAL_FILES]; //i-nodes array
}superBlock1;

typedef struct superBlock2
{
  struct INode2 iNodos[MAX_TOTAL_FILES]; //i-nodes array
}superBlock2;

