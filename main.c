#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

#define PATH_MAX 4096

int main(int argc, char *argv[]){
    bool isForced = false, isQuiet = false;

    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
            printf("Usage: dotsync [OPTIONS]\n"
                   "Options:\n"
                   "  -f, --force      Ignore timestamp check and always copy\n"
                   "  -q, --quiet      Silence all output except for errors\n"
                   "  -h, --help       Show this help\n");
            return 2;
        }
        else if(strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--forced") == 0){
            isForced = true;
        }
        else if(strcmp(argv[i], "-q") == 0 || strcmp(argv[i], "--quiet") == 0){
            isQuiet = true;
        }
    }

    FILE *cfgptr = fopen("dotsync.cfg", "a+");

    if(cfgptr != NULL) {
        //this checks if the file is empty, in it is case it writes the instructions in the first line
        fseek(cfgptr, 0, SEEK_END);
        if(ftell(cfgptr) == 0) {
            fprintf(cfgptr, "# Write here the source and destination of the copy. es: path/to/source/|path/to/destination/");
            fclose(cfgptr);
            fprintf(stderr, "Empty configuration file created! Exiting...\n");
            return 1;
        }
        else{
            rewind(cfgptr);
            printf("Configuration found! Starting syncing...\n");
        }

        //here starts reading the config and skips the commented lines
        clock_t time = clock();
        int countLine = 1, countFile = 0;
        char pathToSrc[PATH_MAX], pathToDst[PATH_MAX], line[PATH_MAX * 2];
        while(fgets(line, sizeof(line), cfgptr)){
            if(line[0] == '#' || line[0] == '\n'){
                countLine++;
                continue;
            }
            if(sscanf(line, "%4095[^|]|%4095[^\n]\n", pathToSrc, pathToDst) == 2) {
                if(!isQuiet){
                    printf("Starting copy of [%s] to [%s]... ", pathToSrc, pathToDst);
                }

                //here checks the timestamps in order to skip the copy if the destination file is newer
                struct stat infoSrc, infoDst;

                if(stat(pathToDst, &infoDst) == 0) {
                    if(stat(pathToSrc, &infoSrc) == 0) {
                        if (isForced == 0 && infoDst.st_mtime > infoSrc.st_mtime)
                        {   
                            if(!isQuiet){
                                printf("\nDestination file is newer than source! Skipping...\n");
                            }
                            countLine++;
                            continue;
                        }
                    }
                    else{
                        fprintf(stderr, "\nFailed to open source file metadata at line %d: %s!\n", countLine, strerror(errno));
                    }
                }
                
                //here creates the folder if it doesn't exists yet
                char tempPath[PATH_MAX];
                strcpy(tempPath, pathToDst);
                char *ptr = tempPath;

                while((ptr = strchr(ptr, '/')) != NULL){
                    int ptrIndex = 0;
                    if((ptrIndex = ptr - tempPath) != 0){
                        tempPath[ptrIndex] = '\0';
                        mkdir(tempPath, 0777);
                        tempPath[ptrIndex] = '/';
                    }
                    ptr++;
                }

                //here it opens the files and starts the copy process
                FILE *srcptr = fopen(pathToSrc, "r");

                if(srcptr != NULL){
                    FILE *dstptr = fopen(pathToDst, "w");

                    if(dstptr != NULL){
                        unsigned char buffer[PATH_MAX];
                        size_t input, output;

                        while(!feof(srcptr)){
                            input = fread(buffer, sizeof(unsigned char), PATH_MAX, srcptr);

                            if(input > 0){
                                output = fwrite(buffer, sizeof(unsigned char), input, dstptr);

                                if(input != output){
                                    fprintf(stderr, "\nError: mismatch between reading and writing!\n");
                                }
                            }
                        }
                    }
                    else {
                        fprintf(stderr, "\nCan't open destination file at line %d! %s %s. Skipping to next line...", countLine, strerror(errno), pathToDst);
                    }
                    fclose(dstptr);
                }
                else {
                    fprintf(stderr, "\nCan't open source file at line %d! %s %s. Skipping to next line...", countLine, strerror(errno), pathToSrc);
                }
                fclose(srcptr);
                if(!isQuiet){
                    printf("OK!\n");
                }
                countFile++;
            }
            countLine++;
        }
        fclose(cfgptr);
        time = clock() - time;
        printf("Sync successful: copied %d files in %.3f ms!\n", countFile, ((float)time / CLOCKS_PER_SEC) * 1000);
    }

    return 0;
}