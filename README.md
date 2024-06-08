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

<img src="https://cdn.discordapp.com/attachments/847075046329679903/1248822119819903017/howbg_2.gif?ex=66650fbb&is=6663be3b&hm=e9cae8568db353b3ff14dde5e770bc4c6580c7686873c9f7d6adb028b86f4f09&" alt="Image could not be loaded" width="723" height="108">

### 1. Select a process
Find the target process in the 'process list' and select it

### 2. Select your DLL
Click on the 'inject' button and choose your DLL

<img src="https://cdn.discordapp.com/attachments/847075046329679903/1248823018617176164/gg.gif?ex=66651091&is=6663bf11&hm=a3c3c3aac9f8d6db984f2f77e570bcde3ec62829fa04e70c75edfca4e71c6730&" alt="Image could not be loaded" width="540" height="88">

### 1. Clone this project using Git
```bash
git clone https://github.com/Z1KOx/Injector.git
```
- If you don't have Git installed, you can download it <a href="https://git-scm.com/downloads">here</a>

### 2. Run build.bat
This will compile and link the executable, and it will provide you with the path where the .exe file is located.
