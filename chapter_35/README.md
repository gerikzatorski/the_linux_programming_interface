# Chapter 35

## Exercise 35-1

Can decrease priority:

```sh
./exercise_35_01 -5 echo -n "Hello"
```

Or increase priority with privilege:

```sh
sudo ./exercise_35_01 --5 echo -n "Hello"
```

## Exercise 35-2

```sh
# Make it a set-user-ID-root program
sudo chown root ./exercise_35_02
sudo chmod u+s ./exercise_35_02

./exercise_35_02 f 10 echo "Hello"
```

## Exercise 35-3

```sh
sudo ./exercise_35_03
```

## Exercise 35-4

TODO: First review pipes in chapter 44
