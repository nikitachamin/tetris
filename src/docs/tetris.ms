.TL
BrickGame Tetris 1.0
.AU
School 21 project
.NH 1
Overview
.PP
BrickGame Tetris is a C11 implementation of Tetris.  The game logic is built
as a static library and is independent from the ncurses terminal interface.
The playing field is 10 cells wide and 20 cells high.
.NH 1
Build and run
.PP
Run "make" from the src directory.  The executable is created as
build/tetris.  Run "make install" to install it under PREFIX/bin.  The default
PREFIX is $HOME/.local.
.NH 1
Controls
.IP "Enter"
Start or restart the game.
.IP "Left and Right"
Move the current piece horizontally.
.IP "Down"
Drop the current piece to the lowest available position.
.IP "Space"
Rotate the current piece clockwise.
.IP "P"
Pause or resume the game.
.IP "Escape"
End the current game.
.IP "Q"
Exit the terminal application.
.NH 1
Scoring and levels
.PP
One, two, three, and four cleared rows award 100, 300, 700, and 1500 points.
The level increases after each 600 points and is capped at level 10.  Gravity
becomes faster at each level.  The high score is saved between program runs.
.NH 1
Finite-state machine
.PP
The implementation uses Start, Spawn, Moving, Shift, Attach, Pause, and Game
Over states.  User actions drive transitions from stable states, while timer
expiration causes the Shift transition.  Spawn, Shift, and Attach are explicit
transient states and are completed before control returns to the interface.
The complete graph is stored in docs/tetris_fsm.dot.
.NH 1
Testing
.PP
Run "make test" for the Check unit suite.  Run "make gcov_report" to create an
HTML coverage report under build/coverage/report.
