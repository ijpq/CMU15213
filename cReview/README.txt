Hello, recitation attendee!

This recitation activity is designed to help you understand getopt.
Understanding getopt will be important when you do cachelab! Other things
we hope you get from this recitation is a review of C and an introduction to
reading technical documentation.

If you don't finish the activiy in class, feel free to come to OH, ask your friends,
and work on the activity in your free time. 

There are two C programs in this directory:
- getopt_example.c This program is an example of how getopt can be used. More
information can be found on the slides.
- pyth_solver.c This program (which is incomplete) is supposed to be a program where
you input "sides" a, b, and c and the program outputs whether they are a Pythagorean triple
or not.

There is also a run_tests script in the directory. The program will run your solver
on inputs and then output the result! This allows you to see if your program is running
as you intended.

To compile all files, type:
$ make

IMPORTANT: every time you edit your solver, you need to type:
$ make clean
After this, the executables are erased and then you can recompile with make.

To compile just the example, type:
$ make getopt_example

To make just your Pythagorean triple solver, type:
$ make pyth_solver

To run the given tests, you need to compile your solver, then type:
./run_tests