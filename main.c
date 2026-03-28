#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>

int main(){
    clock_t time;
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
            fprintf(stderr, "Configuration found! Starting process...\n");
        }

        //here starts reading the config and skips the commented lines
        time = clock();
        int count = 0;
        char pathToSrc[256], pathToDst[256];
        while(!feof(cfgptr)){
            char buff = fgetc(cfgptr);
            if(buff == '#'){
                ungetc(buff, cfgptr);

                while(fgetc(cfgptr) != '\n' && feof(cfgptr) == 0);

                count++;
            }
            else{
                ungetc(buff, cfgptr);
                fscanf(cfgptr, "%255[^|]|%255[^\n]\n", pathToSrc, pathToDst);
                fprintf(stderr, "Starting copy of [%s] to [%s]... ", pathToSrc, pathToDst);
                
                //here creates the folder if it doesn't exists yet
                char tempPath[256];
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
                        unsigned char buffer[256];
                        size_t input, output;

                        while(!feof(srcptr)){
                            input = fread(buffer, sizeof(unsigned char), 256, srcptr);

                            if(input > 0){
                                output = fwrite(buffer, sizeof(unsigned char), input, dstptr);

                                if(input != output){
                                    fprintf(stderr, "\nError: mismatch between reading and writing!\n");
                                }
                            }
                        }
                    }
                    else {
                        fprintf(stderr, "\nCan't open destination file at line %d! %s %s. Skipping to next line...", count, strerror(errno), pathToDst);
                    }
                    fclose(dstptr);
                }
                else {
                    fprintf(stderr, "\nCan't open source file at line %d! %s %s. Skipping to next line...", count, strerror(errno), pathToSrc);
                }
                fclose(srcptr);
                fprintf(stderr, "OK!\n");
            }
            count++;
        }
        fclose(cfgptr);
        time = clock() - time;
        fprintf(stderr, "Copy successful in %.3f ms!\n", ((float)time / CLOCKS_PER_SEC) * 1000);
    }

    return 0;
}