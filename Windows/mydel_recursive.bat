@echo off

:DO

if not "%1" == "" (
	del /Q /S "%1" > NUL 2> NUL
	shift
	goto DO
)