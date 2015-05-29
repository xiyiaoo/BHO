@echo off
cd /d %~dp0
dir
copy /y Hook.dll C:\Windows
regsvr32 C:\Windows\Hook.dll
pause