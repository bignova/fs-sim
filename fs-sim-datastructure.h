#if !defined(FS_SIM_DATASTRUCTURE)
#define FS_SIM_DATASTRUCTURE

/* The File structure defines the files in the simulated system. 
 *
 * name: The pointer points to the name of this file
 * next: Since files are saved in the directory using linked-list structue, this
 *       is a file pointer points to the next file in the current direcotry.
 */
typedef struct file {
  char *name;
  struct file *next;
} File;

/*
 * The Directory strucute defines the directories in the simulated system.
 *
 * name: The pointer points to the name of this directory.
 * parent: A directory pointer points to parent directory of the current one.
 * sub: A directory pointer points to the head of the linked list of the current
 *      directory's all sub directories.
 * next: A directory pointer points to the next sub directory of the current
 *       directory's parent.
 * f_head: A file pointer points to the head of the linked list of the files
 *         saved in the current directory.
 */
typedef struct directory {
  char *name;
  struct directory *parent;
  struct directory *sub;
  struct directory *next;
  File *f_head;
} Directory;

/* Fs_sim is defined as the pointer type of the Directory structure. */
typedef Directory *Fs_sim;

#endif
