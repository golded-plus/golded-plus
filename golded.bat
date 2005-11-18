@echo off
rem
rem This is Golded's run command file example.
rem
rem

rem Command runs if netmail wrote
SET _NET_ACT=cd.>>M:\flag\netmail.flg

rem Command runs if echomail wrote
SET _ECHOACT=cd.>>M:\flag\netmail.flg

rem Command runs if localmail wrote
SET _LOCALACT=cd.>>M:\flag\netmail.flg

rem Command runs if messagebase is damaged
SET _REPAIRACT=M:\fastecho\feutil check

rem Command runs if messagebase indexes is damaged
SET _REBUILDACT=M:\fastecho\feutil index


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
    %_NET_ACT%
    %_LOCALACT%
    Goto purge

:E_N__
    Echo.
    Echo **** New Echo and Netmail entered! ****
    %_NET_ACT%
    %_ECHOACT%
    Goto purge

:E___L
    Echo.
    Echo **** New Echo and Local mail entered! ****
    %_ECHOACT%
    %_LOCALACT%
    Goto purge

:E____
    Echo.
    Echo **** New Echomail entered! ****
    %_ECHOACT%
    Goto purge

:__N_L
    Echo.
    Echo **** New Net and Local mail entered! ****
    %_NET_ACT%
    %_LOCALACT%
    Goto purge

:__N__
    Echo.
    Echo **** New Netmail entered! ****
    %_NET_ACT%
    Goto purge

:____L
    Echo.
    Echo **** New Local mail entered! ****
    %_LOCALACT%
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

IF ERRORLEVEL 1 Goto Del M:\SQUISH\TRASHCAN\*.msg

:End
