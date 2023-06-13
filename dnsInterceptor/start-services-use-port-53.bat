@echo off

net start SharedAccess

net start hns

timeout /t 2 >nul
