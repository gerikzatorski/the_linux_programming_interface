# Chapter 27

## Exercise 27-1

The `execlp` call will execute the `xyz` program in `dir2` because the one in `dir1` is not executable despite being found first.

## Exercise 27-2

```sh
./exercise_27_02
```

## Exercise 27-3

The `execve` command detects the interpreter script and executes the following command:

```sh
/bin/cat -n script-path arg...
```

This concatenates the script with any other files listed and numbers the lines.

## Exercise 27-4

The code effectively creates two unrelated processes. The grandchild is orphaned and adopted by the init process and the parent process carries on. This is useful if you want to spin out a process without monitoring it within the parent.

## Exercise 27-5

The string within `printf` is buffered and there is no line break to flush it. When execlp loads the new program into a process's memory, this data is overwritten and ultimately forgotten.

## Exercise 27-6

The `SIGCHLD` handler is invoked as shown by this program.

```sh
./exercise_27_06
```

Because `system()` must block delivery of SIGCHLD while it is executing (Section 27.7), we can be assured that any process using `system` will be able to handle `SIGCHLD` signals as specified.
