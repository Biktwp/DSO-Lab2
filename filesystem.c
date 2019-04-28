/*
 * OPERATING SYSTEMS DESING - 16/17
 *
 * @file 	filesystem.c
 * @brief 	Implementation of the core file system funcionalities and auxiliary functions.
 * @date	01/03/2017
 */

#include "include/filesystem.h" // Headers for the core functionality
#include "include/auxiliary.h"  // Headers for auxiliary functions
#include "include/metadata.h"   // Type and structure declaration of the file system
#include <string.h>


struct superBlock1 sBlock1;
struct superBlock2 sBlock2;
char InodeNames[MAX_TOTAL_FILES][MAX_NAME];
//We will create our own bitmaps being integers
unsigned int b_map[MAX_TOTAL_FILES];
unsigned int i_map[MAX_TOTAL_FILES];
//char block [MAX_TOTAL_FILES][BLOCK_SIZE];

/*
 * @brief 	Generates the proper file system structure in a storage device, as designed by the student.
 * @return 	0 if success, -1 otherwise.
 */

int mkFS(long deviceSize)
{
	//Check the device size 
	if (deviceSize < 51200 || deviceSize > 10485760) return -1;
	//Restore the i-nodes map and the device size is store
	sBlock1.deviceSize = deviceSize;
	sBlock1.firstDataBlock = 0;
	strcpy(InodeNames[0],"root");
	strcpy(sBlock1.iNodos[0].name,InodeNames[0]);
	sBlock1.iNodos[0].sizeFile = BLOCK_SIZE;
	sBlock1.iNodos[0].depth = 0;
	sBlock1.iNodos[0].isDirectory = DIR;
	sBlock1.iNodos[0].directBlock = 41;
	sBlock2.iNodos[0].pointer = 0;
	sBlock2.iNodos[0].open = OPEN;
	b_map[0] = 1;
	i_map[0] = 1;
	for (int j = 0; j<MAX_LOCAL_FILES; j++){
		sBlock2.iNodos[0].iNodes[j] = 41;
	}
	//Go through the i-node array and set it as default
	for(unsigned i = 1; i < MAX_TOTAL_FILES; i++){
		strcpy(InodeNames[i],"");
		strcpy(sBlock1.iNodos[i].name,InodeNames[i]);
		sBlock1.iNodos[i].sizeFile = 0;
		sBlock1.iNodos[i].depth = 0;
		sBlock1.iNodos[i].isDirectory = 2;
		sBlock1.iNodos[i].directBlock = 41;
		sBlock2.iNodos[i].pointer = 41;
		sBlock2.iNodos[i].open = CLOSE;
	}

	//Write metadata in disk
	if(unmountFS() == -1) return -1;

	return 0;
	
}

/*
 * @brief 	Mounts a file system in the simulated device.
 * @return 	0 if success, -1 otherwise.
 */
int mountFS(void)
{
	//Read the superblocks from disk
	if(bread(DEVICE_IMAGE,0,((char*)&(sBlock1))) == -1) return -1;
	if(bread(DEVICE_IMAGE,1,((char*)&(sBlock2))) == -1) return -1;

	// read the i-node map from disk
	if(bread(DEVICE_IMAGE, 2, ((char *)i_map)) == -1) return -1;

	// read the block map from disk
	if (bread(DEVICE_IMAGE, 3, ((char *)b_map)) == -1) return -1;

	// read the inode names from disk
	for(int i = 0; i < MAX_TOTAL_FILES; i++){
		if(bread(DEVICE_IMAGE,4,((char*) (&InodeNames))) == -1) return -1;
	}

	return 0;
}

/*
 * @brief 	Unmounts the file system from the simulated device.
 * @return 	0 if success, -1 otherwise.
 */
int unmountFS(void)
{
	//Write the superblocks in disk
	if(bwrite(DEVICE_IMAGE,0,((char*)&(sBlock1))) == -1) return -1;
	if(bwrite(DEVICE_IMAGE,1,((char*)&(sBlock2))) == -1) return -1;

	// wrtie the i-node map to the disk
	if(bwrite(DEVICE_IMAGE, 2, ((char *)i_map)) == -1) return -1;

	// write the block map to the disk
	if (bwrite(DEVICE_IMAGE, 3, ((char *)b_map)) == -1) return -1;

	// write the inode names to the disk
	for(int i = 0; i < MAX_TOTAL_FILES; i++){
		if(bwrite(DEVICE_IMAGE,4,((char*) (&InodeNames))) == -1) return -1;
	}

	return 0;
}

/*
 * @brief	Creates a new file, provided it it doesn't exist in the file system.
 * @return	0 if success, -1 if the file already exists, -2 in case of error.
 */
int createFile(char *path)
{
	//Check if the path has a correct format
	if(path == NULL || strcmp(path, "") == 0 || path[0] != 47 || strlen(path)>132){
		return -2;
	}

	char aux[132];
	strcpy(aux, path);

	//We check the path and get the file name in case it is correct and the file does not exist
	char *check = ""; //Store the name of each directory in the path and the file
	check = strtok(aux, "/");
	unsigned int found = 0, parent = 0;
	char *new_name;

	while (check != NULL){
		if(namei(check) < 0){
			found = 0;
		}
		else{
			found = 1;
			parent = namei(check);
		}
		new_name = check;
		check = strtok(NULL, "/");
		if (check == NULL && found == 1){
			return -1;
		}
		else if (check != NULL && found == 0){
			return -2;
		}
	}

	if (sBlock1.iNodos[parent].depth >= MAX_DEPTH){
		return -2;
	}

	//Get an empty file descriptor for the i-node
	int fd = ialloc();
	if(fd < 0){
		return -2;
	}

	//Get an empty id for the new associeted block
	int block_num = alloc();
	if(block_num < 0){
		ifree(fd);
		return -2;
	}

	//Put the refenrece in the parent directory
	for(int i = 0; i<MAX_LOCAL_FILES; i++){
		if(sBlock2.iNodos[parent].iNodes[i] == 41) {
			sBlock2.iNodos[parent].iNodes[i]=fd;
			break;
		}
	}

	//Default values of the new file
	sBlock1.iNodos[fd].isDirectory = FILE;
	strcpy(sBlock1.iNodos[fd].name, new_name);
	sBlock1.iNodos[fd].directBlock = block_num;
	sBlock2.iNodos[fd].open = CLOSE;
	sBlock1.iNodos[fd].depth = sBlock1.iNodos[parent].depth + 1;

	return 0;
}

/*
 * @brief	Deletes a file, provided it exists in the file system.
 * @return	0 if success, -1 if the file does not exist, -2 in case of error..
 */
int removeFile(char *path)
{
	//Check if the path has a correct format
	if(path == NULL || strcmp(path, "") == 0 || path[0] != 47|| strlen(path)>132){
		return -2;
	}

	char aux[132];
	strcpy(aux, path);

	//We check the path and get the file descriptor
	char *check = ""; //Store the name of each directory in the path and the file
	check = strtok(aux, "/");
	int fd;
	while (check != NULL){
		if(namei(check) < 0){
			return -1;
		}
		fd = namei(check);
		check = strtok(NULL, "/");
	}

	//Check teh obtained file descriptor
	if(fd < 0 || fd > MAX_TOTAL_FILES-1 || sBlock1.iNodos[fd].isDirectory == DIR){
		return -2;
	}

	//free the block, the i-node and set te size to 0
	freeblock(sBlock1.iNodos[fd].directBlock);
	memset(&(sBlock1.iNodos[fd].sizeFile), 0, BLOCK_SIZE);
	ifree(fd);

	//Set to null the pointer in the parent directory
	for(int i = 0; i<MAX_TOTAL_FILES; i++){
		for(int j = 0; j < MAX_LOCAL_FILES; j++){
			if(sBlock2.iNodos[i].iNodes[j] == fd){
				sBlock2.iNodos[i].iNodes[j] = 41;
			}
		}
	}

	return 0;
}

/*Break up" the device
 * @brief	Opens an existing file.
 * @return	The file descriptor if possible, -1 if file does not exist, -2 in case of error..
 */
int openFile(char *path)
{

	//Check if the path has a correct format
	if(path == NULL || strcmp(path, "") == 0 || path[0] != 47 || strlen(path)>132){
		return -2;
	}

	char aux[132];
	strcpy(aux, path);

	//We check the path and get the file descriptor
	char *check = ""; //Store the name of each directory in the path and the file
	check = strtok(aux, "/");
	unsigned int inode_i;
	while (check != NULL){
		if(namei(check) < 0){
			return -1;
		}
		//Copy the fd of the file, in case the next is NULL and everything before was true
		//Means that this is our file and we need its file descriptor
		inode_i = namei(check);
		check = strtok(NULL, "/");
	}

	//Check if the file descriptor is correct
	if(inode_i < 0 || inode_i > MAX_TOTAL_FILES-1){
		return -1;
	}
	
	//Check if the path is a file or a directory
	if(sBlock1.iNodos[inode_i].isDirectory == DIR){
		return -2;
	}

	//Check if the file is already open
	if(sBlock2.iNodos[inode_i].open == OPEN){
		return -2;
	}

	//Change the status of the file to OPEN
	sBlock2.iNodos[inode_i].open = OPEN;

	//Put the file pointer at the begining of the file
	if(lseekFile(inode_i, 0, FS_SEEK_BEGIN)<0){
		return -1;
	}

	return inode_i;

}

/*
 * @brief	Closes a file.
 * @return	0 if success, -1 otherwise.
 */
int closeFile(int fileDescriptor)
{

	//Check if the file descriptor is valid
	if(fileDescriptor < 0 || fileDescriptor > MAX_TOTAL_FILES-1){
		return -1;
	}
	
	//Check if the path is a file or a directory
	if(sBlock1.iNodos[fileDescriptor].isDirectory == DIR){
		return -1;
	}

	//Check if the file is already close
	if(sBlock2.iNodos[fileDescriptor].open == CLOSE){
		return -1;
	}

	//Close the file
	sBlock2.iNodos[fileDescriptor].open = CLOSE;

	return 0;
	
}

/*
 * @brief	Reads a number of bytes from a file and stores them in a buffer.
 * @return	Number of bytes properly read, -1 in case of error.
 */
int readFile(int fileDescriptor, void *buffer, int numBytes)
{
	//Check if the file descriptor is valid
	if(fileDescriptor < 0 || fileDescriptor > MAX_TOTAL_FILES-1){
		return -1;
	}
	
	//Check if the path is a file or a directory
	if(sBlock1.iNodos[fileDescriptor].isDirectory == DIR){
		return -1;
	}
	
	//Checck the file number of bytes
	if (numBytes < 0 || numBytes > MAX_FILE_SIZE){
		return -1;
	}

	//Check if the file is open
	if (sBlock2.iNodos[fileDescriptor].open == CLOSE){
		return -1;
	}

	//We create a Char block to read the chacters from the file
	char block[BLOCK_SIZE];
	int block_id;

	if(sBlock2.iNodos[fileDescriptor].pointer + numBytes > sBlock1.iNodos[fileDescriptor].sizeFile){
		return -1;
	}

	block_id = bmap(fileDescriptor, sBlock2.iNodos[fileDescriptor].pointer);

	//Get the image from the device and poffsetut the content in the buffer
	bread(DEVICE_IMAGE, block_id, block);

	//Read the bytes of the block and put them in the buffer
	memmove(buffer, block, numBytes);

	return numBytes;
}

/*
 * @brief	Writes a number of bytes from a buffer and into a file.
 * @return	Number of bytes properly written, -1 in case of error.
 */
int writeFile(int fileDescriptor, void *buffer, int numBytes)
{
	//Check if the file descriptor is valid
	if (fileDescriptor < 0 || fileDescriptor > MAX_TOTAL_FILES-1){
		return -1;
	}
	
	//Check if the path is a file or a directory
	if(sBlock1.iNodos[fileDescriptor].isDirectory == DIR){
		return -1;
	}

	//Checck the write number of bytes
	if (numBytes < 0 || numBytes > MAX_FILE_SIZE){
		return -1;
	}

	//Check if the file is open
	if (sBlock2.iNodos[fileDescriptor].open == CLOSE){
		return -1;
	}

	//char block [BLOCK_SIZE];
	//int block_id;

	if(sBlock2.iNodos[fileDescriptor].pointer + numBytes > sBlock1.iNodos[fileDescriptor].sizeFile){
		return -1;
	}


	
	return numBytes;
}

/*
 * @brief	Modifies the position of the seek pointer of a file.
 * @return	0 if succes, -1 otherwise.
 */
int lseekFile(int fileDescriptor, long offset, int whence)
{

	//Check if the file descriptor is valid
	if (fileDescriptor < 0 || fileDescriptor > MAX_TOTAL_FILES-1){
		return -1;
	}
	
	//Check if the path is a file or a directory
	if(sBlock1.iNodos[fileDescriptor].isDirectory == DIR){
		return -1;
	}
	
	//Check if the offset is bigger than the maximum file size. 
	if((unsigned int)offset > MAX_FILE_SIZE-1){
		return -1;
	}

	//We need to check the whence number
	//Change the pointer offset possitions from the current one
	if(whence == FS_SEEK_CUR){
		if(sBlock2.iNodos[fileDescriptor].pointer + offset > MAX_FILE_SIZE-1 || sBlock2.iNodos[fileDescriptor].pointer + offset < 0){
			return -1;
		}
		sBlock2.iNodos[fileDescriptor].pointer += offset;
	}
	//Change the pointer to the begining of the file
	if(whence == FS_SEEK_BEGIN){
		sBlock2.iNodos[fileDescriptor].pointer = 0;
	}
	//Change the pointer to the end of the file
	if(whence == FS_SEEK_END){
		sBlock2.iNodos[fileDescriptor].pointer = MAX_FILE_SIZE-1;
	}
	
	return 0;
}

/*
 * @brief	Creates a new directory provided it it doesn't exist in the file system.
 * @return	0 if success, -1 if the directory already exists, -2 in case of error.
 */
int mkDir(char *path)
{
	//Check if the path has a correct format
	if(path == NULL || strcmp(path, "") == 0 || path[0] != 47 || strlen(path)>132){
		return -2;
	}

	char aux[132];
	strcpy(aux, path);

	//We check the path and if the new directory already exits
	char *check = ""; //Store the name of each directory in the path and the file
	check = strtok(aux, "/");
	unsigned int found = 0, parent = 0;
	char *new_name;
	int i = 0;
	while (check != NULL){
		if(namei(check) < 0){
			found = 0;
		}
		else{
			found = 1;
			parent = namei(check);
		}
		new_name = check;
		check = strtok(NULL, "/");
		if (check == NULL && found == 1){
			return -1;
		}
		else if (check != NULL && found == 0){
			return -2;
		}
	}

	//check the depth of the parent directory
	if (sBlock1.iNodos[parent].depth >= MAX_DEPTH){
		return -2;
	}

	//Get an empty file descriptor for the i-node
	int fd = ialloc();
	if(fd < 0){
		return -2;
	}

	//Get an empty id for the new associeted block
	int block_num = alloc();
	if(block_num < 0){
		ifree(fd);
		return -2;
	}

	//Put the refenrece in the parent directory
	for(i = 0; i<MAX_LOCAL_FILES; i++){
		if(sBlock2.iNodos[parent].iNodes[i] == 41) {
			sBlock2.iNodos[parent].iNodes[i]=fd;
			break;
		}
	}

	//Default values of the new directory
	sBlock1.iNodos[fd].isDirectory = DIR;
	strcpy(sBlock1.iNodos[fd].name, new_name);
	sBlock2.iNodos[fd].indirectBlock = block_num;
	sBlock1.iNodos[fd].depth = sBlock1.iNodos[parent].depth + 1;
	for (int j = 0; j<MAX_LOCAL_FILES; j++){
		sBlock2.iNodos[fd].iNodes[j] = 41;
	}

	return 0;
}

/*
 * @brief	Deletes a directory, provided it exists in the file system.
 * @return	0 if success, -1 if the directory does not exist, -2 in case of error..
 */
int rmDir(char *path)
{
	return -2;
}

/*
 * @brief	Lists the content of a directory and stores the inodes and names in arrays.
 * @return	The number of items in the directory, -1 if the directory does not exist, -2 in case of error..
 */
int lsDir(char *path, int inodesDir[10], char namesDir[10][33])
{

	//Check if the path has a correct format
	if(path == NULL || strcmp(path,"") == 0 || path[0] != 47|| strlen(path)>99){
		return -2;
	}

	unsigned int parentDir;

	if(strcmp(path,"/") != 0){

		char aux[132];
		strcpy(aux, path);
		//We check the path and get the direcotry file descriptor
		char *check = ""; //Store the name of each directory in the path and the file
		check = strtok(aux, "/");
		
		while (check != NULL){
			if(namei(check) < 0){
				return -1;
			}
			parentDir = namei(check);
			check = strtok(NULL, "/");
		}
	}

	else{
		parentDir = 0;
	}

	int counter = 0; //A counter to know how many files and directories are in the lsDir

	//Search all the files and directories that have that directory as parent
	for(int i = 0; i<MAX_LOCAL_FILES; i++){
		if(sBlock2.iNodos[parentDir].iNodes[i] != 41){
			//Store the inodes numbers and names in the arrays
			inodesDir[counter] = sBlock2.iNodos[parentDir].iNodes[i];
			strcpy(namesDir[counter], sBlock1.iNodos[inodesDir[counter]].name);
			//Print the files and/or directories
			if(sBlock1.iNodos[inodesDir[counter]].isDirectory == DIR){
				printf("DIR    %s\n", sBlock1.iNodos[inodesDir[counter]].name);	
			}
			else{
				printf("FILE   %s\n", sBlock1.iNodos[inodesDir[counter]].name);
			}
			counter++;
		}
		
	}

	return counter;

}

/*
LOW LEVEL FILE SYSTEM ALGORITHMS
*/

/*
 * namei
 * @brief	returns the file descriptor of a file from the file name.
 * @return	fd if success, -1 in case the file does not exists
 */
int namei(char *fileName) {
	for(int fd = 0; fd < MAX_TOTAL_FILES; fd++) {
		//Loking for the file and retrun the number
		if(strcmp(sBlock1.iNodos[fd].name, fileName) == 0) {
			return fd;
		}
	}
	//In case it is not found
	return -1;
}

/*
 * b_map:
 * @brief	Translate l ogical address into physical address.
 * @return	Direct block number in case of succes and -1 in case of error
 */
int bmap(int inode_id, int offset){

	
	//check if the id is valid
	if(inode_id > MAX_TOTAL_FILES){
		return -1;
	}

//int block[BLOCK_SIZE/4];

	//Find the data block
	if(offset < BLOCK_SIZE){
		return sBlock1.iNodos[inode_id].directBlock;
	}
/*
	//No tengo ni idea de lo que hace aqui hulio, creo que es para directorios
	if (offset < BLOCK_SIZE*BLOCK_SIZE/4){
		bread(DEVICE_IMAGE, sBlock1.iNodos[inode_id].indirectBlock, block);
		offset = (offset-BLOCK_SIZE) / BLOCK_SIZE;
		return block[offset] ;
	}
*/

	return -1;
	
}


/*
 * alloc:
 * @brief	Allocate a free block for teh file
 * @return	returns the block map position in case of succes and -1 in case of error
 */
int alloc(void){
	
	char block[BLOCK_SIZE];

	for (int i = 0; i<MAX_TOTAL_FILES; i++){
		if(b_map[i] == 0){
			//That block is not busy, now it will be
			b_map[i] = 1;
			memset(block, 0, BLOCK_SIZE);
			bwrite(DEVICE_IMAGE, i+sBlock1.firstDataBlock, block);
			return i;
		}
	}
	return -1;
}


/*
 * ialloc:
 * @brief	Allocate a free i-node for a file
 * @return	returns the i-node map position in case of succes and -1 in case of error
 */

int ialloc (void){
	
	//Look for a free i-node
	for (int i =0; i<MAX_TOTAL_FILES; i++){
		if (i_map[i] == 0){
			//free i-node found
			i_map[i] = 1;
			memset(&(sBlock1.iNodos[i]), 0, sizeof(INode));
			return i;
		}
	}
	return -1;
}

/*
 * free:
 * @brief	free an allocated block
 * @return	return 0 in case od succes and -1 in case of error
 */

int freeblock(int block_id){
	
	//check if the id is valid
	if(block_id >MAX_TOTAL_FILES){
		return -1;
	}
	//Free the block
	b_map[block_id] = 0;
	
	return 0;
}

/*
 * ifree:
 * @brief	free an allocated i-node
 * @return	return 0 in case od succes and -1 in case of error
 */

int ifree(int inode_id){
	
	//check if the id is valid
	if(inode_id >MAX_TOTAL_FILES){
		return -1;
	}
	//Free the i-node
	i_map[inode_id] = 0;
	
	return 0;
}