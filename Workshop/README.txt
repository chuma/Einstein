
  N E W T / 0   a n d   W O R K S H O P
  =====================================

  Contents
  --------

1. Git and Newt/0
2. Newt/0 inside Einstein
3. Workshop


  1. Git and Newt/0
  =================

Developer information: Newt/0 is a Git repostory inside a Git repostory. Newt/0
is maintained as a subtree inside the Workshop directory.

Newt/0 lives as a fork on GitHub. I used this command to link Einstein and Newt:
	$ git remote add -f newt0 https://github.com/MatthiasWM/NEWT0.git
$ git merge -s ours --no-commit Bproject/master <2>
$ git read-tree --prefix=dir-B/ -u Bproject/master <3>
$ git commit -m "Merge B project as our subdirectory" <4>

$ git pull -s subtree Bproject master <5>


  2. Newt/0 inside Einstein
  =========================

Newt/0 is a NewtonScript compiler and bytecode interpreter written by Makoto
Nukui ( http://gnue.github.com/NEWT0 http://so-kukan.com/gnue/ ). Einstein uses
Newt/0 to compile and package Newton Script applocations which can then be
installed in the Einstein emulator or distributed as .pkg files for Apple
Newton MessagePads.

TODO: more documentation on the integrattion of Newt/0


  3. Workshop
  ===========

Workshop will eventually be an Integrated Developer Environment for Newton
Script applications that uses the Einstein emulator for testing and debugging.

TODO: more documentation on the concept of Workshop
