# Simple DLL Backdoor
<b>A simple reverse shell DLL</b>

# What does each folder have and does
1. <b>DLL:</b>
   - Contains Repair.dll and main.cpp (source code of Repair.dll)
   - This DLL is a reverse shell that spawns powershell
   - Connects to IP: 127.0.0.1 and Port 4444
     - change it from the code

2. <b>Injector:</b>
   - Contains Injector.exe and main.cpp (source code of Injector.exe)
   - The Injector injects to the process msedge.exe
     - auto process = "msedge.exe";
       Change the process according to your wish and it accepts DLLs
   - The Injector.exe's source code specify the complete path of Repair.dll
     - Complete path of the DLL should be specified to be injected by the Injector
       - const char* dllPath = "D:/Programming/C++/High_Shellcode_with_SE/DLL/Repair.dll";
         Change the path to the dll according to yours

3. <b>PrivEscalationVBScript</b>
   - Contains FAKE_UAC.vbs, VBSRepair.dll and main.cpp (modified source code of Repair.dll)
   - The FAKE_UAC.vbs script uses rundll32.exe command to run the VBSRepair.dll 
   - A UAC pops up until the Yes button is pressed which spawns an Administrator powershell
   - VBSRepair.dll uses IP: 127.0.0.1 and Port 4444

# TODO:
- Implement persistence for all the exe
- Create a C++ executable to execute the VB Script
- Implement some error handling for the DLL source code
- Implement a feature to check a specified github file for what server IP and Port it should be used<br>to connect to the C2 server
- Make new folder that uses DLL Hollowing 
    Reference: https://trustedsec.com/blog/burrowing-a-hollow-in-a-dll-to-hide -> Make it in C++

<br><br>Project created in CLion