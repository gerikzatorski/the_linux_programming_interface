# Chapter 39

## Exercise 39-1

```sh
# file needs to permit CAP_SYS_NICE capability
sudo setcap "cap_sys_nice=p" exercise_39_01

top &
./exercise_39_01 r 5 $(pidof top)
kill -9 $(pidof top)
```
