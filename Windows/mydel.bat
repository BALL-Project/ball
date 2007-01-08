@echo off

:DO

if not "%1" == "" (
	del /Q "%1"  > NUL 2> NUL
	shift
	goto DO
)