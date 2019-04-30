/*
 * OPERATING SYSTEMS DESING - 16/17
 *
 * @file 	auxiliary.h
 * @brief 	Headers for the auxiliary functions required by filesystem.c.
 * @date	01/03/2017
 */


/*
 * b_map:
 * @brief	Translatel ogical address into physical address.
 * @return	return 0 in case od succes and -1 in case of error
 */
int bmap(int inode_id, int offset);

/*
 * alloc:
 * @brief	Allocate a free block for teh file
 * @return	returns the block map position in case of succes and -1 in case of error
 */
int alloc();

/*
 * ialloc:
 * @brief	Allocate a free i-node for a file
 * @return	returns the i-node map position in case of succes and -1 in case of error
 */
int ialloc();

/*
 * free:
 * @brief	free an allocated block
 * @return	return 0 in case od succes and -1 in case of error
 */

int freeblock(int block_id);


/*
 * ifree:
 * @brief	free an allocated i-node
 * @return	return 0 in case od succes and -1 in case of error
 */
int ifree(int inode_id);

/*
 * existDir
 * @brief	returns the file descriptor of a directory from the directory name.
 * @return	fd if success, -1 in case the directory does not exists
 */
int existDir(int depth);

/*
 * existFile
 * @brief	returns the file descriptor of a file from the file name.
 * @return	fd if success, -1 in case the directory does not exists
 */
int existFile(int depth);