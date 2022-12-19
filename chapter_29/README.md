# Chapter 29

## Exercise 29-1

`pthread_join` returns the error number `EDEADLK`.

```sh
./exercise_29_01
```

## Exercise 29-2

The problem with the program is that main thread creates a new thread using a stack allocated variable `buf`. The main thread then calls `pthread_exit()` which results in all contents of that stack becoming undefined. Any work done by the new thread will be using undefined data if the race-condition fails (ie. main thread exits first).
