# Chapter 49

## Exercise 49-1

```sh
echo "This is a text file" > tfile.txt
./exercise_49_01 tfile.txt copy.txt
cat copy.txt
```

## Exercise 49-2

TODO: go over chapter 48 first

## Exercise 49-3

First program verifies `SIGBUS`.

```sh
./exercise_49_03a tfile_49_03a.txt
```

Second program verifies `SIGSEGV`.

```sh
./exercise_49_03b tfile_49_03b.txt
```

## Exercise 49-4

Program writes "abc" to memory addresses. Output file reads "cba".

```sh
./exercise_49_04 tfile_49_04.txt

# verify nonlinear file mapping
cat tfile_49_04.txt
```
