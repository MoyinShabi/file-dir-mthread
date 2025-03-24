#include "header.h"
#include "pthread.h"

void readAndWriteToFile(const char *sourceFile, const char *destFile);
void *copyDirContentsToFile(void * /* const char *directoryPath, const char *destFile */);

const char *destFile = "destination.txt";

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Incorrect usage! Use as %s <existing_directory_absolute_path> <destination_file.txt>\n", argv[0]);
        return -1;
    }

    if (strcmp(argv[2], destFile) == 0)
    {
        fprintf(stderr, "Kindly try again using another file name!\n");
        return -1;
    }

    char sourceFile[FILE_LENGTH];
    // char directoryPath[DIR_LENGTH];
    // char filename[DIR_LENGTH];
    DirAndFile dirAndFile;

    char actualCurrentDir[DIR_LENGTH];
    char newCurrentDir[DIR_LENGTH];

    char *currentWorkingDir = getcwd(actualCurrentDir, DIR_LENGTH);
    if (currentWorkingDir == NULL)
    {
        perror("Error extracting current working directory! ");
        return -1;
    }
    int cdStatus = chdir("..");
    if (cdStatus < 0)
    {
        perror("Error changing current working directory, please try again! ");
        return -1;
    }

    char *newWorkingDir = getcwd(newCurrentDir, DIR_LENGTH);
    if (newWorkingDir == NULL)
    {
        perror("Error extracting new current working directory! ");
        return -1;
    }

    // strcpy(directoryPath, argv[1]);
    // strcpy(filename, argv[2]);
    // strcpy(dirAndFile.dir, argv[1]);
    strcpy(dirAndFile.dir, actualCurrentDir);
    // strcpy(dirAndFile.dir, newCurrentDir); // Try this instead of the above line to see the difference
    strcpy(dirAndFile.file, argv[2]);

    printf("Enter an existing file name in your current working directory as <file_name.txt> (Max 50 characters): ");
    fgets(sourceFile, FILE_LENGTH, stdin);
    sourceFile[strcspn(sourceFile, "\n")] = '\0';

    // Change back to the original working directory before proceeding
    cdStatus = chdir(actualCurrentDir);
    if (cdStatus < 0)
    {
        perror("Error changing current working directory, please try again! ");
        return -1;
    }

    readAndWriteToFile(sourceFile, destFile);
    // copyDirContentsToFile(directoryPath, filename);

    pthread_t threadDescriptor;
    int thread = pthread_create(&threadDescriptor, NULL, copyDirContentsToFile, &dirAndFile);
    if (thread < 0)
    {
        perror("Error creating thread! ");
        return -1;
    }
    pthread_join(threadDescriptor, NULL);

    return 0;
}
