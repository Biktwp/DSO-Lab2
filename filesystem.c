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

/*
 * @brief 	Generates the proper file system structure in a storage device, as designed by the student.
 * @return 	0 if success, -1 otherwise.
 */

struct superBlock sBlock;
struct INode inodes;
char InodeNames [MAX_FILES][MAX_NAME];



int mkFS(long deviceSize)
{
	//Check the device size 
	if (deviceSize < 51200 || deviceSize > 10485760) return -1;
	
	//Restore the i-nodes map and the device size is store
	sBlock.deviceSize = deviceSize;

	//Go through the i-node array and set it as default
	for(unsigned i = 0; i < MAX_FILES; i++){
		sBlock.iNodos[i].name = InodeNames[i];
		strcpy(InodeNames[i],"");
		sBlock.iNodos[i].open = CLOSE;
	}
	
	//"Break up" the device
	if(unmountFS() == -1) return -1;

	return 0;
	
}

/*
 * @brief 	Mounts a file system in the simulated device.
 * @return 	0 if success, -1 otherwise.
 */
int mountFS(void)
{
	//Read the metadata blocks from the disk
	if(bread(DEVICE_IMAGE,0,((char*)(&(sBlock))))== -1) return -1;
	if(bread(DEVICE_IMAGE,1,((char*)(&(InodeNames))))==-1) return -1;
	return 0;
}

/*
 * @brief 	Unmounts the file system from the simulated device.
 * @return 	0 if success, -1 otherwise.
 */
int unmountFS(void)
{
	//Write in the device the metadata blocks
	if(bwrite(DEVICE_IMAGE,0,((char*)(&(sBlock))))== -1) return -1;
	if(bwrite(DEVICE_IMAGE,1,((char*)(&(InodeNames))))==-1) return -1;
	return 0;
}

/*
 * @brief	Creates a new file, provided it it doesn't exist in the file system.
 * @return	0 if success, -1 if the file already exists, -2 in case of error.
 */
int createFile(char *path)
{
	for(int i = 0; i < MAX_FILES; i++){
		if(strcmp(sBlock.iNodos[i].name,path) == 0) return -1; 
	}
	return -2;
}

/*
 * @brief	Deletes a file, provided it exists in the file system.
 * @return	0 if success, -1 if the file does not exist, -2 in case of error..
 */
int removeFile(char *path)
{
	return -2;
}

/*
 * @brief	Opens an existing file.
 * @return	The file descriptor if possible, -1 if file does not exist, -2 in case of error..
 */
int openFile(char *path)
{
	//Get the i-node from the path name
	int inode_i = namei(path);

	//Check if the file exists
	if(inode_i < 0){
		return -1;
	}

	//Check if the file is already open
	if(sBlock.iNodos[inode_i].open == OPEN){
		return -2;
	}

	//Change the status of the file to OPEN
	sBlock.iNodos[inode_i].open = OPEN;

	/*
	¿¿Habria que poner aqui un lseek al begin del file??
	*/

	return inode_i;
}

/*
 * @brief	Closes a file.
 * @return	0 if success, -1 otherwise.
 */
int closeFile(int fileDescriptor)
{
	//Check if the file descriptor is valid
	if(fileDescriptor < 0 || fileDescriptor > MAX_FILES-1){
		return -1;
	}

	//Check if the file is already close
	if(sBlock.iNodos[fileDescriptor].open == CLOSE){
		return -1;
	}

	//Close the file
	sBlock.iNodos[fileDescriptor].open == CLOSE;

	return 0;
}

/*
 * @brief	Reads a number of bytes from a file and stores them in a buffer.
 * @return	Number of bytes properly read, -1 in case of error.
 */
int readFile(int fileDescriptor, void *buffer, int numBytes)
{
	//Check if the file descriptor is valid
	if(fileDescriptor < 0 || fileDescriptor > MAX_FILES-1){
		return -1;
	}
	//Checck the file number of bytes
	if (numBytes < 0 || numBytes > MAX_FILE_SIZE){
		return -1;
	}

	//Check if the file is open
	if (sBlock.iNodos[fileDescriptor].open == CLOSE){
		return -1;
	}

	/*
	No pillo de los i-node map
	*/

	//We create a Char block to read the chacters from the file
	char block[BLOCK_SIZE];
	//Get the image from the device and put the content in the buffer
	bread(DEVICE_IMAGE, fileDescriptor, block);
	//Reaf the bytes of the block and put them in the buffer
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
	if (fileDescriptor < 0 || fileDescriptor > MAX_FILES-1){
		return -1;
	}

	//Checck the file number of bytes
	if (numBytes < 0 || numBytes > MAX_FILE_SIZE){
		return -1;
	}

	//Check if the file is open
	if (sBlock.iNodos[fileDescriptor].open == CLOSE){
		return -1;
	}

	/*
	i-node map??
	*/

	

	return -1;
}

/*
 * @brief	Modifies the position of the seek pointer of a file.
 * @return	0 if succes, -1 otherwise.
 */
int lseekFile(int fileDescriptor, long offset, int whence)
{
	//Check if the file descriptor is valid
	if (fileDescriptor < 0 || fileDescriptor > MAX_FILES-1){
		return -1;
	}

	//We need to check the whence number
	//Change the pointer offset possitions from the current one
	if(whence == FS_SEEK_CUR){
		if(sBlock.iNodos[fileDescriptor].pointer + offset > MAX_FILES-1 || sBlock.iNodos[fileDescriptor].pointer + offset < 0){
			return -1;
		}
		sBlock.iNodos[fileDescriptor].pointer += offset;
	}
	//Change the pointer to the begining of the file
	if(whence == FS_SEEK_BEGIN){
		sBlock.iNodos[fileDescriptor].pointer = 0;
	}
	//Change the pointer to the end of the file
	if(whence == FS_SEEK_END){
		sBlock.iNodos[fileDescriptor].pointer = MAX_FILE_SIZE-1;
	}
	return 0;
}

/*
 * @brief	Creates a new directory provided it it doesn't exist in the file system.
 * @return	0 if success, -1 if the directory already exists, -2 in case of error.
 */
int mkDir(char *path)
{
	return -2;
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
	return -2;
}

/*
OUR FUNCTIONS
*/

/*
namei: returns the file descriptor of a file from the path name
*/
int namei(char *fileName) {
	for(int fd = 0; fd < MAX_FILES; fd++) {
		//Loking for the file and retrun the number
		if(strcmp(sBlock.iNodos[fd].name, fileName) == 0) {
			return fd;
		}
	}
	//In case it is not found
	return -1;
}