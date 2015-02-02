# aos-tests
Test cases for the AOS projects.
If you use this testcases I expect you to contribute by writing you own testscases and add them.
The testcase should just be a single c file (name it testX.c) and should terminate if successfull.
Also print a description of the expected behaviour of your testcase.
Prefered contribution methods are branching and pull request...
Hope this helps everybody.

# Usage
To run just copy the test folder into your folder with the gtthread.h and
gtthread.a (or adjust the paths in the Makefile).
Then run
``` make ```
in the tests folder to build all the testcases and if successfull this will run them
The script will prompt you through what to do and what the expected output
should be. If your version does not output the desired thing go fix it (or fix
the testcases  if you find errors there).
# Debugging
To create executables that can be debuged (e.g with gdb + ddd) you have to adapt
the makefile (explaination in the comments).
Then run:
``` make debug ```
This will generate executables with debug symbols included (you also have to
generate the object files of your library with debug flags enables).

# Further test cases to write
More tests for mutex locks, tests for round robin working correctly...
