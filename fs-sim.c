/*
 * The following functions simulate some of the functionality of the UNIX 
 * filesystem and commands. Users could call these functions to simulate the 
 * file or directory creation, the directory-structured navigation and the files
 * or directories deletion. The filesystem is implemented by linked-list 
 * structure.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fs-sim.h"

/*
 * Four helper (static) functions.
 * 
 * print_list is used to print files and directories with certain format by 
 * typing ls command.
 *
 * check_name is used to check whether if the current directory already
 * contained a same-name file or directory as the paramter arg.
 *
 * destroy_files is used to deallocate all files under the current directory. 
 *
 * destroy_directories is used to deallocate all dynamically allocated memory 
 * under the "top" directory and "top" itself.
 * 
 * Explained more under.
 */
static void print_list(File *file_head, Directory *sub_head);
static int check_name(Fs_sim fs, const char arg[]);
static void destroy_files(File *file_head);
static void destroy_directories(Fs_sim top);

/*
 * mkfs is the initialzed function of simulated filesystem. Before the
 * commands used, it should be first called to initialize a new filesystem
 * which would dynamically allocate the root directory of the filesystem.
 *
 * files: The pointer points to the filesystem defined in main function. Since
 *        Fs_sim is defined as pointer points to directory, files is actually
 *        a double pointer pointed to the current directory in filesystem.
 */
void mkfs(Fs_sim *files)
{
  if (files != NULL)
  {
    *files = malloc(sizeof(Directory));
    if (*files != NULL)
    {
      (*files)->name = NULL;
      (*files)->parent = NULL;
      (*files)->sub = NULL;
      (*files)->next = NULL;
      (*files)->f_head = NULL;
    }
    else
      printf("fail to create the filesystem!\n");
  }
}

/*
 * touch function is used to simulate the touch command in UNIX. It would create
 * a new file as defined in fs-sim-structure.h with parameter arg as the name 
 * of the new file.
 *
 * The function would return 1 if valid arguments passed in and new file
 * created ,and 0 if invalid cases happened (explain more in the function).
 *
 * files: The pointer used to track the current directory in the filesystem.
 * arg: A characters pointer points to name that would be assigned for new file.
 */
int touch(Fs_sim *files, const char arg[])
{
  int result = 0;
  File *curr, *prev = NULL, *new_file = NULL;

  /* Both files and arg need to be valid pointers for use */
  if (files != NULL && *files != NULL && arg != NULL)
  {
    /* If the name is empty, nothing would be created */
    if (!strcmp(arg, ""))
      result = 0;
    /*  
     * If name (arg) is not just a single forward-slash character, then any name
     * contained forward-slash character somewhere would be invalid.
     */
    else if (strcmp(arg, "/") && strchr(arg, '/') != NULL)
      result = 0;
    else if (!strcmp(arg, ".") || !strcmp(arg, "..") || !strcmp(arg, "/"))
      result = 1;
    /* case of a normal valid file name */
    else
    {
      /* 
       * The name of new file cannot be an existing file or directory.
       * Therefore, using the helper function, check_name, to check all names of
       * files and sub directories in the current directory. If no repetition,
       * go for the linked list insertion. 
       */
      if (!check_name(*files, arg))
      {
        curr = (*files)->f_head;

        /* Inserting new file into the linkedlist in increasing order */
        while (curr != NULL && strcmp(arg, curr->name) > 0)
        {
          prev = curr;
          curr = curr->next;
        }

        new_file = malloc(sizeof(*new_file));
        if (new_file != NULL)
        {
          new_file->name = malloc(strlen(arg) + 1);
          if (new_file->name != NULL)
          {
            result = 1;

            strcpy(new_file->name, arg);
            new_file->next = curr;

            /* handling empty linkedlist conditon */
            if (prev == NULL)
              (*files)->f_head = new_file;
            else
              prev->next = new_file;
          }
          else
          {
            printf("fail to set the file name!\n");
          }
        }
        else
        {
          printf("fail to create the new file!\n");
        }
      }
    }
  }

  return result;
}

/*
 * mkdir function is used to simulate the mkdir command in UNIX. It would create
 * a new sub-directory in the current directory as defined in fs-sim-structure.h
 * with parameter arg as the name of the new directory.
 *
 * The function would return 1 if valid arguments passed in and new directory
 * created ,and 0 if invalid cases happened (most of invalid cases are the same
 * as in touch function).
 *
 * files: The pointer used to track the current directory in the filesystem.
 * arg: A characters pointer points to name that would be assigned for new
 *      directory.
 */
int mkdir(Fs_sim *files, const char arg[])
{
  int result = 0;
  Directory *curr, *parent, *prev = NULL, *new_directory = NULL;

  if (files != NULL && *files != NULL && arg != NULL)
  {
    if (!strcmp(arg, ""))
      result = 0;
    /* 
     *  Same as before, name contained forward-slash character (not sole of it)
     *  would be invalid.
     */
    else if (strcmp(arg, "/") && strchr(arg, '/') != NULL)
      result = 0;
    else if (!strcmp(arg, ".") || !strcmp(arg, "..") || !strcmp(arg, "/"))
      result = 0;
    else
    {
      /* 
       * arg cannot be a name of existing file or sub-directory located in the
       * current directory.
       */
      if (check_name(*files, arg))
        result = 0;
      else
      {
        /* 
	 * Using a directory pointer (or Fs_sim), parent, to explicity indicate
	 * that the files is pointing to the parent directory of the new sub-
	 * directory, which would be later assigned to its parent pointer.
	 */
        parent = *files;
        curr = (*files)->sub;

        /* Still inserting into the linkedlist in increasing order */
        while (curr != NULL && strcmp(arg, curr->name) > 0)
        {
          prev = curr;
          curr = curr->next;
        }

        new_directory = malloc(sizeof(*new_directory));

        if (new_directory != NULL)
        {
          new_directory->name = malloc(strlen(arg) + 1);

          if (new_directory->name != NULL)
          {
            strcpy(new_directory->name, arg);
            new_directory->parent = parent; /* Saving its parent directory*/
            new_directory->sub = NULL;
            new_directory->next = curr; /* Linking with same-level directory */
            new_directory->f_head = NULL;

            if (prev == NULL)
              (*files)->sub = new_directory;
            else
              prev->next = new_directory;

            result = 1;
          }
          else
          {
            printf("fail to set the directory name!\n");
          }
        }
        else
        {
          printf("fail to create the new directory!\n");
        }
      }
    }
  }

  return result;
}

/*
 * cd function is used to simulate the cd command in UNIX. The function would
 * change the current directory of its Fs_sim parameter and navigate to certain
 * directory as told by arg parameter.
 *
 * The function would return 1 if valid arguments passed in and navigation made
 * correctly, and 0 if invalid cases happened. (Explain more under)
 *
 * files: The pointer used to track the current directory in the filesystem.
 * arg: A characters pointer points to the name of target directory or certain
 *      patterns of characters which indicate certain types of navigation. 
 */
int cd(Fs_sim *files, const char arg[])
{
  int result = 0;

  if (files != NULL && *files != NULL && arg != NULL)
  {
    if (strcmp(arg, "/") && strchr(arg, '/') != NULL)
      result = 0;
    /* A single period represents staying in the current directory */
    else if (!strcmp(arg, "."))
      result = 1;
    /* 
     * Double periods represents changing to the parent directory. No effect if
     * the current directory is the root.
     */
    else if (!strcmp(arg, ".."))
    {
      if ((*files)->parent != NULL)
        *files = (*files)->parent;

      result = 1;
    }
    /*
     * A single forward-slash or empty string indicates moving to the root
     * directory. No effect if current directory is the root.
     */
    else if (!strcmp(arg, "/") || !strcmp(arg, ""))
    {
      Directory *root = *files;

      /* Keep jumping to the parent until reach the root which has no parent */
      while (root->parent != NULL)
        root = root->parent;

      *files = root;
      result = 1;
    }
    else
    {
      /* 
       * If arg is a normal valid string, searching for it in the linkedlist of 
       * sub-directories of the current directory and moving into it if found.
       */
      Directory *curr = (*files)->sub;
      result = 0;

      if (curr != NULL)
      {
        while (curr != NULL && strcmp(curr->name, arg) != 0)
          curr = curr->next;

        if (curr != NULL)
        {
          result = 1;
          *files = curr;
        }
      }
    }
  }

  return result;
}

/*
 * ls function is used to simulate the ls command in UNIX. The function would
 * list the files and subdirectories of the current directories, or of its
 * argument, or just the argument if that is a file.
 *
 * The function would return 1 if valid arguments passed in and list files
 * /directories correctly, and 0 if invalid cases happened. (Explain more under)
 *
 * files: The pointer used to track the current directory in the filesystem.
 * arg: A characters pointer points to the name of listing target or certain
 *      patterns of characters which indicate certain types of list. 
 */
int ls(Fs_sim *files, const char arg[])
{
  int result = 0;

  if (files != NULL && *files != NULL && arg != NULL)
  {
    if (strcmp(arg, "/") && strchr(arg, '/') != NULL)
      result = 0;
    /* 
     * A single period or empty string represents listing all files and
     * subdirectories in the current directory.
     */
    else if (!strcmp(arg, ".") || !strcmp(arg, ""))
    {
      /*
       * Passing two head pointers of the linkedlists, one is for files and 
       * another one is for subdirectories. The print_list function would 
       * assign the output in the format of increasing order. 
       */
      print_list((*files)->f_head, (*files)->sub);
      result = 1;
    }
    /* 
     * Double periods represents listing files and subdirectories in the parent
     * directory.
     */
    else if (!strcmp(arg, ".."))
    {
      if ((*files)->parent != NULL)
        /* Instead, passing heads of files and subdirectories in the parent. */
        print_list((*files)->parent->f_head, (*files)->parent->sub);
      else
        /* If current directory is the root, worked as same as calling "." */
        print_list((*files)->f_head, (*files)->sub);

      result = 1;
    }
    /* 
     * A single forward-slash represents listing files and subdirectories in 
     * the root.
     */
    else if (!strcmp(arg, "/"))
    {
      Directory *root = *files;

      /* Keep jumping up until reach the root */
      while (root->parent != NULL)
        root = root->parent;

      print_list(root->f_head, root->sub);
      result = 1;
    }
    else
    {
      /* 
       * Check if arg is a name existing in the current directory, otherwise, 
       * simply return 0.
       */
      if (check_name(*files, arg))
      {
        File *curr_file = (*files)->f_head;
        Directory *curr_directory = (*files)->sub;

        /* 
	 * Searching arg in the linkedlist of files, if found, simply print out
	 * the name of that file.
	 */
        while (curr_file != NULL && strcmp(arg, curr_file->name))
          curr_file = curr_file->next;

        if (curr_file != NULL)
          printf("%s\n", curr_file->name);
        /* 
	 * If arg is not found in the linkedlist of files, check if it is the
	 * name of a subdirectory. If found, listing all files and 
	 * subdirecotries in that subdirectory.
	 */
        else
        {
          while (curr_directory != NULL && strcmp(arg, curr_directory->name))
            curr_directory = curr_directory->next;

          print_list(curr_directory->f_head, curr_directory->sub);
        }
        result = 1;
      }
      else
        result = 0;
    }
  }

  return result;
}

/*
 * pwd function is used to simulate the pwd command in UNIX. The function would
 * print out the full path from the root to the current directory.
 *
 * files: The pointer used to track the current directory in the filesystem.
 */
void pwd(Fs_sim *files)
{
  if (files != NULL)
  {
    /* Simply print out a forward slash if the current directory is the root. */
    if ((*files)->parent == NULL)
      printf("/\n");
    else
    {
      /* An array of pointers of chracters is used to save the path */
      char **path;
      int levels = 0;
      Directory *curr = *files;
      int i;

      /* Counting the number of levels from the current directory to the root */
      while (curr->parent != NULL)
      {
        levels++;
        curr = curr->parent;
      }

      /* Using levels to allocate memory for the array to save the path */
      path = malloc(levels * sizeof(char *));

      if (path != NULL)
      {
        curr = *files;

        /* 
	 * Jumping through the path from the current directory to the root and 
	 * save every directory passing through in the array.
	 */
        i = levels - 1;
        while (curr->parent != NULL)
        {
          path[i--] = curr->name;
          curr = curr->parent;
        }

        /* Printing out all names of directories in the path */
        for (i = 0; i < levels; i++)
          printf("/%s", path[i]);
        printf("\n");

        free(path);
      }
      else
      {
        printf("fail to track the path!\n");
      }
    }
  }
}

/*
 * rmfs function is used to clean out the current filesystem. It would remove
 * all things (directories, files) in the filesystem. It deallocates any
 * dynamically-allocated memory, including directories, files and their names.
 * It calls a helper function, destroy_directories, which receives a directory
 * pointer and free all things under that directory and the directory itself.
 * 
 * files: The pointer used to track the current directory in the filesystem.
 */
void rmfs(Fs_sim *files)
{
  Fs_sim root;

  /* 
   * checking the parameter is not NULL and the filesystem has been correctly 
   * created.
   */
  if (files != NULL && *files != NULL)
  {
    root = *files;

    /* tracking back from the current directory until reach the root */
    while (root->parent != NULL)
      root = root->parent;

    /* 
     * calling the helper function to remove all things under the root and root
     * itself.
     */
    destroy_directories(root);

    /* 
     * the current directory pointer would be NULL until the next filesystem 
     * created (calling mkfs). 
     */
    *files = NULL;
  }
}

/*
 * rm function is used to remove a file or a directory from the current
 * directory. The function returns 1 if any file or directory was correctly
 * removed. Otherwise, it returns 0.
 * 
 * files: The pointer used to track the current directory in the filesystem.
 */
int rm(Fs_sim *files, const char arg[])
{
  int result = 0;

  if (files != NULL && *files != NULL && arg != NULL)
  {
    /*
     * the name of target could not be a single or double period, or an empty
     * string, or contains forward-slash character. 
     */
    if (!strcmp(arg, ".") || !strcmp(arg, "..") || !strcmp(arg, "") ||
        strchr(arg, '/') != NULL)
    {
      result = 0;
    }
    /* returns 0 if the target does not exist in the current directory */
    else if (!check_name(*files, arg))
    {
      result = 0;
    }
    else
    {
      File *curr_file = (*files)->f_head;
      File *prev_file = NULL;

      /* checking whether if the target is a existed file */
      while (curr_file != NULL && strcmp(arg, curr_file->name))
      {
        prev_file = curr_file;
        curr_file = curr_file->next;
      }

      /* remove and deallocate the file from the linkedlist if found */
      if (curr_file != NULL)
      {
        if (prev_file != NULL)
        {
          prev_file->next = curr_file->next;
          free(curr_file->name);
          free(curr_file);
        }
        else
        {
          /* handling the case if the file is the first one in the linkedlist */
          (*files)->f_head = curr_file->next;
          free(curr_file->name);
          free(curr_file);
        }
      }
      /* 
       * the case when the target was not found in the linkedlist of files.
       * Since the helper function, check_name, has been called above, it is for
       * sure that the target exists in the current directory. Therefore, if it
       * was not found in the linkedlist of files, it must be a subdirectory.
       */
      else
      {
        Directory *curr_directory = (*files)->sub;
        Directory *prev_directory = NULL;

        /* finding out the target in the linkedlist of subdirectories */
        while (curr_directory != NULL && strcmp(arg, curr_directory->name))
        {
          prev_directory = curr_directory;
          curr_directory = curr_directory->next;
        }

        /* remove it from the linkedlist */
        if (prev_directory != NULL)
          prev_directory->next = curr_directory->next;
        else
          /* the case when it is the first subdirectory in the list */
          (*files)->sub = curr_directory->next;

        /* 
	 * cutting its connections with any other directory in the same or upper 
	 * level to avoid any unintended deallocation. 
	 */
        curr_directory->next = NULL;
        curr_directory->parent = NULL;

        /*
	 * deallocate all things under the target directory and the directory
	 * itself by calling the helper function.
	 */
        destroy_directories(curr_directory);
      }

      result = 1;
    }
  }

  return result;
}

/*
 * print_list is used to print files and directories in the format of increasing
 * order.
 *
 * file_head: A file pointer points to the linked list of all files of the
 *            current directory.
 * sub_head:  A directory pointer points to the linked list of all sub 
 *            directories of the current directory. 
 */
static void print_list(File *file_head, Directory *sub_head)
{
  /*
   * Since linkedlists of files and subdirectories are already in the increasing
   * order, simply comparing one pair of them and printing the one coming first
   * until one linkedlist reaches the end.
   */
  while (file_head != NULL && sub_head != NULL)
  {
    if (strcmp(file_head->name, sub_head->name) < 0)
    {
      printf("%s\n", file_head->name);
      file_head = file_head->next;
    }
    else
    {
      printf("%s/\n", sub_head->name);
      sub_head = sub_head->next;
    }
  }

  /* Printing out the rest of the linkedlist which is still not empty */
  if (file_head != NULL && sub_head == NULL)
  {
    while (file_head != NULL)
    {
      printf("%s\n", file_head->name);
      file_head = file_head->next;
    }
  }
  else if (file_head == NULL && sub_head != NULL)
  {
    while (sub_head != NULL)
    {
      printf("%s/\n", sub_head->name);
      sub_head = sub_head->next;
    }
  }
}

/*
 * check_name is used to check whether if the current directory already
 * contained a same-name file or directory as the paramter arg. For touch and
 * mkdir commands, new file or directory is only created when none of the files
 * and sub directory in the current directory have the same name as the new one.
 * Also, cd and ls commands relies on check_name to check if there has the
 * target directory or file to navigate to or print out.
 * 
 * The function returns 1 if the file/directory name (arg) is found in the
 * current directory and 0 if not found.
 *
 * fs: a double directory pointer points to the current directory which would be
 *     checked for names.
 * arg: the target name.
 */
static int check_name(Fs_sim fs, const char arg[])
{
  int found = 0;
  File *curr_file = fs->f_head;
  Directory *curr_directory = fs->sub;

  /* Searching for arg in the linkedlist of files */
  if (curr_file != NULL)
  {
    while (curr_file != NULL && strcmp(curr_file->name, arg) != 0)
      curr_file = curr_file->next;

    if (curr_file != NULL)
      found = 1;
  }

  /* Searching for arg in the linkedlist of subdirectories */
  if (curr_directory != NULL)
  {
    while (curr_directory != NULL && strcmp(curr_directory->name, arg) != 0)
      curr_directory = curr_directory->next;

    if (curr_directory != NULL)
      found = 1;
  }

  return found;
}

/* 
 * destroy_files would destroy the entire linkedlist of files passing by the 
 * file pointer, file_head. It is only used when destroy all things under a
 * targeted directory.
 *
 * file_head: a file pointer points to the first file in the linkedlist which
 *            would be removed. 
 */
static void destroy_files(File *file_head)
{
  File *curr = file_head, *temp;

  while (curr != NULL)
  {
    temp = curr;
    curr = curr->next;
    free(temp->name);
    free(temp);
  }
}

/* 
 * destroy_directories would deallocate all dynamically allocated memory, 
 * including files, subdirectories and their names, under the directory pointed 
 * by top, and the directory itself from the filesystem.
 *
 * top: a directory pointer points to the top directory of everything which
 *      would be deallocated. 
 */
static void destroy_directories(Fs_sim top)
{
  /*
   * If the current directory had subdirectory, recursively calling the function 
   * to destroy the subdirectory itself and everything under it.
   */
  if (top->sub != NULL)
    destroy_directories(top->sub);

  /*
   * If the current directory is not the last subdirectory in the linkedlist of
   * subdirectories, recursively calling the function to destroy the next 
   * subdirectory and everything under it.
   */
  if (top->next != NULL)
    destroy_directories(top->next);

  /*
   * When there is no more subdirectory or reaching the end of linkedlist of 
   * subdirectories, deallocating the current directory and all files in it. 
   */
  destroy_files(top->f_head);
  free(top->name);
  free(top);
}
