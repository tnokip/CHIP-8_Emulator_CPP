# Guide to buiding the emulator for your system

## Linux

### 1. Install Prerequisites
#### Ubuntu/Debian
```
sudo apt update
sudo apt install build-essentials cmake libsdl3-dev git
```

#### Fedora
```
sudo dnf update
sudo dnf install build-essentials cmake git libsdl3-dev
```

#### Arch
```
sudo pacman -S sdl3
```

#### See Step 3.5 to download the SDL3 packages dynamically
### 2. Clone Github Repository at desired location
```
git clone https://github.com/tnokip/CHIP-8_Emulator_CPP.git
cd CHIP-8_Emulator_CPP
```

### 3.Create a Build Directory
```
mkdir build
cd build
```

### 3.5(optional) Dynamically download SDL3 packages if not available in your respective Package Manager (replace 3.0.0 with the latest version) 
```
<!-- Using wget --!>
wget https://github.com/libsdl-org/SDL/releases/download/release-3.0.0/SDL3-3.0.0.tar.gz
tar -xzf SDL3-3.0.0.tar.gz

<!-- Using curl --!>
curl -L -o SDL3-3.0.0.tar.gz https://github.com/libsdl-org/SDL/releases/download/release-3.0.0/SDL3-3.0.0.tar.gz
tar -xzf SDL3-3.0.0.tar.gz
```

### 4. Run CMake  to configure
```
cmake ..
cmake --build .
```

#### 4.5 Dynamically link libraries if downloaded via step 3.5
```
cmake .. -DSDL3_DIR=path/to/your/< **SDL3Config.cmake** file>
cmake --build .
```

### 5.Run the program
```
./Chip8
```

## Windows
#### Download the correct latest SDL3-devel from https://github.com/libsdl-org/SDL/releases according to your compiler, extract the zip file

#### Open PowerShell as administrator

### Clone the repository
```
git clone https://github.com/tnokip/CHIP-8_Emulator_CPP.git
cd CHIP-8_Emulator_CPP
```

#### Create Build Directory
```
mkdir build
cd build
```

#### Copy and Paste the SDL.dll file inside build folder
#### Run CMake to Configure
```
cmake .. -DCMAKE_BUILD_TYPE=Release -DSDL3_DIR=path/to/your/< **SDL3Config.cmake** file>
cmake --build .

<!-- Using mingw --!>
cmake -G "MinGW Makefiles" .. -DCMAKE_BUILD_TYPE=Release -DSDL3_DIR=path/to/your/< **SDL3Config.cmake** file>
cmake --build .

```

### Run the program by opening the exe file

