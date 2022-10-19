# Chapter 4

## Exercise 4-1

Roughly test the program with and without the append option.

```sh
df -h | ./exercise_04_01 f1
cat f1
ls -a | ./exercise_04_01 f1 -a
```

## Exercise 4-2

To create a file with holes, we use the `fileio/seek_io.c` program from the book.

```sh
touch holeyfile
./seek_io holeyfile s100000 wabc
```

And verify size and contents of the copied file.

```sh
./exercise_04_02 holeyfile holeycopy
ls -l holeycopy
cat holeycopy
```
