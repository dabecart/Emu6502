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

# TODO list
- Add the +1 cycle if crossing page boundary.
- Implement the real BRK instruction.