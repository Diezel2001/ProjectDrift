from pyMsp import msp 

board = msp("dev/ttyACM0", 115200)

board.getIMU()