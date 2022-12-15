# Chapter 26

## Exercise 26-1

Some implementations no longer re-parent orphaned processes directly to `init`. There is a note about this in the [TLPI Errata](https://man7.org/tlpi/errata/index.html).

On my system, the process is re-parented to `/lib/systemd/systemd`.

```sh
./exercise_26_01
```

## Exercise 26-2

The child is adopted by systemd after the parent terminates. No `wait` call is needed from the grandparent in this program. Interrupt process to end (`SIGINT`).

```sh
./exercise_26_02
```

## Exercise 26-3

```sh
./exercise_26_03 [exit-status]
```

## Exercise 26-4

```sh
./exercise_26_04
```
