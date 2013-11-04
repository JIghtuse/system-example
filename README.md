system-example
==============

Linux system programming examples

###Index
* [tmpfile](#tmpfile)


###tmpfile

Secure temporary file creation (uses O\_TMPFILE). Requres Linux 3.11 or higher.  
Partially based on [WillDignazio gist](https://gist.github.com/WillDignazio/6077679).

Program require root privileges to run (I could be wrong).

To see 'hidden' file, one can:

* Place sleep after open() call and before close() call:

        printf("Temporary file fd: %d\n", fd);
        sleep(100);

* Compile, run program and remember fd number

        # ./tmpfile
        Temporary file fd: 3

* In another terminal run lsof to see file:

        # lsof -p $(pidof tmpfile)
        COMMAND   PID USER   FD   TYPE DEVICE SIZE/OFF    NODE NAME
        ...
        tmpfile 18849 root    3u   REG    8,7        0 1602782 /home/jightuse/code/projects/system-example/#1602782 (deleted)
