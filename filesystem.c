/*
 * OPERATING SYSTEMS DESING - 16/17
 *
 * @file 	filesystem.c
 * @brief 	Implementation of the core file system funcionalities and auxiliary functions.
 * @date	01/03/2017 superBlock3
 */

#include "include/filesystem.h" // Headers for the core functionality
#include "include/auxiliary.h"  // Headers for auxiliary functions
#include "include/metadata.h"   // Type and structure declaration of the file system
#include <string.h>


struct superBlock1 sBlock1;
struct superBlock2 sBlock2;
struct superBlock3 sBlock3;
char InodeNames[MAX_TOTAL_FILES][MAX_NAME];
//We will create our own bitmaps being integers
unsigned int b_map[MAX_TOTAL_FILES];
unsigned int i_map[MAX_TOTAL_FILES];
char directory1[MAX_NAME+1];
char directory2[MAX_NAME+1];
char directory3[MAX_NAME+1];
char file[MAX_NAME];
//char block [MAX_TOTAL_FILES][BLOCK_SIZE];

/*
 * @brief 	Generates the proper file system structure in a storage device, as designed by the student.
 * @return 	0 if success, -1 otherwise.
 */

int mkFS(long deviceSize)
{
	//Check the device size 
	if (deviceSize < 51200 || deviceSize > 10485760){ 
		printf("ERROR: Invalid device size\n"); 
		return -1;
		}
	//Restore the i-nodes map and the device size is store
	sBlock1.deviceSize = deviceSize;
	sBlock1.firstDataBlock = 0;
	strcpy(InodeNames[0],"root");
	strcpy(sBlock1.iNodos[0].name,InodeNames[0]);
	sBlock1.iNodos[0].sizeFile = BLOCK_SIZE;
	sBlock1.iNodos[0].depth = 0;
	sBlock1.iNodos[0].isDirectory = DIR;
	sBlock1.iNodos[0].directBlock = 0;
	sBlock2.iNodos[0].pointer = 0;
	sBlock3.iNodos[0].open = OPEN;
	sBlock3.iNodos[0].parent = 0;
	b_map[0] = 1;
	i_map[0] = 1;
	for (int j = 0; j<MAX_LOCAL_FILES; j++){
		sBlock2.iNodos[0].iNodes[j] = 41;
	}
	//Go through the i-node array and set it as default
	for(unsigned i = 1; i < MAX_TOTAL_FILES; i++){
		strcpy(InodeNames[i],"");
		strcpy(sBlock1.iNodos[i].name,InodeNames[i]);
		sBlock1.iNodos[i].directBlock = 41;
	}

	//Write metadata in disk
	if(unmountFS() == -1){
		printf("ERROR: Cannot unmount the file system"); 
		return -1;
	}
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
	if(bread(DEVICE_IMAGE,2,((char*)&(sBlock3))) == -1) return -1;

	// read the i-node map from disk
	if(bread(DEVICE_IMAGE, 3, ((char *)i_map)) == -1) return -1;

	// read the block map from disk
	if (bread(DEVICE_IMAGE, 4, ((char *)b_map)) == -1) return -1;

	// read the inode names from disk
	if(bread(DEVICE_IMAGE,5,((char*) (&InodeNames))) == -1) return -1;

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
	if(bwrite(DEVICE_IMAGE,2,((char*)&(sBlock3))) == -1) return -1;

	// wrtie the i-node map to the disk
	if(bwrite(DEVICE_IMAGE, 3, ((char *)i_map)) == -1) return -1;

	// write the block map to the disk
	if (bwrite(DEVICE_IMAGE, 4, ((char *)b_map)) == -1) return -1;

	// write the inode names to the disk
	for(int i = 0; i < MAX_TOTAL_FILES; i++){
		if(bwrite(DEVICE_IMAGE,5,((char*) (&InodeNames))) == -1) return -1;
	}

	return 0;
}

/*
 * @brief	Creates a new file, provided it it doesn't exist in the file system.
 * @return	0 if success, -1 if the file already exists, -2 in case of error.
 */
int createFile(char *path)
{
	//namei();
	//Check if the path has a correct format
	if(path == NULL || strcmp(path, "") == 0 || path[0] != 47 || strlen(path)>132){
		printf("ERROR: Invalid path\n");
		return -2;
	}

	unsigned depth = 0;
	char aux[132];
	char aux1[33];
	strcpy(aux, path);
	strcpy(file,"");
	strcpy(directory1,"");
	strcpy(directory2,"");
	strcpy(directory3,"");
	strcpy(aux1,"");

	//We check the path and if the new directory already exits
	char *check = ""; //Store the name of each directory in the path and the file
	char *save = "";
	check = strtok(aux, "/");
	while (check != NULL){
		if(strlen(check)>32){
			printf("ERROR: Max directory or file name is 32\n"); 
			return -2;
		}
		if(depth == 0){
			save = check;
			check = strtok(NULL, "/");
			if(check == NULL) strcpy(file,save);
			else strcpy(aux1,save);
			printf("DIRECTORIO %s\n",directory1);
		}
		else if (depth == 1)
		{
			save = check;
			check = strtok(NULL, "/");
			if(check == NULL) strcpy(file,save);
			else strcpy(directory2,save);
		}
		else if (depth == 2)
		{
			save = check;
			check = strtok(NULL, "/");
			if(check == NULL)  strcpy(file,save);
			else  strcpy(directory3,save);
		}
		else
		{
			strcpy(file,check);
			check = strtok(NULL, "/");
		}
		depth++;
	}
	strcpy(directory1,aux1);
	printf("%s %ld %s %s\n",directory1,strlen(directory3),directory2,directory3);
	if(existFile(depth-1) > 0){
		printf("ERROR: Cannot create a file in a file\n");
		return -2;
	}
	if(strcmp(directory1,"") && depth == 1) strcpy(directory1,"root");	
	if (depth - 1 > MAX_DEPTH){
		printf("ERROR: The depth of the file system is 3\n");
		return -2;
	}

	int ret = existDir(depth-1);
	printf("%d\n",ret);
	if(ret <= -1){
		printf("ERROR: The path does not exist\n");
		return -1;
	}
	unsigned int  parent = 0;
	int full = -1;
	if(depth == 1) parent = 0;
	else parent = existDir(depth-1);
	//Put the refenrece in the parent directory
	for(int i = 0; i<MAX_LOCAL_FILES; i++){
		//printf("%d ",sBlock2.iNodos[parent].iNodes[i]);
		if(sBlock2.iNodos[parent].iNodes[i] == 41) {
			full = i;
			break;
		}
	}
	//printf("\n");
	if(full == -1){
		printf("ERROR: Max number of local files and directories is 10\n");
		return -2;
	}


	//Get an empty file descriptor for the i-node
	int fd = ialloc();
	if(fd < 0){
		printf("ERROR: Max number of files and directories is 40\n");
		return -2;
	}

	//Get an empty id for the new associeted block
	int block_num = alloc();
	if(block_num < 0){
		ifree(fd);
		return -2;
	}

	sBlock2.iNodos[parent].iNodes[full] = fd;

	//Default values of the new file
	sBlock1.iNodos[fd].sizeFile = FILE_SIZE;
	sBlock1.iNodos[fd].isDirectory = FILE;
	strcpy(sBlock1.iNodos[fd].name, file);
	sBlock1.iNodos[fd].directBlock = block_num;
	sBlock3.iNodos[fd].open = CLOSE;
	sBlock1.iNodos[fd].depth = depth;
	sBlock3.iNodos[fd].parent = parent;
	return 0;
}

/*
 * @brief	Deletes a file, provided it exists in the file system.
 * @return	0 if success, -1 if the file does not exist, -2 in case of error.
 */
int removeFile(char *path)
{
	//Check if the path has a correct format
	if(path == NULL || strcmp(path, "") == 0 || path[0] != 47|| strlen(path)>132){
		printf("ERROR: Invalid path\n");
		return -2;
	}
	
	unsigned depth = 0;
	char aux[132];
	strcpy(aux, path);
	strcpy(file,"");
	strcpy(directory1,"");
	strcpy(directory2,"");
	strcpy(directory3,"");

	//We check the path and get the file descriptor
	char *check = ""; //Store the name of each directory in the path and the file
	char *save = "";
	check = strtok(aux, "/");
	while (check != NULL){
		if(strlen(check)>32){
			printf("ERROR: Max directory or file name is 32\n"); 
			return -2;
		}
		if(depth == 0){
			save = check;
			check = strtok(NULL, "/");
			if(check == NULL) strcpy(file,save);
			else strcpy(directory1,save);
		}
		else if (depth == 1)
		{
			save = check;
			check = strtok(NULL, "/");
			if(check == NULL) strcpy(file,save);
			else strcpy(directory2,save);
		}
		else if (depth == 2)
		{
			save = check;
			check = strtok(NULL, "/");
			if(check == NULL)  strcpy(file,save);
			else  strcpy(directory3,save);
		}
		else
		{
			strcpy(file,check);
			check = strtok(NULL, "/");
		}
		depth++;
	}

	int fd = existFile(depth);
	//Check teh obtained file descriptor
	if(fd < 0){
		printf("ERROR: File not found\n");
		return -1;
	}
	if(fd > MAX_TOTAL_FILES-1 || sBlock1.iNodos[fd].isDirectory == DIR){
		printf("ERROR: %s is a directory\n",file);
		return -2;
	}
	//free the block, the i-node and set te size to 0
	freeblock(sBlock1.iNodos[fd].directBlock);
	sBlock1.iNodos[fd].sizeFile = 0;
	ifree(fd);
	//memset(&(sBlock1.iNodos[fd].sizeFile), 0, BLOCK_SIZE);
	
	printf("FILE %s DELETED\n",path);
	return 0;
}

/*Break up the device
 * @brief	Opens an existing file.
 * @return	The file descriptor if possible, -1 if file does not exist, -2 in case of error..
 */
int openFile(char *path)
{

	//Check if the path has a correct format
	if(path == NULL || strcmp(path, "") == 0 || path[0] != 47 || strlen(path)>132){
		printf("ERROR: Invalid path\n");
		return -2;
	}

	unsigned depth = 0;
	char aux[132];
	strcpy(aux, path);
	strcpy(file,"");
	strcpy(directory1,"");
	strcpy(directory2,"");
	strcpy(directory3,"");

	//We check the path and get the file descriptor
	char *check = ""; //Store the name of each directory in the path and the file
	char *save = "";
	check = strtok(aux, "/");
	while (check != NULL){
		if(strlen(check)>32){
			printf("ERROR: Max directory or file name is 32\n"); 
			return -2;
		}
			
		if(depth == 0){
			save = check;
			check = strtok(NULL, "/");
			if(check == NULL) strcpy(file,save);
			else strcpy(directory1,save);
		}
		else if (depth == 1)
		{
			save = check;
			check = strtok(NULL, "/");
			if(check == NULL) strcpy(file,save);
			else strcpy(directory2,save);
		}
		else if (depth == 2)
		{
			save = check;
			check = strtok(NULL, "/");
			if(check == NULL)  strcpy(file,save);
			else  strcpy(directory3,save);
		}
		else
		{
			strcpy(file,check);
			check = strtok(NULL, "/");
		}
		depth++;
	}

	int inode_i = existFile(depth);

	//Check if the file descriptor is correct
	if(inode_i < 0 || inode_i > MAX_TOTAL_FILES-1){
		printf("ERROR: Invalid file\n");
		return -1;
	}
	
	//Check if the path is a file or a directory
	if(sBlock1.iNodos[inode_i].isDirectory == DIR){
		printf("ERROR: Invalid file is a directory\n");
		return -2;
	}

	//Check if the file is already open
	if(sBlock3.iNodos[inode_i].open == OPEN){
		printf("ERROR: File already open\n");
		return inode_i;
	}

	//Change the status of the file to OPEN
	sBlock3.iNodos[inode_i].open = OPEN;

	//Put the file pointer at the begining of the file
	if(lseekFile(inode_i, 0, FS_SEEK_BEGIN)<0){
		return -2;
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
		printf("ERROR: Invalid file\n");
		return -1;
	}
	
	//Check if the path is a file or a directory
	if(sBlock1.iNodos[fileDescriptor].isDirectory == DIR){
		printf("ERROR: It is a directory\n");
		return -1;
	}

	//Check if the file is already close
	if(sBlock3.iNodos[fileDescriptor].open == CLOSE){
		printf("ERROR: The file is already close\n");
		return 0;
	}

	//Close the file
	sBlock3.iNodos[fileDescriptor].open = CLOSE;

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
		printf("ERROR: Invalid file descriptor\n");
		return -1;
	}
	
	//Check if the path is a file or a directory
	if(sBlock1.iNodos[fileDescriptor].isDirectory == DIR){
		printf("ERROR: It is a directory\n");
		return -1;
	}
	
	//Check the file number of bytes
	if (numBytes < 0 || numBytes > MAX_FILE_SIZE){
		printf("ERROR: Invalid number of bytes\n");
		return -1;
	}

	//Check if the file is open
	if (sBlock3.iNodos[fileDescriptor].open == CLOSE){
		printf("ERROR: The file is closed\n");
		return -1;
	}

	//We create a Char block to read the chacters from the file
	char block[BLOCK_SIZE];
	int block_id;

	if(sBlock2.iNodos[fileDescriptor].pointer + numBytes > sBlock1.iNodos[fileDescriptor].sizeFile){
		printf("ERROR: Max file size 2048 bytes\n");
		return -1;
	}

	block_id = bmap(fileDescriptor, sBlock2.iNodos[fileDescriptor].pointer);

	//Get the image from the device and poffsetut the content in the buffer
	if(bread(DEVICE_IMAGE, block_id, block)==-1)return -1;
	//create an auxiliar block to copy only the bytes from the i-node pointer
	char blockAux[BLOCK_SIZE];

	for(int i = 0; i<numBytes; i++){
		blockAux[i]=block[i+sBlock2.iNodos[fileDescriptor].pointer];
	}
	//Read the bytes of the block and put them in the buffer
	memmove(buffer, blockAux, numBytes);
	printf("%s\n",blockAux);

	lseekFile(fileDescriptor, numBytes, FS_SEEK_CUR);
	
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
		printf("ERROR: Invalid file descriptor\n");
		return -1;
	}
	
	//Check if the path is a file or a directory
	if(sBlock1.iNodos[fileDescriptor].isDirectory == DIR){
		printf("ERROR: It is a directory\n");
		return -1;
	}

	//Checck the write number of bytes
	if (numBytes < 0 || numBytes > MAX_FILE_SIZE){
		printf("ERROR: Invalid number of bytes\n");
		return -1;
	}

	//Check if the file is open
	if (sBlock3.iNodos[fileDescriptor].open == CLOSE){
		printf("ERROR: The file is closed\n");
		return -1;
	}

	if(sBlock2.iNodos[fileDescriptor].pointer + numBytes > sBlock1.iNodos[fileDescriptor].sizeFile){
		printf("ERROR: Max file size 2048 bytes\n");
		return -1;
	}

	//Se escribe en un bloque el buffer que almacena lo que se desa escribir
	char block[BLOCK_SIZE];
	unsigned int block_num;

	block_num = bmap(fileDescriptor, sBlock2.iNodos[fileDescriptor].pointer);

	//create an auxiliar block to copy only the bytes from the i-node pointer
	char blockAux[BLOCK_SIZE];

	if (bread(DEVICE_IMAGE, block_num, block) == -1) return -1;

	//Read the bytes of the buffer and put them in the aux. block
	memmove(blockAux, buffer, BLOCK_SIZE);
	
	for(int i = 0; i<numBytes; i++){
		block[i+sBlock2.iNodos[fileDescriptor].pointer] = blockAux[i];
	}

	//Write the written buffer in the memory
	if (bwrite(DEVICE_IMAGE, block_num, block) == -1) return -1;

	lseekFile(fileDescriptor, numBytes, FS_SEEK_CUR);

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
		printf("ERROR: Invalid file descriptor\n");
		return -1;
	}
	
	//Check if the path is a file or a directory
	if(sBlock1.iNodos[fileDescriptor].isDirectory == DIR){
		printf("ERROR: It is a directory\n");
		return -1;
	}
	
	//Check if the offset is bigger than the maximum file size. 
	if((unsigned int)offset > MAX_FILE_SIZE-1){
		printf("ERROR: Invalid offset\n");
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
	//printf("%ld\n",strlen(path));
	//Check if the path has a correct format
	if(path == NULL || strcmp(path, "") == 0 || path[0] != 47 || strlen(path)>99){
		printf("ERROR: Invalid path\n");
		return -2;
	}

	unsigned int i = 0, depth = 0;
	char aux[132];
	strcpy(aux, path);
	strcpy(file,"");
	strcpy(directory1,"");
	strcpy(directory2,"");
	strcpy(directory3,"");
	//We check the path and if the new directory already exits
	char *check = ""; //Store the name of each directory in the path and the file
	check = strtok(aux, "/");
	while (check != NULL){
		if(strlen(check)>32){
			printf("ERROR: Max directory or file name is 32\n"); 
			return -2;
		} 
		if(depth == 0){
			strcpy(directory1,check);
		}
		else if (depth == 1)
		{
			strcpy(directory2,check);
		}
		else
		{
			strcpy(directory3,check);
		}
		check = strtok(NULL, "/");
		depth++;
	}
	//printf("%s %ld %s %s\n",directory1,strlen(directory3),directory2,directory3);
	
	if(existFile(depth-1) > 0){	
		printf("ERROR: Cannot create a directory in a file\n");
		return -2;
	}
	
	if ( depth > MAX_DEPTH){
		printf("ERROR: The depth of the file system is 3\n");
		return -2;
	}
	int ret = existDir(depth);
	if(ret <= -2){
		printf("ERROR: The path does not exist\n");
		return -2;
	}
	if(ret >= 0){
		printf("ERROR: The directory already exist\n");
		return -1;
	}

	unsigned int  parent = 0;
	int full = -1;
	if(depth == 1) parent = 0;
	else parent = existDir(depth - 1);
	if(parent == -1){
		printf("ERROR: The path does not exist\n");
		return -2;
	}
	
	//Put the refenrece in the parent directory
	for(i = 0; i<MAX_LOCAL_FILES; i++){
		if(sBlock2.iNodos[parent].iNodes[i] == 41) {
			full = i;
			break;
		}
	}

	if(full == -1){
		printf("ERROR: Max number of local files and directories is 10\n");
		return -2;
	}



		//Get an empty file descriptor for the i-node
	int fd = ialloc();
	if(fd < 0){
		printf("ERROR: Max number of files and directories is 40\n");
		return -2;
	}

	//Get an empty id for the new associeted block
	int block_num = alloc();
	if(block_num < 0){
		ifree(fd);
		return -2;
	}

	sBlock2.iNodos[parent].iNodes[full] = fd;
	sBlock1.iNodos[fd].directBlock = fd;
	//Default values of the new directory
	sBlock1.iNodos[fd].isDirectory = DIR;
	if(depth == 1){
		strcpy(sBlock1.iNodos[fd].name, directory1);
		sBlock3.iNodos[fd].parent = 0;
	}
	else if (depth == 2)
	{
		strcpy(sBlock1.iNodos[fd].name, directory2);
		sBlock3.iNodos[fd].parent = parent;
	}
	else{
		strcpy(sBlock1.iNodos[fd].name, directory3);
		sBlock3.iNodos[fd].parent = parent;
	}
	sBlock2.iNodos[fd].indirectBlock = block_num;
	sBlock1.iNodos[fd].depth = depth ;
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
	//Check if the path has a correct format
	if(path == NULL || strcmp(path,"") == 0 || path[0] != 47|| strlen(path)>99){
		printf("ERROR: Invalid path\n");		
		return -2;
	}

	unsigned int i = 0, depth = 0, dir = 0;
	char aux[132];
	strcpy(file,"");
	strcpy(directory1,"");
	strcpy(directory2,"");
	strcpy(directory3,"");

	if(strcmp(path,"/") != 0){
	
		strcpy(aux, path);
		//We check the path and if the new directory already exits
		char *check = ""; //Store the name of each directory in the path and the file
		check = strtok(aux, "/");
		while (check != NULL){
			if(strlen(check)>32){
				printf("ERROR: Max directory or file name is 32\n"); 
				return -2;
			}
			if(depth == 0){
				strcpy(directory1,check);
			}
			else if (depth == 1)
			{
				strcpy(directory2,check);
			}
			else
			{
				strcpy(directory3,check);
			}
			check = strtok(NULL, "/");
			depth++;
		}
	}

	else{
		printf("ERROR: Cannot delete the root directory \n");
		return -2;
	}

	if (depth - 1 > MAX_DEPTH){
		printf("ERROR: The depth of the file system is 3\n");
		return -2;
	}

	dir = existDir(depth);
	
	if(dir == -2){	
		printf("ERROR: The path does not exist\n");	
		return -1;
	}

	if(sBlock1.iNodos[dir].isDirectory == FILE){
		printf("IT IS NOT A DIRECTORY\n");
		return -2;
	}

	for(i = 0; i<MAX_LOCAL_FILES; i++){
	
		if(sBlock2.iNodos[dir].iNodes[i] != 41){
			if(sBlock1.iNodos[sBlock2.iNodos[dir].iNodes[i]].isDirectory == FILE){
				sprintf(aux, "%s/%s", path, sBlock1.iNodos[sBlock2.iNodos[dir].iNodes[i]].name);
				removeFile(aux);
			}
			else if(sBlock1.iNodos[sBlock2.iNodos[dir].iNodes[i]].isDirectory == DIR){
				sprintf(aux, "%s/%s", path, sBlock1.iNodos[sBlock2.iNodos[dir].iNodes[i]].name);
				rmDir(aux);
			}
		}
		else
		{
			break;
		}
	}
	printf("DIR %s DELETED\n",path);
	for(i = 0; i < MAX_LOCAL_FILES; i++){
		if(sBlock2.iNodos[dir].iNodes[i] != 41){
			sBlock2.iNodos[dir].iNodes[i] = 41;
		}
		else
		{
			break;
		}
	}
	freeblock(dir);
	ifree(dir);
	return 0;
}

/*
 * @brief	Lists the content of a directory and stores the inodes and names in arrays.
 * @return	The number of items in the directory, -1 if the directory does not exist, -2 in case of error..
 */
int lsDir(char *path, int inodesDir[10], char namesDir[10][33])
{

	//Check if the path has a correct format
	if(path == NULL || strcmp(path,"") == 0 || path[0] != 47|| strlen(path)>99){
		printf("ERROR: Invalid path\n");
		return -2;
	}
	unsigned int depth = 0;
	unsigned int parentDir;
	strcpy(file,"");
	strcpy(directory1,"");
	strcpy(directory2,"");
	strcpy(directory3,"");

	if(strcmp(path,"/") != 0){

		char aux[132];
		strcpy(aux, path);

		//We check the path and if the new directory already exits
		char *check = ""; //Store the name of each directory in the path and the file
		check = strtok(aux, "/");
		while (check != NULL){
			if(strlen(check)>32){
				printf("ERROR: Max directory or file name is 32\n"); 
				return -2;
			}
			if(depth == 0){
				strcpy(directory1,check);
			}
			else if (depth == 1)
			{
				strcpy(directory2,check);
			}
			else
			{
				strcpy(directory3,check);
			}
			check = strtok(NULL, "/");
			depth++;
		}
		parentDir = existDir(depth);
	}

	else{
		parentDir = 0;
	} 
	if(parentDir == -1){
		printf("ERROR: The path does not exist\n");
		return -1;
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
				counter++;
			}
			else if(sBlock1.iNodos[inodesDir[counter]].isDirectory == FILE){
				printf("FILE   %s\n", sBlock1.iNodos[inodesDir[counter]].name);
				counter++;
			}
			
		}
		
	}

	if(counter == 0){
		printf("EMPTY DIRECTORY\n");
	}

	printf("%d FILES AND DIRECTORIES FOUND\n", counter);
	return counter;

}

/*
LOW LEVEL FILE SYSTEM ALGORITHMS
*/

/*
BORRRAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAR
 * namei
 * @brief	returns the file descriptor of a file from the file name.
 * @return	fd if success, -1 in case the file does not exists
 */
int namei() {
	for(int fd = 0; fd < MAX_TOTAL_FILES; fd++) {
		//Loking for the file and retrun the number
		printf("%d %s", fd ,sBlock1.iNodos[fd].name);
		//if(sBlock1.iNodos[fd].directBlock == 41) {
		//	return fd;
		//}	
			printf("\n");
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

	//Find the data block
	if(offset < BLOCK_SIZE){
		return sBlock1.iNodos[inode_id].directBlock + 5;
	}

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
			//printf("%d \n",i_map[i]);
			memset(&(sBlock1.iNodos[i]), 0, sizeof(INode));
			return i;
		}
		//printf("%d ",i_map[i]);
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
	for(int i = 0; i < MAX_LOCAL_FILES; i++){
		//printf("%d %d\n",sBlock2.iNodos[sBlock3.iNodos[inode_id].parent].iNodes[i], sBlock1.iNodos[inode_id].directBlock);
		if(sBlock2.iNodos[sBlock3.iNodos[inode_id].parent].iNodes[i] == sBlock1.iNodos[inode_id].directBlock){
			sBlock2.iNodos[sBlock3.iNodos[inode_id].parent].iNodes[i] = 41;
			break;
		}
	}
	i_map[inode_id] = 0;
	strcpy(InodeNames[inode_id],"");
	strcpy(sBlock1.iNodos[inode_id].name,"");
	sBlock1.iNodos[inode_id].sizeFile = 0;
	sBlock1.iNodos[inode_id].depth = 0;
	sBlock1.iNodos[inode_id].isDirectory = 2;
	sBlock1.iNodos[inode_id].directBlock = 41;
	sBlock2.iNodos[inode_id].pointer = 41;
	sBlock3.iNodos[inode_id].open = CLOSE;
	sBlock3.iNodos[inode_id].parent = 41;
	return 0;
}

int existDir(int depth){
	int aux = -2;
	//printf("depth %d\n",depth);
	for(int fd = 0; fd <MAX_TOTAL_FILES; fd++) {
		//Loking for the file and retrun the number
		printf("parent %s d1 %s\n ",sBlock1.iNodos[fd].name, directory1);
		if(strcmp(sBlock1.iNodos[fd].name, directory1) == 0 && strcmp(sBlock1.iNodos[sBlock3.iNodos[fd].parent].name,"root") == 0 && sBlock1.iNodos[fd].isDirectory == DIR) {
			if(depth >= 2){
				for(int i = 0; i < MAX_LOCAL_FILES; i++){
					int n1 = sBlock2.iNodos[fd].iNodes[i];
					//printf("%d\n",n1);
					printf("parent %s d2 %s %d\n ",sBlock1.iNodos[n1].name,directory2, n1);
					if(strcmp(sBlock1.iNodos[n1].name,directory2) == 0 && sBlock1.iNodos[n1].isDirectory == DIR){
						if(depth >= 3){
							for(int j = 0; j < MAX_LOCAL_FILES; j++){
								int n2 = sBlock2.iNodos[n1].iNodes[j];
								//printf("%d\n",n2);
								printf("parent %s d3 %s \n ",sBlock1.iNodos[n2].name,directory3);
								if(strcmp(sBlock1.iNodos[n2].name,directory3) == 0 && sBlock1.iNodos[n2].isDirectory == DIR){
									return n2;
								}	
								else aux = -1;
							}
							return -1;
						}
						else return n1;
					}
					else aux = -1;
				}
				return -1;
			}
			else return fd;
		}
		else aux = -1;
	}
	return aux;
	/*
	int i = depth, j = -2, save = -1,auxiliar = -1;
	for(;i > 0; i--){
		char *aux1;
		char *aux2;
		if(i == 3){
			aux1 = directory3;
			aux2 = directory2;
		}
		else if (i == 2)
		{
			aux1 = directory2;
			aux2 = directory1;
		}
		else
		{
			aux1 = directory1;
			aux2 = "root";
		}
		for(int fd = 0; fd <MAX_TOTAL_FILES; fd++) {
			//Loking for the file and retrun the number
			if(strcmp(sBlock1.iNodos[fd].name, aux1) == 0 && sBlock1.iNodos[fd].depth == i && strcmp(sBlock1.iNodos[sBlock3.iNodos[fd].parent].name,aux2) == 0 && sBlock1.iNodos[fd].isDirectory == DIR) {
				if(create != -1 && j == -2){
					auxiliar = fd;
				}
				j = fd;
				break;
			}
			
		}	
		if(create == 1 && depth == i && j <= -2) j = -1, save = 0;
		if(create == 1 && depth != i && j == -1) j = -2, save = -2;
	}
	if(save== 0 && create == 1 && j != -2) j = -1;
	if(save == -2 && create == 1 && j != -2) j = -2;
	if(auxiliar != -1 && create != 1) j = auxiliar;
	return j;
	*/
}	

int existFile(int depth){
	//namei();
	int aux = existDir(depth-1);
	if(aux == -1){
		return -1;
	} 
	if(strcmp(file,"")==0)return -1;
	for(int i = 0; i < MAX_LOCAL_FILES;i++){
		int n1 = sBlock2.iNodos[aux].iNodes[i];
		//printf("%d %s\n",n1,sBlock1.iNodos[n1].name);
		if(strcmp(sBlock1.iNodos[n1].name,file) == 0 && sBlock1.iNodos[n1].isDirectory == FILE) return n1;
	}
	return -1;
	/*
	if(strcmp(file,"")==0)return -1;
	//int i = -2;
	char *direct;
	if(depth -1 == 0) direct = "root";
	else if (depth-1 == 1) direct = directory1;
	else if (depth-1 == 2) direct = directory2;
	else direct  = directory3;
		for(int fd = 0; fd < MAX_TOTAL_FILES; fd++) {
			//Loking for the file and retrun the number
			if(strcmp(sBlock1.iNodos[fd].name, file) == 0 && sBlock1.iNodos[fd].depth == depth && strcmp(sBlock1.iNodos[sBlock3.iNodos[fd].parent].name,direct) == 0 && sBlock1.iNodos[fd].isDirectory == FILE) {
				//i = fd;
			}
		}	
		if(existDir(depth - 1,create)==-2){
			if(depth - 1 == 0)return -1;
			return 0;
		}*/
	return -1;
}