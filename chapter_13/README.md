# Chapter 13

## Exercise 13-1

a)

| File Size | BUF_SIZE | time  |
|-----------|----------|-------|
| 1         | 1        | 0.003 |
| 1         | 16       | 0.003 |
| 1         | 256      | 0.003 |
| 1         | 4096     | 0.003 |
| 100       | 1        | 0.003 |
| 100       | 16       | 0.003 |
| 100       | 256      | 0.003 |
| 100       | 4096     | 0.003 |
| 10000     | 1        | 0.016 |
| 10000     | 16       | 0.006 |
| 10000     | 256      | 0.003 |
| 10000     | 4096     | 0.003 |
| 1000000   | 1        | 1.043 |
| 1000000   | 16       | 0.084 |
| 1000000   | 256      | 0.010 |
| 1000000   | 4096     | 0.006 |

b)

The `O_SYNC` flag forces constant synchronized I/O file integrity (ie. data and metadata are flushed to the disk after every write()). `?????` if the program was testing my patience.

| File Size | BUF_SIZE | time  |
|-----------|----------|-------|
| 1         | 1        | 0.006 |
| 1         | 16       | 0.006 |
| 1         | 256      | 0.005 |
| 1         | 4096     | 0.005 |
| 100       | 1        | 0.192 |
| 100       | 16       | 0.024 |
| 100       | 256      | 0.013 |
| 100       | 4096     | 0.015 |
| 10000     | 1        | ????? |
| 10000     | 16       | 1.244 |
| 10000     | 256      | 0.071 |
| 10000     | 4096     | 0.010 |
| 1000000   | 1        | ????? |
| 1000000   | 16       | ????? |
| 1000000   | 256      | 8.144 |
| 1000000   | 4096     | 0.547 |

c)

TODO: test different types of filesystems

## Exercise 13-2

TODO: test different types of filesystems

## Exercise 13-3

`fflush(fp)` flushes data for the specified stream from the user-space buffer to the kernal-space buffer cache via write().

`fsync(fileno(fp))` retrieves the file descriptor for the fp stream and then flushes associated data AND metadata from the kernel-space to disk.

## Exercise 13-4

By default streams referring to disk files employ fully buffered I/O (\_IOFBF) while terminal devices employ line-buffered I/O (\_IOLBF). Therefore, if the code is redirected to a file, the write() call will transfer data to disk before before the printf() data is flushed to disk.

## Exercise 13-5

```sh
./exercise_13_05 [-n] file
```
