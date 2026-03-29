# DotSync

DotSync is a small tool to sync dotfiles in bulk through a configuration file.

I made this tool to automatise dotfiles' backup process whitout relying on system calls or external services.
The tool is anyway capable of handling any kind of text file, incuding .c or .html, but not complex formats like .docx or .odt files.

## How to use
Once launched the tool will scan the folder where it was placed for a configuration file (dotsync.cgf). If the file is not found the tool will create one empty with a header line with an example of configuration. Once the configuration file is set, the tool will parse line after line to copy files, skipping the ones which are newer than the source and recreating the folder structure specified to the target destination.

In order to set the configuration file you need to add paths to it with this format: `/path/to/source/file.txt|/path/to/destination/file.txt`

Each pair of paths needs its own line and a '|' must be used to separate the source and destionation path. A sample configuration file could be written like this:

```
/home/user/docs/sample.txt|/home/user/backup/sample.txt
/home/user/docs/facsimile.txt|/home/user/backup/facsimile.txt
/home/user/docs/project.txt|/home/gigi/user/backup/project.txt
```

To comment a line add a '#' at the beginning. Commented lines will be skipped.

The tool will also accepts a number of arguments such as:

| Option | Result |
|---|---|
| -h, --help | Show usage and option list. |
| -f, --forced | Ignore timestamp check and always copy. |
| -q, --quiet | Silence all output except for errors. |


## How to compile
To compile the code ensure you have a compiler first (i.e. gcc).

To compile write:

`gcc main.c -o dotsync`

You can then launch it with:

`./dotsync`

Dotsync was written specifically to compile with any standard from c99 to c23 in order to ensure maximum compatibility.

## Technicals
This tools works by reading and writing with strings, thus ensuring data interity through byte-per-byte copy, and uses recursion to recreate the folder structure specified to the destination.
Methods like `ftell()` and `fseek()` were used to manage the configuration file.
The tool employs no system calls and works solely through the `sys/stat.h` and `stdio.h` libraries.
The `errno.h` library allows for correct error logging and methods from the `strings.h` library like `strchr` allow string manipulation.
In order to avoid unnecessary operations the tool also eploys `stat` to check timestamps of files to ensure the source file doesn't override a newer file.
Finally, `clock()` from the `time.h` library allows the calculation of the total time taken by the process.

## License
[GPL3.0](https://choosealicense.com/licenses/gpl-3.0/)