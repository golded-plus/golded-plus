@echo off
for %%i in (map\*.txt) do for %%j in (map\*.txt) do chsgen %%i %%j
