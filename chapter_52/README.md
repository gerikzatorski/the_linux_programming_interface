# Chapter 52

## Exercise 52-1

Create message queue first. Then wait to receive message with 2 second timeout.

```sh
../tlpi-dist/pmsg/pmsg_create -cx /mq
./exercise_52_01 -t 2 /mq
```

Make sure to cleanup message queue.

```sh
../tlpi-dist/pmsg/pmsg_unlink /mq
```

## Exercise 52-2

Run example similar to one given in chapter 44.

```sh
./exercise_52_02_server &
./exercise_52_02_client 3
./exercise_52_02_client 2
./exercise_52_02_client
```

Cleanup server process.

```sh
kill -s SIGTERM $(pidof exercise_52_02_server)
```

## Exercise 52-3

TODO: after completing chapter 46

## Exercise 52-4

TODO

## Exercise 52-5

Create message queue and call program.

```sh
../tlpi-dist/pmsg/pmsg_create -cx /mq
./exercise_52_05 /mq
```

Multiple messages should only yield one response from server.

```sh
../tlpi-dist/pmsg/pmsg_send /mq abc
../tlpi-dist/pmsg/pmsg_send /mq abcdefg
```

Make sure to cleanup message queue.

```sh
../tlpi-dist/pmsg/pmsg_unlink /mq
```

## Exercise 52-6

Create message queue and call program.

```sh
../tlpi-dist/pmsg/pmsg_create -cx /mq
./exercise_52_06 /mq 4
```

Send message and review output. The `si_value` should match second argument of exercise program.

```sh
../tlpi-dist/pmsg/pmsg_send /mq abcdefg
```

Make sure to cleanup message queue.

```sh
../tlpi-dist/pmsg/pmsg_unlink /mq
```

## Exercise 52-7

Because Listing 52-7 only uses buffer to receive the messages from the message queue, *buffer* could be made a global variable without affecting the operation (ie. displaying the number of bytes read). However, any further use of the buffer (ex. reading/displying the buffer contents) would introduce possible race conditions as threads share the same gloval and heap variables.
