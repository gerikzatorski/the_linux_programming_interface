# Chapter 33

## Exercise 33-1

```sh
./exercise_33_01
```

## Exercise 33-2

There is no guarantee that the `SIGCHLD` signal is sent to the thread that called `fork()`. Signal dispositions are process-wide and any signal delivered to a multithreaded process arbitrarily selects a thread to invoke the handler.
