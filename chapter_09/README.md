# Chapter 9

## Exercise 9-1

Start with `real=1000 effective=0 saved=0 file-systems=0` (i.e. process is privileged).

a) `setuid(2000)` --> `real=2000 effective=2000 saved=2000 file-systems=2000`
b) `setreuid(-1, 2000)` --> `real=1000 effective=2000 saved=2000 file-systems=2000`
c) `seteuid(2000)` --> `real=1000 effective=2000 saved=0 file-systems=2000`
d) `setfsuid(2000)` --> `real=1000 effective=0 saved=0 file-systems=2000`
e) `setresuid(-1, 2000, 3000)` --> `real=1000 effective=2000 saved=3000 file-systems=2000`

## Exercise 9-2

The process is not privileged because the effective id does not equal zero.

## Exercise 9-3

We need to make the exercise a set-user-id program because setgroups() requires privilege.

```sh
sudo chown root exercise_09_03
sudo chmod u+s exercise_09_03
./exercise_09_03
```

## Exercise 9-4

Given a non-privileged set-user-ID program...

a) To suspend and resume the set-user-ID.

```c
euid = geteuid(); /* this will equal Y */

setuid(getuid());
setuid(euid);

seteuid(getuid());
seteuid(euid);

setreuid(-1, getuid());
setreuid(-1, euid);

setresuid(-1, getuid(), -1);
setresuid(-1, euid, -1);
```

b) To permanently drop the set-user-ID.

```c
/* setuid() cannot permanently drop saved ID */

/* seteuid() cannot permanently drop saved ID */

setreuid(getuid(), getuid());

setresuid(getuid(), getuid(), getuid());
```

## Exercise 9-5

Given a privileged set-user-ID program...

a) To suspend and resume the set-user-ID.

```c
euid = 0;

/* setuid() cannot suspend and resume privilege */

seteuid(getuid());
seteuid(0);

setreuid(-1, getuid());
setreuid(-1, 0);

setresuid(-1, getuid(), -1);
setresuid(-1, 0, -1);
```

b) To permanently drop the set-user-ID.

```c
setuid(getuid());

/* seteuid() cannot permanently drop privilege */

setreuid(getuid(), getuid());

setresuid(getuid(), getuid(), getuid());
```
