@echo off
if "%1" == "clean" (
    del /s /q *.o
    del /q main.elf
    del /q main.hex
    make Burn SHELL=cmd
) else if "%1" == "normal" (
    make Burn SHELL=cmd
) else (
    echo Usage:
    echo %0 [clean ^| normal]
)