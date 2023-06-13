@echo off

reg add "HKLM\SYSTEM\CurrentControlSet\services\Dnscache" /v Start /t REG_DWORD /d 2 /f

timeout /t 2 >nul
