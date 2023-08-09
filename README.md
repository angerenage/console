# Retro Game Console

## Introduction

Hello and welcome to my Retro Game Console Emulator project!

My name is Ange, and I'm a computer enthusiast with a deep-rooted fondness for the constraints and ingenuity of vintage gaming consoles. These iconic machines from the past not only defined generations but were also marvels of technology given the limitations of their era. Their unique challenges, combined with my penchant for understanding and designing computational systems, spurred me to embark on this ambitious project.

While I have dabbled in crafting simple processors in the past, this endeavor is unlike anything I've attempted before. The objective here is to design and emulate a hypothetical game console inspired by the old legends. This project is an amalgamation of the classic console architecture's from between the 5th and 6th generation combined with modern understanding and a hint of personal creativity.

However, it's essential to note that this undertaking is purely a hobby. So, while I'm passionate and excited about it, it's essential to set expectations right: the journey, if it comes to an end, will be filled with learning, exploration, mistakes, and improvements.

If you spot inconsistencies, potential issues, or areas of improvement in the architecture, please don't hesitate to share! Your feedback and suggestions are invaluable, and collaborative efforts are the cornerstone of open-source wonders. Remember, it's a collective voyage, and every contribution, big or small, plays a pivotal role.

Thank you for joining me in this nostalgic and educational endeavor. Let's recreate the magic of the bygone era and build something remarkable together!

## Contributing

`/!\ The emulator will be written in C++ with GLFW or SLD for the controls /!\`

Your contributions are not only welcomed but also highly appreciated! This project thrives on the collective knowledge and expertise of the community. Here's how you can contribute:

1. **Pull Requests**: 
    - **Fork** the repository.
    - Make your desired changes or additions in your forked repo.
    - Submit a **pull request**. Please ensure your code is well-documented, and if possible, add tests. I will review all pull requests and merge them if they fit the project's direction and quality standards.
   
2. **Issues**: If you encounter any bugs, inconsistencies, or even have a suggestion, feel free to open an issue. Detailed bug reports are incredibly beneficial.

3. **Community Discussions**: The more the project grows, the more we'll need a space to communicate more dynamically. If the project garners enough attention and interest, a Discord server or a similar platform could be set up for more direct conversations, brainstorming, and troubleshooting. Your active participation and feedback will play a huge role in this decision.

Remember, every contribution, no matter how minor it may seem, is invaluable for the project's growth and improvement.

### License
This project is licensed under the **GNU General Public License v3.0**. Please make sure you comply with its terms while using or distributing this project. You can read more about the license [here](</LICENSE>).

---

# CPU

## General Specifications

- **Clock Speed**: 50MHz for each core.
- **Cache**:
    - **L1 Cache**: 8KB per core (divided between I-Cache and D-Cache).
    - **L2 Cache**: 32KB or 64KB shared between the two cores.

## Instruction Set

1. **Basic Arithmetic Operations:**
	- `ADD` (Addition)
	- `SUB` (Subtraction)
	- `MUL` (Multiplication)
	- `DIV` (Division)
   
2. **Logic Operations:**
	- `AND` (Bitwise AND)
	- `OR` (Bitwise OR)
	- `XOR` (Bitwise XOR)
	- `NOT` (Bitwise NOT)
	- `LSL` (Logical Shift Left)
	- `LSR` (Logical Shift Right)
	- `ROL` (Rotate Left)
	- `ROR` (Rotate Right)
   
3. **Flow Control:**
	- `JMP` (Unconditional Jump)
	- `Bcc` (Conditional Branches, where "cc" could be one of the following conditions)
        - **EQ**: Equal
        - **NE**: Not Equal
        - **CS/HS**: Carry Set/Unsigned higher or same
        - **CC/LO**: Carry Clear/Unsigned lower
        - **MI**: Negative
        - **PL**: Positive or Zero
        - **VS**: Overflow
        - **VC**: No Overflow
        - **HI**: Unsigned Higher
        - **LS**: Unsigned Lower or Same
        - **GE**: Greater than or Equal (signed)
        - **LT**: Less Than (signed)
        - **GT**: Greater Than (signed)
        - **LE**: Less than or Equal (signed)
	- `JSR` (Jump to Subroutine)
	- `RTS` (Return from Subroutine)
   
4. **Memory Access:**
	- `LD` (Load from memory to a register)
	- `ST` (Store from a register to memory)
	- `PUSH` (Push to stack)
	- `POP` (Pop from stack)
   
5. **Special Operations:**
	- `MATMUL` (Matrix Multiplication for transformation operations)
   
6. **Miscellaneous:**
	- `NOP` (No Operation, can be useful in various scenarios, like waiting for some operations to complete)
	- `HALT` (Halts the CPU)
	- `FETCH` (Block copy using burst mode)

## Registers

### General-Purpose Registers:

1. **Data Registers (D0-D7)**:
    - **Size**: 32 bits each.
    - **Use**: Used for general computations, arithmetic operations, and data storage.

2. **Address Registers (A0-A6)**:
    - **Size**: 32 bits each.
    - **Use**: Used for addressing and pointer operations.

### Special-Purpose Registers:

3. **SP (Stack Pointer or A7)**:
    - **Size**: 32 bits.
    - **Use**: Points to the top of the stack, managing subroutine calls, returns, and local variable storage.

4. **PC (Program Counter)**:
    - **Size**: 32 bits.
    - **Use**: Holds the address of the next instruction to be executed.

5. **SR (Status Register)**:
    - **Size**: 16 bits
    - **Use**: Represents the state of the processor, including flags and system status bits.

6. **IR (Instruction Register)**:
    - **Size**: 32 bits.
    - **Use**: Holds the currently executing instruction.

7. **BL (Bus Lock Register)**:
    - **Size**: 1 bit.
    - **Use**: Used to manage bus access and ensure atomic operations.

8. **CID (Core ID Register)**:
    - **Size**: 1 bit.
    - **Use**: Identifies the CPU core, assuming a dual-core system.

9. **IER (Interrupt Enable Register)**:
    - **Size**: 16 bits
    - **Use**: Controls which interrupts are enabled or disabled. Each bit corresponds to a different interrupt source.

## Interrupt System

### Overview:
The interrupt system facilitates asynchronous communication between hardware peripherals and the CPU. When a peripheral requires the CPU's attention, it sends an interrupt signal. The CPU then momentarily halts its current operation, services the interrupt, and subsequently resumes its original task.

### Components:

1. **Interrupt Controller**: 
   - Role: Monitors interrupt lines for signals and manages the dispatch of corresponding Interrupt Service Routines (ISRs).
   - Features:
        - Equipped with memory-mapped registers dedicated to storing ISR pointers.
        - **Interrupt Status Register**: Provides information about which interrupt was triggered. When an interrupt is triggered, its respective bit is set.

2. **ISR Pointers**: 

   - Role: Point to the starting address of an ISR in memory.
   - Storage: Stored in the memory-mapped registers within the interrupt controller.
   - Configuration: To enable or disable an interrupt, system software writes to the designated memory-mapped register. An ISR pointer set to null (e.g., 0x00) effectively disables that specific interrupt.

### Interrupt Handling Process:

1. **Interrupt Detection**: The interrupt controller continuously checks interrupt lines for any active signals. If several interrupts are triggered at the same time, a priority is defined materially.

2. **ISR Pointer Check**: Upon detecting an active interrupt signal, the controller reviews the corresponding ISR pointer.
    - If null (0x00): The interrupt is deemed disabled, and the CPU continues its ongoing task without disruption.
    - If valid: The process advances to the next step.

3. **Register Swap**: The active CPU register set is swapped out, ensuring the current operational context is securely stored.

4. **Update Program Counter (PC)**: The PC adjusts to the address specified by the ISR pointer.

5. **Execute ISR**: The CPU starts processing instructions from the ISR address. This continues until the system encounters an RTS (Return from Subroutine) instruction or an equivalent.

6. **Exit Interrupt Routine**: Upon RTS execution, the previously saved register set is restored, reinstating the original operational context.

7. **Resume Execution**: The CPU continues with its prior task from the point of interruption.

### Interrupt Management:
- **Setting ISR Pointers**: To adjust the ISR pointer for a specific interrupt, the system writes the desired address to the corresponding memory-mapped register in the interrupt controller.
- **Enabling/Disabling Interrupts**: By writing a null (e.g., 0x00) value to the ISR pointer, the interrupt gets disabled. Writing a valid address re-enables it.

---

# GPU

## General Specifications

- **Clock Speed**: 100MHz.
- **Resolution**: Maximum of 480p.
- **Aspect Ratio**: 16:9.
- **Color Depth**: 24-bit color (16 million colors).
- **Texture Filtering**: Trilinear filtering on mipmaps.
- **Shading Model**: Phong interpolation for more realistic lighting.
- **VRAM**: 1.5MB.
- **Video Output**: VGA (Video Graphics Array) port.

---

# MEMORY

## General Specifications

- **RAM**: 4MB.

## virtual memory map

1. **Bootloader**: store in the first part of the memory, load the bios and other essential element of the console.
2. **RAM**: the ram itself, which will contain all the program informations.
3. **Hard Drive**: provides access to the hard drive of the console.
4. **CD Rom**: allows access to the disk drive and therefore to the game which will be loaded into memory by the bootloader.
5. **Interrupt Controller**: provides access to the mapped registers of the interrupt controller.

---

# AUDIO

## Sound Digital Signal Processor (DSP) General Specifications

- **Clock Speed**: 50MHz.
- **Sound Channels**: Supports 32 simultaneous channels.

---

# INPUT/OUTPUT
- **Controller Interface**: Serial interface for game controllers. Controls are not managed by interrupt but by polling.

---

# STORAGE

- **Primary Game Media**: CDs.

## 16MB Mini Hard Disk Structure

1. ### Capacity Distribution:
    - **Game Saves**: Approximately 13-14MB reserved primarily for game saves.
    - **Metadata & Thumbnails**: Reserve 2-3MB specifically for metadata and thumbnail images.
2. ### Game Save Metadata:
    - **Hash**: A unique identifier for each save to ensure data integrity and help prevent corruption.
    - **Game Title/ID**: Identifies which game the save belongs to.
    - **Date & Time**: Timestamp of when the game was saved.
    - **Thumbnail**: A visual snapshot or icon representing the game save. This could be an in-game screenshot or a predefined image associated with the game.
    - **Other Metadata**: Player name, playtime, in-game location, etc.
3. ### User Interface:
    - **List View**: Display game saves in a list or grid format with a scroll function.
    - **Sorting Options**: Allow sorting by date, game title, or player name.
    - **Thumbnail Preview**: Hovering or selecting a save could enlarge the thumbnail for a clearer view.
    - **Deletion**: Offer easy options for players to delete older saves.

---
