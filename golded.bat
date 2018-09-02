@echo off
rem
rem This is Golded's run command file example.
rem
rem
SET flagpath=M:\flag
SET fastechopath=M:\fastecho

rem Flag to creates if netmail wrote
SET _NET_FLAG=%flagpath%\netmail.flg

rem Flag to creates if echomail wrote
SET _ECHO_FLAG=%flagpath%\echomail.flg

rem Flag to creates if localmail wrote
SET _LOCAL_FLAG=%flagpath%\locmail.flg

rem Command runs if messagebase is damaged
SET _REPAIRACT=%fastechopath%\feutil check

rem Command runs if messagebase indexes is damaged
SET _REBUILDACT=%fastechopath%\feutil index


M:\GOLDED\GOLDED.EXE

If ErrorLevel 101 Goto ERROR
If ErrorLevel 100 Goto REBUILD
If ErrorLevel 008 Goto ERROR
If ErrorLevel 007 Goto E_N_L
If ErrorLevel 006 Goto E_N__
If ErrorLevel 005 Goto E___L
If ErrorLevel 004 Goto E____
If ErrorLevel 003 Goto __N_L
If ErrorLevel 002 Goto __N__
If ErrorLevel 001 Goto ____L
                  Goto _____

:ERROR
    Echo.
    Echo GoldED Error exit!
    %_REPAIRACT%
    Goto goldrun

:REBUILD
    Echo.
    Echo ERROR! Index files needs rebuilding!
    %_REBUILDACT%
    Goto GOLDRUN

:E_N_L
    Echo.
    Echo **** New Echo, Net and Local mail entered! ****
    cd . >>%_NET_FLAG%
    cd . >>%_LOCAL_FLAG%
    Goto purge

:E_N__
    Echo.
    Echo **** New Echo and Netmail entered! ****
    cd . >>%_NET_FLAG%
    cd . >>%_ECHO_FLAG%
    Goto purge

:E___L
    Echo.
    Echo **** New Echo and Local mail entered! ****
    cd . >>%_ECHO_FLAG%
    cd . >>%_LOCAL_FLAG%
    Goto purge

:E____
    Echo.
    Echo **** New Echomail entered! ****
    cd . >>%_ECHO_FLAG%
    Goto purge

:__N_L
    Echo.
    Echo **** New Net and Local mail entered! ****
    cd . >>%_NET_FLAG%
    %_LOCAL_FLAG%
    Goto purge

:__N__
    Echo.
    Echo **** New Netmail entered! ****
    cd . >>%_NET_FLAG%
    Goto purge

:____L
    Echo.
    Echo **** New Local mail entered! ****
    cd . >>%_LOCAL_FLAG%
    Goto purge

:_____
    Echo.
    Echo **** No new mail entered... ****
    Goto purge


:purge


rem Purge trashcan request.

If not Exist M:\SQUISH\TRASHCAN\*.msg Goto End

Echo.
Echo en. Area "Trashcan"- Recycle bin - contains some messages. Delete its?
Echo en.
Echo   .
Echo ru. В области "Trashcan" - коpзине для удалённых писем, находятся какие-то
Echo ru. сообщения. Очистить коpзину?
CHOICE /c:YN /T:N,05
IF ERRORLEVEL 2 goto Start

IF ERRORLEVEL 1 Del M:\SQUISH\TRASHCAN\*.msg

:End
