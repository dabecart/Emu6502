import xml.etree.ElementTree as ET
from dataclasses import dataclass

@dataclass
class Instruction():
    mnemonic: str
    comment: str
    opcode: int
    addressing: str|None    = None
    byteLen: int      = 0
    clockCycles: int        = 0

    def toString(self) -> str:
        if self.addressing:
            addressing = self.addressing + ','
            return f'{{"{self.mnemonic}", {self.opcode :#04x}, ' \
                f'{addressing :<30} {self.byteLen}, {self.clockCycles}, ' \
                f'{self.mnemonic}_ins_}},  // {self.mnemonic}: {self.comment}'
        else:
            return f'{{"{self.mnemonic}", {self.opcode :#04x}, INVALID_ADDRS}},' \

def parseAddressing(addrs: str) -> str:
    match addrs:
        case "absolute": return "ABS_ADDRS"
        case "absolute,X": return "ABS_INDEXED_X_ADDRS"
        case "absolute,Y": return "ABS_INDEXED_Y_ADDRS"
        case "indirect": return "ABS_INDIRECT_ADDRS"
        case "accumulator": return "ACCUMULATOR_ADDRS"
        case "immediate": return "IMMEDIATE_ADDRS"
        case "implied": return "IMPLIED_ADDRS"
        case "relative": return "PROGRAM_COUNTER_ADDRS"
        case "zeropage": return "ZP_ADDRS"
        case "(indirect,X)": return "ZP_INDEXED_INDIRECT_ADDRS"
        case "zeropage,X": return "ZP_INDEXED_X_ADDRS"
        case "zeropage,Y": return "ZP_INDEXED_Y_ADDRS"
        case "(zeropage)": return "ZP_INDIRECT_ADDRS"
        case "(indirect),Y": return "ZP_INDIRECT_INDEXED_Y_ADDRS"
        case _:
            print(f"Undefined addressing {addrs}")
    return ""

def parseXML(file_path) -> dict[int, Instruction]:
    instructions: dict[int, Instruction] = {}

    tree = ET.parse(file_path)
    root = tree.getroot()

    # Initialize an index to pair <dt> and <dd> tags.
    dt_dd_pairs = zip(root.findall("dt"), root.findall("dd"))

    for dt, dd in dt_dd_pairs:
        # Extract the mnemonic name.
        mnemonic = dt.text.strip()

        # Extract the summary from the <p> tag with aria-label="summary" in <dd>.
        summary = ""
        summary_tag = dd.find(".//p[@aria-label='summary']")
        if summary_tag is not None:
            summary = summary_tag.text.strip()

        # Extract details table data (addressing mode and opcode).
        for tr in dd.findall(".//table[@aria-label='details']//tr")[1:]:  # Skip header row.
            addressing_td = tr.find("td[1]")
            opCode_td = tr.find("td[3]")
            argumentCount_td = tr.find("td[4]")
            cycles_td = tr.find("td[5]")

            if addressing_td is not None and opCode_td is not None \
                and cycles_td is not None and argumentCount_td is not None:
                addrs = parseAddressing(addressing_td.text.strip())
                opCode = int(opCode_td.text.strip(), 16)
                byteLen = int(argumentCount_td.text.strip())
                clockCycles = int(cycles_td.text.strip())

                instructions[opCode] = Instruction(mnemonic=mnemonic,
                                                   comment=summary,
                                                   opcode=opCode,
                                                   addressing=addrs,
                                                   byteLen=byteLen,
                                                   clockCycles=clockCycles)
    return instructions

def getFunctions(opers: dict[int, Instruction]) -> set[str]:
    funcs: set[str] = set()
    for val in opers.values():
        funcs.add(f"void {val.mnemonic}_ins_(CPU* cpu, CPUInstruction* instruction, uint16_t dir, uint8_t data);")
    return sorted(funcs)

operations: dict[int, Instruction] = parseXML("tools/instructions.xml")
funcs = getFunctions(operations)

for f in funcs:
    print(f)

# for i in range(256):
#     if i in operations:
#         print(operations[i].toString())
#     else:
#         notDefined = Instruction('IND', 'Instruction Not Defined', i)
#         print(notDefined.toString())