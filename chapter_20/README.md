# Chapter 20

## Exercise 20-1

Use example from 20.12 to test:

```sh
./exercise_20_01 15 &
./sig_sender <PID> 1000000 10 2
```

## Exercise 20-2

```sh
./exercise_20_02
```

## Exercise 20-3

First program verifies `SA_RESETHAND`. It should print messages indicating the handler was only visited once.

```sh
./exercise_20_03a
```

Second program verifies `SA_NODEFER`. The assertion at the end should not fail.

```sh
./exercise_20_03b
```

## Exercise 20-4

```sh
./exercise_20_04
```
