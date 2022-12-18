# Chapter 28

## Exercise 28-1

Version `exercise_28_01a` tests `fork` while `exercise_28_01b` tests `vfork`. By default, each makes a million calls.

```sh
time ./exercise_28_01a [number-forks]
# 29.49s user 26.00s system 101% cpu 54.797 total
```

```sh
time ./exercise_28_01b [number-forks]
# 5.46s user 11.88s system 110% cpu 15.730 total
```
