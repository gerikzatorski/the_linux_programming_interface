# the_linux_programming_interface

## Setup Dependencies

A summary of steps by the [tlpi online reference](https://man7.org/tlpi/code/index.html) to setup on Ubuntu (20.04).

```sh
# download and extract files
# more specifically, write files to stdout and pipe to tar command

wget -c https://man7.org/tlpi/code/download/tlpi-220912-dist.tar.gz -O - | tar -xz

# Debian/Ubuntu dependencies listed in the BUILDING file

sudo apt install libcap-dev \
                 libacl1-dev \
                 libselinux1-dev \
                 libseccomp-dev \
                 gcc-multilib

# build all programs in all directories

cd tlpi-dist/
make
```

## Build Exercises

```sh
mkdir build
cd build
cmake ..
cmake --build .
```
