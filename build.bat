@echo off

mkdir bin 2> NUL > NUL

where /q cl
IF errorlevel 1 (
    where /q gcc
    IF errorlevel 1 (
        echo "ERROR: Make sure to have msvc or mingw installed!"
        @echo on
        exit /b
    ) ELSE (
        gcc -Wall -Wextra -pedantic -ggdb -o bin\clip2file src\main.c 
    )
) ELSE (    
    cl /W4 /Fe:bin\clip2file src\main.c user32.lib
)

@echo on


