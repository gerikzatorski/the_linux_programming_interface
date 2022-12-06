# Chapter 22

## Exercise 22-1

Run the program and quickly suspend and continue the process within 10 seconds. A message prints indicated whether the handler was visited prior to unblocking `SIGCONT`.

```sh
./exercise_22_01
```

## Exercise 22-2

Examine the program's output to see the order of delivered signals.

```sh
./exercise_22_02
```

## Exercise 22-3

```sh
time ./sig_speed_sigsuspend 1000000
# 0.31s user 3.19s system 61% cpu 5.683 total
```

```sh
time ./exercise_22_03 1000000
# 0.01s user 3.20s system 64% cpu 4.962 total
```

## Exercise 22-4

TODO
