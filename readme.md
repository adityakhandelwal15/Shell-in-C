Main code resides in shell.c.I have made a makefile so that directely "./terminal" will execute the code of shell.I have implemented bonus ques too.
1)Remindme-Not neccessary for ythe message to be in qoutes.
2)clock - this command has too flags one which was given and i have added another for how many times should it print the output.It is "-n" and then the number of times you wanna print it.

The code is modular.
1)ls is implemented in a separate file with all kinds of flags stated.
2)pinfo is also implemented in seperate files.

Multiple commands do not run seperated by semicolon.Single command input is only accepted.

All commands to work properly.Adding & at the of a command runs that in backgound.

All the required specifications are tested and working.
Special commands:
1)Overkill
2)kjob
3)fg
4)bg
All signals are handled properly."quit" is the only way to exit the terminal.
Piping and Redirection is also working.
