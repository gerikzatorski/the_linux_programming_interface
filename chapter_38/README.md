# Chapter 38

## Exercise 38-1

Modifying the set-user-ID file changes the permissions from `-rwsr-xr-x` back to `-rwxr-xr-x`. This is likely to prevent malicious changes to a set-user-ID program.

## Exercise 38-2

Might need to enable root password on some linux distributions (eg. Ubuntu) for this program to work with root user.

```sh
# make it a set-user-ID-root program:
sudo chown root ./exercise_38_02
sudo chmod u+s exercise_38_02

./exercise_38_02 [-u username]
```
