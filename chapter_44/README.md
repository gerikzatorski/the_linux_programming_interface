# Chapter 44

## Exercise 44-1

Empty input will will terminate the program.

```sh
./exercise_44_01
```

## Exercise 44-2

Program runs demos for custom implementations of `popen()` and `pclose()`.

```sh
./exercise_44_02
```

## Exercise 44-3

This is run from the cmake build folder.

```sh
rm /tmp/seqnum_backup

./exercise_44_03 &
../tlpi-dist/pipes/fifo_seqnum_client 3
../tlpi-dist/pipes/fifo_seqnum_client 2
../tlpi-dist/pipes/fifo_seqnum_client

# kill server
kill -s SIGTERM $(pidof exercise_44_03)

# restart server to test backup
./exercise_44_03 &
../tlpi-dist/pipes/fifo_seqnum_client

# kill server
kill -s SIGTERM $(pidof exercise_44_03)
```

## Exercise 44-4

Ensure `SIGINT` and removes server FIFO prior to terminating.

```sh
./exercise_44_04 &
../tlpi-dist/pipes/fifo_seqnum_client 3

kill -s SIGINT $(pidof exercise_44_04)
ls /tmp/seqnum_sv
```

And do the same for `SIGTERM`.

```sh
./exercise_44_04 &
../tlpi-dist/pipes/fifo_seqnum_client 3

kill -s SIGTERM $(pidof exercise_44_04)
ls /tmp/seqnum_sv
```

## Exercise 44-5

The proposed solution would introduce a race condition. If any client attempts to write to the server FIFO, it would be sent a `SIGPIPE` signal (which kills the process by default).

## Exercise 44-6

Server uses nonblocking I/O to print an error message for malicious requests but continue running.

```sh
./exercise_44_06 &

# Send malicious requests
./exercise_44_06_malicious
./exercise_44_06_malicious

kill -s SIGTERM $(pidof exercise_44_06)
```
