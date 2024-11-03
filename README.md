# Emu6502
*Developed by @dabecart, 2024.*

A small simulator for my 65C02 based old-school computer.

# Build and run

From the console:
```
$ make && build/emu6502 a.out
```

If you are using VS Code:
1. Install C/C++ extension.
2. After installation:
    - To run without debugging: run the task`Debug: Start Without Debugging` (`Ctrl + F5`) 
    - To debug: run the task `Debug: Start Debugging` (`F5`)

# Simulating the peripherals:

## ACIA: Asynchronous Communications Interface Adapter

To simulate the serial port communications, you can specify the simulator using the `-s` argument the route of a serial port to use. If you don't have a serial port at hand, don't worry, you can create your own tunnel. The idea is that you'll have both ends of the serial port inside your computer: the emu6502 will connect to one end and you may connect yourself to the other. 

To create a tunnel in **Windows**, I recommend taking a look at [freevirtualports](https://freevirtualserialports.com/) (I know the name sounds sketchy but it's a pretty fine tool to debug serial ports). Create a COM-COM connection and pass one end as an argument to the emulator.

To create a tunnel in **Linux**, I recommend using `socat`. Install it and run:
```
$ socat -d -d pty,raw,echo=0 pty,raw,echo=0
```
This will return two newly created device routes. **Do not close this terminal.**

Use either of the two as argument for the emu6502 (on a new terminal):
```
$ ./build/emu6502 -s /dev/pts/7 a.out
``` 

On (yet) another terminal, you can `cat` the other route to listen to outgoing data from the simulator:
```
$ cat /dev/pts/6
```

# TODO list
- Add the +1 cycle if crossing page boundary.
- Implement the real BRK instruction.
- Implement 65C02 instructions.