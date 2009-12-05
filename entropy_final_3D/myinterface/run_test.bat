@ECHO OFF

SETLOCAL

IF '%1' == '' (
	ECHO Usage: run_test.bat ^<tag^>
	GOTO END
)

SET PLATFORM=%1

REM	FOR %%d in (tsi) DO (
FOR %%d in (plume tsi) DO (
REM	FOR %%d in (saddle electro equation circle source) DO (
	Release\myinterface f:\\data\flow_entropy\3D\%%d.vec > log.txt
	mkdir result
	mkdir result\%%d
	SET pp=result\%%d\%PLATFORM%
	mkdir !pp!
	MOVE log.txt !pp!
	MOVE entropy !pp!
	MOVE importance.bin !pp!
	MOVE myseeds.seed !pp!
	MOVE r.vec !pp!
)

:END

