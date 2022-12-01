# Chapter 18

## Exercise 18-1

What I used for `longrunner.c`:

```c
#include <unistd.h>

int main(int argc, char *argv[])
{
    sleep(99999);
    return 0;
}
```

```sh
cc -o longrunner longrunner.c
ls -li longrunner
./longrunner &
emacs longrunner.c # make edits
cc -o longrunner longrunner.c
ls -li longrunner
```
The i-node number of the executable (`longrunner`) is different. It seems like the compiler using the same filename but writing to a new file.

## Exercise 18-2

The chmod call fails with error code `ENOENT` because `mylink` does not point to the `myfile` within the test directory. It is rather a dangling sybmolic link to a nonexistent `myfile` within the same directory. This is the result of using relative paths in the call `symlink("myfile", "../mylink")` which interprets relative paths based to the location of the link itself.

## Exercise 18-3

```sh
./exercise_18_03 <pathname>
```

## Exercise 18-4

```sh
./exercise_18_04 [pathname ...]
```

## Exercise 18-5

```sh
./exercise_18_05
```

## Exercise 18-6

No need to modify the listing. Simply use the `-d` flag for `nftw_dir_tree.c` to see the effect of `FTW_DEPTH`. When using the flag, children are processed first. This is an indication of postorder traversal.

## Exercise 18-7

```sh
./exercise_18_07 [pathname]
```

## Exercise 18-8

TODO

## Exercise 18-9

```sh
./exercise_18_09
```

The calls to `fchdir()` take less time than `chdir()`. I suspect one reason for this is that `fchdir()` uses file descriptors which can readily access useful information like the i-node object for a file (see Chapter 14). On the other hand `chdir()` merely accepts a string argument which must require some overhead to access the same information. 
