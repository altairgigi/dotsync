#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

int main(){

    FILE *fptr = fopen("dotsync.cfg", "a+");
    if(fptr != NULL) {

        if(ftell(fptr) == 0) {
            fprintf(fptr, "# Write here the source and destination of the copy. es: path/to/source/|path/to/destination/");
            fclose(fptr); //closes the file after the write operation
            printf("Configuration not found!\n");
            return 1;
        }
        else{
            rewind(fptr);
        }

        char buff;
        fscanf(fptr, "%c", &buff);
        if(buff = '#'){
            //skip line
        }
        else{
            //start parsing
        }
    }

    //if target source exists parse content and save to buffer
    //otherwise skip it and signal missing file/folder

    //if source destination doesn't exist create folder
    //create or remake file copying content from buffer

    //procede to next line

    //once parsing complete return success

    return 0;
}