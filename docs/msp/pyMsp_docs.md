## how to import
> import pyMsp

## how to import main msp class
> from pyMsp import msp

## Sample Code
> from pyMsp import msp
>
> board = msp("/dev/ttyACM0", 11500)
> boardname = board.getName()
> print(boardname.name) or boardname.print()
>
> imudata = board.getimuRawData()
> imudata.print()
> accel_x = imudata.accel_x


## List of Methods

