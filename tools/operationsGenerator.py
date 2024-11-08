"""
operationsGenerator.py

Simple tool to generate a bunch of operations to test acia_hw_ctrl.s
"""

import random

operators: tuple[str]   = ('+', '-', '*', '/')

count: int = int(input("How many?"))

with open('ops.txt', 'w') as file:
    for i in range(count):
        op1: int = random.randint(0, 0xffff)
        op2: int = random.randint(0, 0xffff)
        op:  str = random.choice(operators)

        strOp: str = f'{op1}{op}{op2}\r'
        print(strOp)
        file.write(strOp)
        