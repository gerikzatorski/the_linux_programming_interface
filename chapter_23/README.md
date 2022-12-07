# Chapter 23

## Exercise 23-1

```sh
./exercise_23_01
```

## Exercise 23-2

TLPI library provides programs for both `t_nanosleep.c` and `t_clock_nanosleep.c` that uses `TIMER_ABSTIME` with an additional flag. When bombarded with SIGINT signals, the former seems to take an infinite amount of time while the latter ends after 60 seconds.

## Exercise 23-3

Program schedules two timers (one with evp argument specified as NULL) to compare.

```sh
./exercise_23_03 
```

## Exercise 23-4

```sh
./exercise_23_04 secs[/nsecs][:int-secs[/int-nsecs]]...
```
