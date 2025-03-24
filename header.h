#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

#ifndef HEADER_H
#define HEADER_H

#define FILE_LENGTH 50
#define DIR_LENGTH 1024
#define BUFFER_SIZE 100

typedef struct _DirAndFile
{
    char dir[DIR_LENGTH];
    char file[FILE_LENGTH];
} DirAndFile;

void readAndWriteToFile(const char *sourceFile, const char *destFile)
{
    // Open source file
    int fd_s = open(sourceFile, O_RDONLY);
    if (fd_s < 0)
    {
        perror("Error opening source file! ");
        return;
    }

    // Open dest file
    int fd_d = open(destFile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (fd_d < 0)
    {
        perror("Error opening the destination file! ");
        return;
    }

    char buffer[BUFFER_SIZE];
    int bytesRead = read(fd_s, buffer, BUFFER_SIZE);
    if (bytesRead < 0)
    {
        perror("Error reading from the source file! ");
        return;
    }
    else if (bytesRead == 0)
    {
        fprintf(stderr, "Nothing to read from the %s file! Please add some text and try agin.\n", sourceFile);
        return;
    }

    while (bytesRead > 0)
    {
        int bytesWritten = write(fd_d, buffer, bytesRead);
        if (bytesWritten < 0)
        {
            perror("Error writing to the destination file! ");
            return;
        }

        bytesRead = read(fd_s, buffer, BUFFER_SIZE);
        if (bytesRead < 0)
        {
            perror("Error reading from the source file! ");
            return;
        }
    }

    printf("Succeesfully copied the contents of %s into %s! Check it out.\n", sourceFile, destFile);

    close(fd_s);
    close(fd_d);
}

void *copyDirContentsToFile(void *data /* const char *directoryPath, const char *destFile */)
{
    DirAndFile *dirAndFile = (DirAndFile *)data;
    // Open specified directory
    // DIR *directory = opendir(directoryPath);
    DIR *directory = opendir(dirAndFile->dir);
    if (directory == NULL)
    {
        perror("Error opening specified directory! ");
        return (void *)-1;
    }

    // Open destination file
    int fd = open(dirAndFile->file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (fd < 0)
    {
        perror("Error opening destination file! ");
        return (void *)-1;
    }

    struct dirent *dirEntryRead = readdir(directory);

    while (dirEntryRead != NULL)
    {

        if (strcmp(dirEntryRead->d_name, ".") == 0 || strcmp(dirEntryRead->d_name, "..") == 0 || dirEntryRead->d_name[0] == '.')
        {
            dirEntryRead = readdir(directory);
            continue;
        }
        int bytesWritten = write(fd, dirEntryRead->d_name, strlen(dirEntryRead->d_name));
        if (bytesWritten < 0)
        {
            perror("Error writing to the destination file! ");
            return (void *)-1;
        }

        write(fd, "\n", 1);
        dirEntryRead = readdir(directory);
    }

    // printf("Successfully copied the contents of %s into %s! Check it out.\n", directoryPath, destFile);
    printf("Successfully copied the contents of %s into %s uing another thread! Check it out.\n", dirAndFile->dir, dirAndFile->file);

    closedir(directory);
    close(fd);

    return NULL;
}

#endif