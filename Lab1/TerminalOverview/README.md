# Terminal Overview

"Name 10 terminal commands"

That was a prompt I was given when interviewing for LinkedIn several years ago, and that was not the first time someone had asked me about the terminal at an interview! It's not that you should memorize 10 terminal commands, but rather you should understand that working on the terminal is an essential skill.

In this lab, you are going to perform a few terminal commands to get familiar with the Unix Environment.

## Part 0 - Obtain the mono repo

Make sure you have accepted the invitation to the monorepo from the main course page.

Run through the following github tutorial if you are not familiar with git.

https://try.github.io/levels/1/challenges/1

## Part 1 - SSH

The first thing you are going to need to do is learn how to SSH.

**I don't care what operating system you use as long as you have a terminal**

* For Windows Users installing Putty is sufficient. You might prefer to also install the bash shell from the Windows Store as well.
* For Linux/Mac users, you already have a terminal available! Just search for 'terminal'.

* Once your terminal is open, 'ssh' into ccis with: *ssh ccis_user_name_here@login.ccs.neu.edu*
  * If for some reason you do not have a ccis username, let me know. We'll have to e-mail operations.
* After you have successfully ssh'd, you are now running programs on the ccis servers (not locally on your machine).
* Now you may 'git clone' the mono repo to this location (Presumably you have done this if you are reading this text).

## Part 2 - The (at least) 10 Commands

Try running the followng commands.

1. ls
2. mkdir
3. cd
4. man
5. rmdir
6. sort
7. cat
8. nl
9. cut
10. grep

In fact, calling each of these 'commands' is not correct. Each of these is itself a program (typically implemented in C).

As an example, here is the source code for 'ls': http://git.savannah.gnu.org/cgit/coreutils.git/tree/src/ls.c
Each of these commands are part of the coreutils package in Unix. If you look through the source tree, you will additionally find many other programs (i.e. terminal commands you run in your shell) here: http://git.savannah.gnu.org/cgit/coreutils.git/tree/src/.

## Part 3 - Learn-C

The main programming language we will be using in this course is "C".

Complete the 'Learn the Basics' section of the C tutorial. If you are a C expert, then you can move onto the deliverables section.

http://www.learn-c.org/

## More resources to help

- If you are not comfortable with compiling, take a look at this resource: https://www.cs.fsu.edu/~myers/howto/g++compiling.txt
- Hackerrank has an excellent series of shell tutorials here: https://www.hackerrank.com/domains/shell/bash
  - I highly recommend going through them if you are not comfortable with the shell.

## Deliverable

* Copy and Paste the output of you running several terminal commands above into a file called "output" (no capitalization, no file extension, exactly as named.)
  * Make sure to 'commit' this file to the repo.
* Create a file called 'main.c' in your respository. This program should take a user specified input for a 'starting' integer and a 'ending' integer and count from the start to the stop and then exit.
  * Make sure to 'commit' this file to the repo.


