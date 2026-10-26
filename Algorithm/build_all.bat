@echo off
rem Build all lecture programs with MinGW gcc (Korean output = CP949)
if not exist bin mkdir bin
for %%d in (ch08_tree ch09_priority_queue ch10_graph1 ch11_graph2) do (
  for %%f in (%%d\*.c) do (
    echo Building %%f
    gcc -fexec-charset=CP949 "%%f" -o "bin\%%d_%%~nf.exe"
  )
)
echo.
echo Done. Executables are in the bin folder.
pause
