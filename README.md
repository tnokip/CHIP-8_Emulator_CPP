# CHIP-8 Emulator

A simple CHIP-8 emulator written in C++ with SDL3.  
This is my very first emulator project, built as a learning exercise.  

⚠️ **Note:** The current version is buggy and incomplete. It runs some programs but breaks on collision in Pong and possibly other games. 
I am keeping it this way until I gain more knowledge, so this repository serves as a snapshot of my learning journey.

---

## ✨ Features
- Basic CHIP-8 instruction set implementation
- SDL3-based graphics output
- Keyboard input mapping
- Runs simple ROMs (Version from 0.1 till the next can run only the IBM logo)

---

## 🚧 Current Limitations
- Collision handling is broken (e.g., Pong crashes on ball/paddle collision, Tetris only renders the first piece again and again)
- Some opcodes may not be fully implemented or accurate
- Performance and timing are not yet tuned

---

## 📦 Releases
- **Buggy Version:** Will be published as the first release to preserve my initial attempt.
- **Proper Version:** Planned for the future, after I acquire more knowledge and fix the issues.

---

## 🛠 Build Instructions
   ```bash
   git clone https://github.com/<your-username>/CHIP-8_Emulator_CPP.git
   cd CHIP-8_Emulator_CPP
   make
   ./chip8

    
