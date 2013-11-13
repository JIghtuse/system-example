system-example
==============

Linux system programming examples

###Index
* [largefile](#largefile)
* [tmpfile](#tmpfile)
* [syncint](#syncint)


###largefile

Utility creates empty file and enlarge it (up to ~16Tb) by seeking past its end
at large offset and writing one byte on this position. This creates new space
between 0 and new length padded with zeros. Hole (zero padding) in Unix
filesystems usually do not occupy any physical disk space.

Program based on Robert Love's
[Linux System Programming](http://shop.oreilly.com/product/0636920026891.do)
(File I/O - Seeking with lseek() - Seeking Past the End of a File), but creates
larger files.

Usage:

    ./bin/largefile <new large file filename>

###tmpfile

Secure temporary file creation (uses O\_TMPFILE). Requres Linux 3.11 or higher.  
Partially based on
[WillDignazio gist](https://gist.github.com/WillDignazio/6077679).

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


###syncint

Different synchronization approaches to protect shared resource (int variable).
Based on AlexOnLinux [series on synchronization](http://www.alexonlinux.com/do-you-need-mutex-to-protect-int).

Do you need a mutex to protect an int?

Code has one shared resource, global integer variable. Process create threads by
number of available processors. Each thread affined to certain processor core.
Then each thread increments global variable INC\_TO times.

Program can be compiled to use 5 different approaches to synchronization:

* no synchronization - guess what happened to global int?

        make bin/syncint

* mutex - inefficient way to synchronization which uses locks. One thread
takes mutex and make its work. In the same time another threads sleep waiting
when working thread unlock mutex.

        CFLAGS=-D_USE_MUTEX make bin/syncint

* spinlock - more efficient way for such small critical region as one
increment. Each thread spin tried to get access to shared resource. So it eats
a lot of processor time but works sufficiently faster than code with mutex.

        CFLAGS=-D_USE_SPIN make bin/syncint

* atomic increment - really fast and atomic increment (just one instruction)

        CFLAGS=-D_USE_ATOMIC make bin/syncint

* transaction - brand-new technology which allow threads work concurrently until
they not interact with the same memory region. Slow in this example (probably
due to transaction cancels - threads addresses one memory address all the time).

        CFLAGS=-D_USE_GTM make bin/syncint
