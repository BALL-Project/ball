// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PDBdefs.C,v 1.2 2005/02/12 23:08:27 oliver Exp $
//

#include <BALL/FORMAT/PDBdefs.h>

namespace BALL
{
	namespace PDB
	{
		const char* FORMAT_UNKNOWN = "%.6s";
		const char* FORMAT_ANISOU  = "%.6s%5ld %-4.4s%c%3.3s %c%4ld%c %7ld%7ld%7ld%7ld%7ld%7ld  %4.4s%2.2s%2.2s";
		const char* FORMAT_ATOM    = "%.6s%5ld %-4.4s%c%3.3s %c%4ld%c   %8.3f%8.3f%8.3f%6.2f%6.2f      %4.4s%2.2s%2.2s";
		const char* FORMAT_ATOM_PARTIAL_CRG = "%.6s%5ld %-4.4s%c%3.3s %c%4ld%c   %8.3f%8.3f%8.3f%6.2f%6.2f      %4.4s%4.4s";
		const char* FORMAT_AUTHOR  = "%.6s  %2ld%-60.60s";
		const char* FORMAT_CAVEAT  = "%.6s  %2ld %4.4s    %51.51s";
		const char* FORMAT_CISPEP  = "%.6s %3ld %3.3s %c %4ld%c   %3.3s %c %4ld%c       %3ld       %6f";
		const char* FORMAT_COMPND  = "%.6s  %2ld%-60.60s";
		const char* FORMAT_CONECT  = "%.6s%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld";
		const char* FORMAT_CRYST1  = "%.6s%9f%9f%9f%7f%7f%7f %11.11s%4ld";
		const char* FORMAT_DBREF   = "%.6s %4.4s %c %4ld%c %4ld%c %6.6s %8.8s %12.12s %5ld%c %5ld%c";
		const char* FORMAT_END     = "%.6s";
		const char* FORMAT_ENDMDL  = "%.6s";
		const char* FORMAT_EXPDTA  = "%.6s  %2ld%60.60s";
		const char* FORMAT_FORMUL  = "%.6s  %2ld  %3.3s %2ld%c%51.51s";
		const char* FORMAT_FTNOTE  = "%.6s %3ld %59.59s";
		const char* FORMAT_HEADER  = "%.6s    %-40.40s%9.9s   %4.4s";
		const char* FORMAT_HELIX   = "%.6s %3ld %3.3s %3.3s %c %4ld%c %3.3s %c %4ld%c%2ld%30.30s %5ld";
		const char* FORMAT_HET     = "%.6s %3.3s  %c%4ld%c  %5ld  %43.43s";
		const char* FORMAT_HETATM  = "%.6s%5ld %-4.4s%c%3.3s %c%4ld%c   %8.3f%8.3f%8.3f%6.2f%6.2f      %4.4s%2.2s%2.2s";
		const char* FORMAT_HETNAM  = "%.6s  %2ld %3.3s %55.55s";
		const char* FORMAT_HYDBND  = "%.6s      %4.4s%c%3.3s %c%5ld%c %4.4s%c %c%5ld%c %4.4s%c%3.3s %c%5ld%c%6ld %6ld";
		const char* FORMAT_JRNL    = "%.6s      %58.58s";
		const char* FORMAT_KEYWDS  = "%.6s  %2ld%60.60s";
		const char* FORMAT_LINK    = "%.6s      %4.4s%c%3.3s %c%4ld%c               %4.4s%c%3.3s %c%4ld%c  %6ld %6ld";
		const char* FORMAT_MASTER  = "%.6s    %5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld";
		const char* FORMAT_MODEL   = "%.6s    %4ld";
		const char* FORMAT_MODRES  = "%.6s %3.3s %3.3s %c %4ld%c %3.3s  %41.41s";
		const char* FORMAT_MTRIX1  = "%.6s %3ld%10f%10f%10f     %10f    %1ld";
		const char* FORMAT_MTRIX2  = "%.6s %3ld%10f%10f%10f     %10f    %1ld";
		const char* FORMAT_MTRIX3  = "%.6s %3ld%10f%10f%10f     %10f    %1ld";
		const char* FORMAT_OBSLTE  = "%.6s  %2ld %9.9s %4.4s      %4.4s %4.4s %4.4s %4.4s %4.4s %4.4s %4.4s %4.4s";
		const char* FORMAT_ORIGX1  = "%.6s    %10f%10f%10f     %10f";
		const char* FORMAT_ORIGX2  = "%.6s    %10f%10f%10f     %10f";
		const char* FORMAT_ORIGX3  = "%.6s    %10f%10f%10f     %10f";
		const char* FORMAT_REMARK  = "%.6s %3ld %-59.59s";
		const char* FORMAT_REVDAT  = "%.6s %3ld%2ld %9.9s %5.5s   %1ld       %6.6s %6.6s %6.6s %6.6s";
		const char* FORMAT_SCALE1  = "%.6s    %10f%10f%10f     %10f";
		const char* FORMAT_SCALE2  = "%.6s    %10f%10f%10f     %10f";
		const char* FORMAT_SCALE3  = "%.6s    %10f%10f%10f     %10f";
		const char* FORMAT_SEQRES  = "%.6s  %2ld %c %4ld  %3.3s %3.3s %3.3s %3.3s %3.3s %3.3s %3.3s %3.3s %3.3s %3.3s %3.3s %3.3s %3.3s";
		const char* FORMAT_SHEET   = "%.6s %3ld %3.3s%2ld %3.3s %c%4ld%c %3.3s %c%4ld%c%2ld %-4.4s%3.3s %c%4ld%c %-4.4s%3.3s %c%4ld%c";
		const char* FORMAT_SIGATM  = "%.6s%5ld %4.4s%c%3.3s %c%4ld%c   %8f%8f%8f%6f%6f      %4.4s%2.2s%2.2s";
		const char* FORMAT_SIGUIJ  = "%.6s%5ld %-4.4s%c%3.3s %c%4ld%c %7ld%7ld%7ld%7ld%7ld%7ld  %4.4s%2.2s%2.2s";
		const char* FORMAT_SITE    = "%.6s %3ld %3.3s %2ld %3.3s %c%4ld%c %3.3s %c%4ld%c %3.3s %c%4ld%c %3.3s %c%4ld%c";
		const char* FORMAT_SLTBRG  = "%.6s      %4.4s%c%3.3s %c%4ld%c               %4.4s%c%3.3s %c%4ld%c  %6ld%6ld";
		const char* FORMAT_SOURCE  = "%.6s  %2ld%-60.60s";
		const char* FORMAT_SSBOND  = "%.6s %3ld %3.3s %c %4ld%c   %3.3s %c %4ld%c                       %6ld %6ld";
		const char* FORMAT_TER     = "%.6s%5ld      %3.3s %c%4ld%c";
		const char* FORMAT_TITLE   = "%.6s  %2ld%60.60s";
		const char* FORMAT_TURN    = "%.6s %3ld %3.3s %3.3s %c%4ld%c %3.3s %c%4ld%c    %-30.30s";
		const char* FORMAT_TVECT   = "%.6s %3ld%10f%10f%10f%30.30s";

	} // namespace PDB

} // namespace BALL

