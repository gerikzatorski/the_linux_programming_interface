# Chapter 34

## Exercise 34-1

The problem with the code occurs if used in a shell pipeline. The shell places piped processes in a new process group, so this code could send `SIGUSR1` too other piped processes which may be unintended.

This could be avoided by collecting child pids returned from `fork()` calls and sending the `SIGUSR1` signal to each process.

## Exercise 34-2

Program (`EACCES` error) on second attempt to change child PGID.

```sh
./exercise_34_02
```

## Exercise 34-3

Program fails because it is the process group leader.

```sh
./exercise_34_03
```

But what if it isn't the process group leader.

```sh
ls -l | ./exercise_34_03
```

## Exercise 34-4

Modified the program to fork twice and change the pgid of one child. The parent process of the program catches `SIGHUP` given any commandline argument.

Run each command in a seprate terminal then exit the terminal.

```sh
exec /.exercise_34_04 > sig.log 2>&1
```

```sh
exec /.exercise_34_04 x > sighandled.log 2>&1
```

The unhandled version catches two signals in the unchanged child. The handled version catches only one signal in the unchanged child (and two in the parent).

## Exercise 34-5

If `SIGTSTP`was unblocked at the beginning of the handler in Listing 34-6, the process would be vulnerable to `SIGTSTP` signals from outside the handler.

## Exercise 34-6

```sh
./exercise_34_06
```

## Exercise 34-7

```sh
./exercise_34_07
```
