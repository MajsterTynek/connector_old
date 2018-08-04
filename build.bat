@rem epilogue
@ECHO OFF
setlocal
call :main argc argv envp
goto :EOF

rem Thanks to Gynvael Coldwind. // M.S. ;P
rem "goto :EOF" inside called func/label means return

rem  This batch script is under "Spaghetti license":
rem "Feel free to use any part of this code
rem  for your own projects. Enjoy your spaghetti! ".

:sizeoffile
    set lastsize=%~z1
    goto :EOF

:build_failed
    echo.Build failed.
    goto :EOF

:main
    g++ main.cpp -Wall -lws2_32 -o connector.exe 
    if errorlevel 1 goto build_failed

    echo.Build succed.
    goto :EOF