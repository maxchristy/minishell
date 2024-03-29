# Project Title

C Minishell

## Description

This program, aka "mush" (The Minimally Useful SHell) is a simple shell which has nowhere near the functionality of a
full-blown shell like /bin/sh or /bin/csh, but it is fairly powerful for its size and has most features
that one would want in a shell, including:

* Both interactive and batch processing. If mush is run with no argument it reads commands from stdin until an end of file (^D) is typed. If mush is run with an argument, e.g. “mush foofile,” it will read its commands from foofile.
This must not be accomplished by duping the file to stdin. It is important to retain the
original standard input for executed pipelines, otherwise a script starting with the line cat
would proceed to cat the rest of the script.
* Support for redirection. mush supports redirection of standard in (<) and standard out (>) from or into files. mush also supports pipes (|) to connect the standard output of one command to the standard input of the following one.
* Support for quoted strings. mush supports double quotes to allow arguments with embedded spaces.
* Support for backslash escapes. mush supports the use of backslash (‘\‘) to cause the following character to be taken literally.
* A built-in cd command. Recall that cd cannot be run as a child process because it would change the child’s working directory, not the parent’s.
* Support for SIGINT. When the interrupt character (ˆC) is typed, mush catches the resulting SIGINT and responds appropriately. SIGINT does not need to be forwarded to children — the tty driver will take care of that — but after the signal, the shell should not die. Rather it should wait for any running children to terminate and reset itself to a sane state.

## Getting Started

### Dependencies

[GCC Compiler](https://gcc.gnu.org/)

### Installing

* This program can be downloaded in its entirety from this repository.

### Executing program

* Execute the included makefile
```
make
```
* Run the output of said makefile
```
./mush.out
```

## Help

For help or feedback email mjchristy@gmail.com.

## Authors
[@maxchristy](https://github.com/maxchristy)

## Version History

* 0.1
    * Initial Release

## Acknowledgments

Course from which material was assigned

* Professor Phillip Nico
