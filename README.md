# Emu6502
*Developed by @dabecart, 2024.*

A small simulator for my 65C02 based old-school computer.

# Build and run

From the console:
```
$ make && build/emu6502 a.out
```

# Simulating the peripherals

## ACIA: Asynchronous Communications Interface Adapter

The ACIA 6551 was intended to provide the 6502 microprocessor the interface to connect with serial communication data sets and modems. It comes with a lot of utilities to define the serial characteristics (baudrate, stopbits, parity) and their own hardware control signals. Mostly all of the capabilities of the ACIA have been implemented on this simulator (you may find on the [acia.h](src/periph/acia.h) file the comments listing which options are not supported).

To simulate the serial port communications, you can specify the simulator using the `-s` argument the route of a serial port to use. If you don't have a serial port at hand, don't worry, you can create your own tunnel. The idea is that you'll have both ends of the serial port inside your computer: the emu6502 will connect to one end and you may connect yourself to the other. 

To create a tunnel in **Windows**, I recommend taking a look at [freevirtualports](https://freevirtualserialports.com/) (I know the name sounds sketchy but it's a pretty fine tool to debug serial ports). Create a COM-COM connection and pass one end as an argument to the emulator.

To create a tunnel in **Linux**, I recommend using `socat`. Install it and run:
```
$ socat -d -d pty,link=$HOME/ttyEmu0,raw,echo=0,crtscts=1 pty,link=$HOME/ttyEmu1,raw,echo=0,crtscts=1
```

**Do not close this terminal.** This will return two newly created device routes, but you may use their symbolic links `$HOME/ttyEmu0` and `$HOME/ttyEmu1`, which will be located in your `$HOME` folder (temporarily). This is so you **don't** need `sudo` to run this. 

Use either of the two links as argument for the emu6502 (on a new terminal):
```
$ ./build/emu6502 -s $HOME/ttyEmu0 -o run.out a.out
``` 

On (yet) another terminal, you can `cat` the other route to listen to outgoing data from the simulator:
```
$ cat $HOME/ttyEmu1
```

This won't let you write to the serial port though. For that, I recommend you installing a serial terminal application, like `minicom`. Install it and call it:
```
$ minicom -D $HOME/ttyEmu1 -w
```

**Note:** I've noticed that the baudrate does not affect the `minicom` and it 'always' work, whichever configuration you use. I suppose that if you hooked a real serial terminal it would matter the serial configuration set in the 
ACIA peripheral, I haven't tested it though. This may be how `socat` works.

### About using hardware control signals (RTS and CTS) with `socat`

Sadly, `socat` does not allow the use of hardware control signals. If you wanted to use them on your ACIA, I would recommend you to use other serial port tunnel like [tty0tty](https://github.com/freemed/tty0tty). Sadly, this won't work on WSL2, at least at the time of writing, as it does not support custom kernel modules.

If you want to, you may use the `--hw2sw` option in the emulator. This converts the outgoing hardware control signal (#RTS) from the emulator into an incoming XON/XOFF software control signal to the serial port. Same thing in the other direction. If this option is used, be sure **Software Control (XON/XOFF)** is enabled on the serial port you are using.

**Bear in mind the `--hw2sw` option  will hinder the use of binary output (at least the `0x13` and `0x11` bytes) from the emulator.**

About the logic level of signals, bear in mind #RTS is **negated**, meaning that:

| #RTS Value | Meaning                      | Software Control equivalent |
|------------|------------------------------|-----------------------------|
| 0          | 6502 is ready to receive     | XON                         |
| 1          | 6502 is NOT ready to receive | XOFF                        |

# Debugging

If you are using VS Code:
1. Install C/C++ extension.
2. After installation (the [launch.json](.vscode/launch.json) will only work on Linux devices):
    - To run without debugging: run the task`Debug: Start Without Debugging` (`Ctrl + F5`) 
    - To debug: run the task `Debug: Start Debugging` (`F5`)

# TODO list
- Add the +1 cycle if crossing page boundary.
- Implement the real BRK instruction.
- Implement 65C02 instructions.