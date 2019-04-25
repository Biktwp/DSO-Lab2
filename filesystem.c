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

struct superBlock sBlock;
struct INode inodes;
char InodeNames [MAX_TOTAL_FILES][MAX_NAME];

/*
 * @brief 	Generates the proper file system structure in a storage device, as designed by the student.
 * @return 	0 if success, -1 otherwise.
 */

int mkFS(long deviceSize)
{
	//Check the device size 
	if (deviceSize < 51200 || deviceSize > 10485760) return -1;
	
	//Restore the i-nodes map and the device size is store
	sBlock.deviceSize = deviceSize;

	//Go through the i-node array and set it as default
	for(unsigned i = 0; i < MAX_TOTAL_FILES; i++){
		strcpy(InodeNames[i],"");
		strcpy(sBlock.iNodos[i].name, InodeNames[i]);
		sBlock.iNodos[i].open = CLOSE;
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
	//Check if the file already exists
	/*for(int i = 0; i < MAX_TOTAL_FILES; i++){
		if(strcmp(sBlock.iNodos[i].name,path) == 0) return -1; 
	}
	*/

	//Check if the path has a correct format
	if(path == NULL || path == "" || path[0] != 77){
		return -2;
	}
	
	/*EXPLICACION:
	lo que intneto hacer es algo asi como, tu mirame que todos los directorios exsiten, y segun vamos encontrando
	pues vamos mirando mas alla en el check. PEEEERO  que en caso de que TODO sea igual, de ahi el: pues si es igual pero se
	acaba el path, significa que TODO EL PATH ES IGUAL por lo tanto ya existe y de ahi el -1. Pero a su vez hay que decir,
	si el final del path no lo encuentra (es decir, el fichero) significa que no existe todavia y por lo tanto puede ser creado
	pero con el loop este raro que me he montado pues si ve que algo no es igual salta error, y el if ese que esta medio empezado
	de mierda significa que algo asi como si el siguiente en check es  NULL y no es igual (o sea, found = 0), significa que el file
	no existe y puede ser creado. La cosa es que creo que es demasiado rebuscado no? en plan esto tiene que ser mas sencillo no me
	jodas JAJAJJA y a parte que cada vez que hacemos un strtok(NULL, "/") ya se carga el char del path, asi q no puedo abusar tanto
	de él (por no decir que solo hay que usarlo par ael while) porque jode rque me jodo el path y en una itneracion llega al NULL.
	Me explico?? Porque joder vaya chapa te estoy metiendo compañero.
	namei compureba si un file o directory existe, en caso de que no, devuelve -1. Por eso uso esa funcion q ya tiene su loop y todo
	en vez de usar mi propio loop.
	*/

	char *check = ""; //Store the name of each directory in the path and the file
	check = strtok(path, "/");
	unsigned int parent;
	while (check != NULL){
		if(namei(check) < 0){
			return -2;
		}
		parent = namei(check);
		check = strtok(NULL, "/");
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

/*Break up" the device
 * @brief	Opens an existing file.
 * @return	The file descriptor if possible, -1 if file does not exist, -2 in case of error..
 */
int openFile(char *path)
{

	//Check if the path has a correct format
	if(path == NULL || path == "" || path[0] != 77){
		return -2;
	}

	//Check if the path exits and in case it exists, get the fd of the directory
	char *check;
	check = strtok(path, "/");
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
	if(sBlock.iNodos[inode_i].isDirectory == DIR){
		return -2;
	}

	//Check if the file is already open
	if(sBlock.iNodos[inode_i].open == OPEN){
		return -2;
	}

	//Change the status of the file to OPEN
	sBlock.iNodos[inode_i].open = OPEN;

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
	if(sBlock.iNodos[fileDescriptor].isDirectory == DIR){
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
	if(fileDescriptor < 0 || fileDescriptor > MAX_TOTAL_FILES-1){
		return -1;
	}
	
	//Check if the path is a file or a directory
	if(sBlock.iNodos[fileDescriptor].isDirectory == DIR){
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
	if (fileDescriptor < 0 || fileDescriptor > MAX_TOTAL_FILES-1){
		return -1;
	}
	
	//Check if the path is a file or a directory
	if(sBlock.iNodos[fileDescriptor].isDirectory == DIR){
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
	if (fileDescriptor < 0 || fileDescriptor > MAX_TOTAL_FILES-1){
		return -1;
	}
	
	//Check if the path is a file or a directory
	if(sBlock.iNodos[fileDescriptor].isDirectory == DIR){
		return -1;
	}
	
	//Check if the offset is bigger than the maximum file size. 
	if((unsigned int)offset > MAX_FILE_SIZE-1){
		return -1;
	}

	//We need to check the whence number
	//Change the pointer offset possitions from the current one
	if(whence == FS_SEEK_CUR){
		if(sBlock.iNodos[fileDescriptor].pointer + offset > MAX_FILE_SIZE-1 || sBlock.iNodos[fileDescriptor].pointer + offset < 0){
			return -1;
		}
		sBlock.iNodos[fileDescriptor].pointer += offset;
	}
	//Change the pointer to the begining of the file
	if(whence == FS_SEEK_BEGIN){
		//In case the offset is neggative, you cannot go less than 0 in a file
		if(offset<0){
			return -1;
		}
		sBlock.iNodos[fileDescriptor].pointer = 0 + offset;
	}
	//Change the pointer to the end of the file
	if(whence == FS_SEEK_END){
		//In case offset is possitive, you cannot go to the next document in a file
		if(offset>0){
			return -1;
		}
		sBlock.iNodos[fileDescriptor].pointer = (MAX_FILE_SIZE-1) + offset;
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

	//Check if the path has a correct format
	if(path == NULL || path == "" || path[0] != 77){
		return -2;
	}

	//Check if the path exits and in case it exists, get the fd of the directory
	char *check = "";
	check = strtok(path, "/");
	unsigned int partentDir;
	while (check != NULL){
		if(namei(check) < 0){
			return -1;
		}
		partentDir = namei(check);
		check = strtok(NULL, "/");
	}

	int counter = 0; //A counter to know how many files and directories are in the lsDir

	//Search all the files and directories that have that directory as parent
	for(int i = 0; i<MAX_LOCAL_FILES; i++){
		if(sBlock.iNodos[i].parent == partentDir){
			//Store the inodes numbers and names in the arrays
			inodesDir[counter] = i;
			strcpy(namesDir[counter], sBlock.iNodos[i].name);
			//Print the files and/or directories
			if(sBlock.iNodos[i].isDirectory == DIR){
				printf("DIR		%s", sBlock.iNodos[i].name);
			}
			else{
				printf("FILE	%s", sBlock.iNodos[i].name);
			}
			counter++;
		}
	}

	return counter;
}

/*
OUR FUNCTIONS
*/

/*
namei: returns the file descriptor of a file from the path name
*/
int namei(char *fileName) {
	for(int fd = 0; fd < MAX_TOTAL_FILES; fd++) {
		//Loking for the file and retrun the number
		if(strcmp(sBlock.iNodos[fd].name, fileName) == 0) {
			return fd;
		}
	}
	//In case it is not found
	return -1;
}