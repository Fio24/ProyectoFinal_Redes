@echo off

net stop hns

net stop SharedAccess

timeout /t 2 >nul
