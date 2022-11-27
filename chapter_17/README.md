# Chapter 17

## Exercise 17-1

Usage:

```sh
./exercise_17_01 <u|g> <user|group> file
```

Example:

```sh
# Add user to test
sudo useradd john
sudo passwd john

# Create file and modify acl
echo "" > tfile
setfacl -m u:john:rx tfile

# Test
./exercise_17_01 u john tfile
```
