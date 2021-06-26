# robot_game_cpp

C++ console robot game. Very similar to the ship sinking game.

C++ console game. It is a ship sinking alike game called robot game.

Game consists of two players where on the game start players enter their names,
matrix dimension and number of robots. Firstly, the matrix dimensions must be NxN,
meaning the matrix must be a quadratic matrix. The max matrix dimension can be 15x15.

The max number of robots relies on the matrix dimension - the max robot number is always N-1 robots
for the given matrix dimensions and the players need to have at least one robot.

After the dimensions are correctly stated,
each player individually enters the position(s) of their robot(s) respectively:
firstly the first player enters his robots, then the second player enters his robots.

Each player also enters the robots respectively by size,
for instance lets take the matrix dimensions are 3x3 and the players chose to have 2 robots
(in this case there can be min 1 and max 2 robots like previously explained),
so the first player enters the first robot that takes only one coordinate,
then the second one who takes two coordinates and then the same goes for the second player.

A robot can only be positioned in one row or column, it cannot transpass diagonally, etc.
At any point the players can view their state by typing in -2 -2 -2 -2 as coordinates
when entering the coordinates of their robots or -2 -2 when they are trying do destroy a robot.

Their matrix gets outputed as: '-' as empty field, 'R' as a coordinate of a robot (or a part of robot),
'X' as a hit/destroy and 'O' as a try, but the other player missed your robot.

There is no limit for robot guessing, it only ends when one of the players destroys all the robots
from the oposite player or when they quit the game.

On the end, when the game finishes, the results are displayed.
Also, if a player just wants to quit, he can enter -1 -1 while targeting a robot and the game automatically ends.
