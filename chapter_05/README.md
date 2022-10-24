# Chapter 5

# Exercise 5-1

Create and test large file size:

```sh
./exercise_05_01 ten_gigs 10111222333
ls -alh ten_gigs
```

# Exercise 5-2

Despite setting offset to beginning of the file, the data is written to the end of the file because O_APPEND ensures the offset is set prior to each write.

```sh
echo "zzzzzzz" > sample.txt
./exercise_05_02 sample.txt
cat sample.txt
```

# Exercise 5-3

```sh
touch f1 f2
./exercise_05_03 f1 1000000 & ./exercise_05_03 f1 1000000
./exercise_05_03 f2 1000000 x & ./exercise_05_03 f2 1000000 x
```

This leads to file sizes of 2000000 and 1000452 for f1 and f2. Using lseek to append the file leads to race conditions and the concurrent programs are writing to the same offset.

# Exercise 5-6

The output is `Giddayworld`. Initially, the file is written as `Hello,world` but quickly changed to `HELLO,world` when the shared file offset for fd1 and fd2 is set to the beginning of the file and overwritten. Again this is overwritten by simply using fd3 which maintained its offset since creation.
