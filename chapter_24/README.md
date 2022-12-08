# Chapter 24

## Exercise 24-1

The total number of processes is 8. The number is doubled with each call to `fork` because parents and children are executing the same code (this is why you usually see `switch` used in conjuction with `fork`). 

## Exercise 24-2

```sh
./exercise_24_02
```

## Exercise 24-3

To get the core dump of a process while letting the process continue execution, we could fork the process and send a signal to the child that causes a core dump.

## Exercise 24-4

TODO: Need different UNIX implementations

## Exercise 24-5

The returned value from `fork` (ie. `childPid`) previously unused in Listing 24-6 is needed in new program.

```sh
./exercise_24_05
```
