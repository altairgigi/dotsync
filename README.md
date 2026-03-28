# DotSync

DotSync is a small tool to sync files and folders through a configuration file.

I made this tool to automatise dotfiles' backup process whitout relying on system calls or external services.

## How to use
Once launched the tool will scan the folder where it was place for a configuration file (dotsync.cgf). If the file is not found the tool will create one empty with a header line with an example of configuration.

In order to set the configuration file you need to add paths to it with this format: `/path/to/source/file.txt|/path/to/destination/file.txt`

Each pair of paths needs its own line and a '|' must be used to separate the source and destionation path. A sample configuration file could be written like this:

```
/home/user/docs/sample.txt|/home/user/backup/sample.txt
/home/user/docs/facsimile.txt|/home/user/backup/facsimile.txt
/home/user/docs/project.txt|/home/gigi/user/backup/project.txt
```

To comment a line add a '#' at the beginning. Commented lines will be skipped.

NOTE: the tool will create the folders to the destination path if they don't exist already!

## How to compile
To compile the code ensure you have a compiler first (i.e. gcc).

To compile write:

`gcc main.c -o dotsync`

You can then launch it with:

`./dotsync`

## Technicals
This tools works by reading and writing with strings, thus ensuring data interity through byte-per-byte copy, and uses recursion to recreate the folder structure specified to the destination.
Methods like `ftell()` and `fseek()` were used to manage the configuration file.
The tool employs no system calls and works solely through the `sys/stat.h` and `stdio.h` libraries.
The `errno.h` library allows for correct error logging and methods from the `strings.h` libray like `strchr` allow string manipulation.
Finally, `clock()` from the `time.h` library allows the calculation of the total time taken by the process.

## License
[GPL3.0](https://choosealicense.com/licenses/gpl-3.0/)