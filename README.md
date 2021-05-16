# rh

## Requirements

* An ANSI/ISO C compiler is required

## Building

* If `rhlex.c` is deleted then you must have `flex` (version 2.4.6 or higher) to recreate it.

* If `getdate.c`, `rhgram.c`, or `rhgram.h` are deleted then you must have `perl-byacc` (version 1.8.3 or higher) to recreate the deleted file(s).

## Bugs

* The pointer to the error in error messages may be wrong. (Actually, it is more likely to be wrong than correct.)

* Function parameters can not have the same names as functions.

```c
 /* OK */
 b ( c )
 {
	 return ( c + 2 );
 }

 /* will not compile; */
 /* 'b' is a function */
 a ( b )
 {
	 return ( b + 2 );
 }
```

* The file system operator only works on SunOS 4.1.x

* No configure script

## Changes by Andy Valencia

* Support large files (on 32-bit)

* Handle embedded spaces in filenames

* Various portability issues

* Add block allocation size to file attrs accessible
