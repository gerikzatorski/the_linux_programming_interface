# Chapter 57

## Exercise 57-1

Open a datagram socket in the unix domain that sleeps for 1/10th second between messages.

```sh
./exercise_57_01_server 100000000
```

In another terminal, create a client that sends 500 messages. If the client is never blocked, try sending more messages.

```sh
./exercise_57_01_client 500
```

## Exercise 57-2

```sh
./exercise_57_02_server > b &
```

```sh
echo "Lorem ipsum dolor sit amet" > a
./exercise_57_02_client < a
```

Cleanup server process and examine file differences (should be none).

```sh
kill -s SIGTERM $(pidof exercise_57_02_server)
diff a b
```

## Exercise 57-3

Run example similar to one given in chapter 44.

```sh
./exercise_57_03_server &
./exercise_57_03_client 3
./exercise_57_03_client 2
./exercise_57_03_client
```

Cleanup server process.

```sh
kill -s SIGTERM $(pidof exercise_57_03_server)
```

## Exercise 57-4

```sh
./exercise_57_04
```
