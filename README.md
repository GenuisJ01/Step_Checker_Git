# Final Year Project

Step Checker, a malware detection system that checks for human interaction, vmware and hardware. Also, uses a few timing attacks and uses anti analysis, dumping and debugging
methods.

Every import is included in common.h, including the global wow64 variable. 
API.h and utils.hpp include the WMI steps and the only API library required for the functions, ntdll.

Each other required source file is in a separate directory, and have descriptions. 

Important part of the timing attack source file is that the delay has been lowered so that later tests can go off. The recommended time is put in comments

Be sure to change the API.cpp SetDllDirectoryA() to where the project is in your directory!