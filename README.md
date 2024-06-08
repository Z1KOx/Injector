<img src="https://cdn.discordapp.com/attachments/847075046329679903/1244424817801957437/brave-HSoN9lpqE5-unscreen.gif?ex=6664e26c&is=666390ec&hm=01f1e0d674580916c28fefc054374c0adbdaac910c460a288e096b6165ee5cfc&" alt="Image could not be loaded" width="474" height="146">

Injects for you a dll into a process

> [!NOTE]
> - This injector has been tested on the Windows operating system.
> - To build this project, you need to have CMake installed. You can install it <a href="https://cmake.org/download/">here</a>

<img src="https://cdn.discordapp.com/attachments/847075046329679903/1248821372713570455/howbg.gif?ex=66650f09&is=6663bd89&hm=19c2621bb98d2000143b5988e195f61b6b06e8753c7df2594015c615899baf2e&" alt="Image could not be loaded" width="728" height="108">

### 1. Identify the target process
Determine the process where the DLL will be injected

### 2. Open the target process
Access the target process, enabling memory manipulation

### 3. Allocate memory in the target process
Reserve memory within the target process for loading the DLL

### 4. Write the DLL file path into the allocated memory
Copy the DLL file path into the reserved memory space of the target process

### 5. Create a remote thread
Generate a thread within the target process to load the DLL

# How to use this injector?
### 1. Select a process
Find the target process in the 'process list' and select it

### 2. Select your DLL
Click on the 'inject' button and choose your DLL

# Getting Started
### 1. Clone this project using Git
```bash
git clone https://github.com/Z1KOx/Injector.git
```
- If you don't have Git installed, you can download it <a href="https://git-scm.com/downloads">here</a>

### 2. Run build.bat
This will compile and link the executable, and it will provide you with the path where the .exe file is located.
