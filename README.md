# Project Title

C Minishell

## Description

This program, aka "mush" (The Minimally Useful SHell) is a simple shell which has nowhere near the functionality of a
full-blown shell like /bin/sh or /bin/csh, but it is fairly powerful for its size and has most features
that one would want in a shell, including:

* Both interactive and batch processing. If mush2 is run with no argument it reads commands from stdin until an end of file (^D) is typed. If mush2 is run with an argument, e.g. “mush2 foofile,” it will read its commands from foofile.
This must not be accomplished by duping the file to stdin. It is important to retain the
original standard input for executed pipelines, otherwise a script starting with the line cat
would proceed to cat the rest of the script.
* Support for redirection. mush2 supports redirection of standard in (<) and standard out (>) from or into files. mush2 also supports pipes (|) to connect the standard output of one command to the standard input of the following one.
* Support for quoted strings. mush2 supports double quotes to allow arguments with embedded spaces.
* Support for backslash escapes. mush2 supports the use of backslash (‘\‘) to cause the following character to be taken literally.
* A built-in cd command. Recall that cd cannot be run as a child process because it would change the child’s working directory, not the parent’s.
* Support for SIGINT. When the interrupt character (ˆC) is typed, mush2 catches the resulting SIGINT and responds appropriately. SIGINT does not need to be forwarded to children — the tty driver will take care of that — but after the signal, the shell should not die. Rather it should wait for any running children to terminate and reset itself to a sane state.

## Getting Started

### Dependencies

[@GCC Compiler](https://gcc.gnu.org/)

### Installing

* How/where to download your program
* Any modifications needed to be made to files/folders

### Executing program

* How to run the program
* Step-by-step bullets
```
code blocks for commands
```

## Help

Any advise for common problems or issues.
```
command to run if program contains helper info
```

## Authors
https://gcc.gnu.org/
[@maxchristy](https://github.com/maxchristy)

## Version History

* 0.2
    * Various bug fixes and optimizations
    * See [commit change]() or See [release history]()
* 0.1
    * Initial Release

## License

This project is licensed under the [NAME HERE] License - see the LICENSE.md file for details

## Acknowledgments

Course from which material was assigned

* Professor Phillip Nico
