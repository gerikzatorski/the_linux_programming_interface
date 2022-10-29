# Chapter 10

## Exercise 10-1

Assuming that the clock_t value returned is an unsigned 32-bit integer...

The time() function will cycle around
4294967296 / 100 ~ 42949673 seconds
4294967296 / 100 / 60 / 60 / 24 ~ 497 days

The clock() function will cycle around
4294967296 / 1000000 ~ 4295 seconds
4294967296 / 1000000 / 60 ~ 72 minutes

