# `trim` -- simple whitespace trimmer in C
This program trims whitespace from the left and right side of an input
string from stdin.

Usage:

`echo "    some text   " | trim`

yields:

`some text`

There's also options for trimming just left- or right-side whitespace:

`echo "   more text    " | trim --right-only`

yields:

`   more text`
