// $Id: versions.sip,v 1.1 2001/12/27 00:25:44 oliver Exp $

%SecondaryVersion Unix
	"!defined(__win32)" "libBALLc.isUnix()"

%SecondaryVersion Win
	"defined(__win32)" "libBALLc.isWin()"

