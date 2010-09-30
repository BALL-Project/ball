// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PDBdefs.C,v 1.5 2005/02/23 11:34:18 oliver Exp $
//

#include <BALL/FORMAT/PDBdefs.h>

namespace BALL
{

	namespace PDB
	{

		const char* FORMAT_UNKNOWN = "";
		const char* FORMAT_ANISOU  = "%5ld %-4.4s%c%3.3s %c%4ld%c %7ld%7ld%7ld%7ld%7ld%7ld  %4.4s%2.2s%2.2s";
		const char* FORMAT_ATOM    = "%5ld %-4.4s%c%3.3s %c%4ld%c   %8.3f%8.3f%8.3f%6.2f%6.2f      %4.4s%2.2s%2.2s";
		const char* FORMAT_ATOM_PARTIAL_CRG = "%5ld %-4.4s%c%3.3s %c%4ld%c   %8.3f%8.3f%8.3f%6.2f%6.2f      %4.4s%4.4s";
		const char* FORMAT_AUTHOR  = "  %2ld%-60.60s";
		const char* FORMAT_CAVEAT  = "  %2ld %4.4s    %51.51s";
		const char* FORMAT_CISPEP  = " %3ld %3.3s %c %4ld%c   %3.3s %c %4ld%c       %3ld       %6f";
		const char* FORMAT_COMPND  = "  %2ld%-60.60s";
		const char* FORMAT_CONECT  = "%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld";
		const char* FORMAT_CON06   = "%5ld%5ld%5ld%5ld%5ld";	
		const char* FORMAT_CON061 = "%5ld";	
		const char* FORMAT_CON062 = "%5ld%5ld";	
		const char* FORMAT_CON063 = "%5ld%5ld%5ld";	
		const char* FORMAT_CON064 = "%5ld%5ld%5ld%5ld";	
		const char* FORMAT_CRYST1  = "%9.3f%9.3f%9.3f%7.2f%7.2f%7.2f %-11.11s%4ld";
		const char* FORMAT_DBREF   = " %4.4s %c %4ld%c %4ld%c %6.6s %8.8s %12.12s %5ld%c %5ld%c";
		const char* FORMAT_END     = "";
		const char* FORMAT_ENDMDL  = "";
		const char* FORMAT_EXPDTA  = "  %2ld%60.60s";
		const char* FORMAT_FORMUL  = "  %2ld  %3.3s %2ld%c%51.51s";
		const char* FORMAT_FTNOTE  = " %3ld %59.59s";
		const char* FORMAT_HEADER  = "    %-40.40s%9.9s   %4.4s";
		const char* FORMAT_HELIX   = " %3ld %3.3s %3.3s %c %4ld%c %3.3s %c %4ld%c%2ld%30.30s %5ld";
		const char* FORMAT_HET     = " %3.3s  %c%4ld%c  %5ld  %43.43s";
		const char* FORMAT_HETATM  = "%5ld %-4.4s%c%3.3s %c%4ld%c   %8.3f%8.3f%8.3f%6.2f%6.2f      %4.4s%2.2s%2.2s";
		const char* FORMAT_HETNAM  = "  %2ld %3.3s %55.55s";
		const char* FORMAT_HETSYN  = "  %2ld %3.3s %55.55s";
		const char* FORMAT_HYDBND  = "      %4.4s%c%3.3s %c%5ld%c %4.4s%c %c%5ld%c %4.4s%c%3.3s %c%5ld%c%6ld %6ld";
		const char* FORMAT_JRNL    = "      %58.58s";
		const char* FORMAT_KEYWDS  = "  %2ld%60.60s";
		const char* FORMAT_LINK    = "      %4.4s%c%3.3s %c%4ld%c               %4.4s%c%3.3s %c%4ld%c  %6ld %6ld";
		const char* FORMAT_MASTER  = "    %5d%5d%5d%5d%5d%5d%5d%5d%5d%5d%5d%5d";
		const char* FORMAT_MODEL   = "    %4ld";
		const char* FORMAT_MODRES  = " %3.3s %3.3s %c %4ld%c %3.3s  %41.41s";
		const char* FORMAT_MTRIX1  = " %3ld%10.6f%10.6f%10.6f     %10.5f    %1ld";
		const char* FORMAT_MTRIX2  = " %3ld%10.6f%10.6f%10.6f     %10.5f    %1ld";
		const char* FORMAT_MTRIX3  = " %3ld%10.6f%10.6f%10.6f     %10.5f    %1ld";
		const char* FORMAT_OBSLTE  = "  %2ld %9.9s %4.4s      %4.4s %4.4s %4.4s %4.4s %4.4s %4.4s %4.4s %4.4s";
		const char* FORMAT_ORIGX1  = "    %10f%10f%10f     %10f";
		const char* FORMAT_ORIGX2  = "    %10f%10f%10f     %10f";
		const char* FORMAT_ORIGX3  = "    %10f%10f%10f     %10f";
		const char* FORMAT_REMARK  = " %3ld %-59.59s";
		const char* FORMAT_REVDAT  = " %3ld%2ld %9.9s %5.5s   %1ld       %6.6s %6.6s %6.6s %6.6s";
		const char* FORMAT_SCALE1  = "    %10f%10f%10f     %10f";
		const char* FORMAT_SCALE2  = "    %10f%10f%10f     %10f";
		const char* FORMAT_SCALE3  = "    %10f%10f%10f     %10f";
		const char* FORMAT_SEQADV  = " %4.4s %4.4s %c %4ld%c %4.4s %9.9s %3.3s %5ld %21.21s";
		const char* FORMAT_SEQRES  = "  %2ld %c %4ld  %3.3s %3.3s %3.3s %3.3s %3.3s %3.3s %3.3s %3.3s %3.3s %3.3s %3.3s %3.3s %3.3s";
		const char* FORMAT_SHEET   = " %3ld %3.3s%2ld %3.3s %c%4ld%c %3.3s %c%4ld%c%2ld %-4.4s%3.3s %c%4ld%c %-4.4s%3.3s %c%4ld%c";
		const char* FORMAT_SIGATM  = "%5ld %4.4s%c%3.3s %c%4ld%c   %8f%8f%8f%6f%6f      %4.4s%2.2s%2.2s";
		const char* FORMAT_SIGUIJ  = "%5ld %-4.4s%c%3.3s %c%4ld%c %7ld%7ld%7ld%7ld%7ld%7ld  %4.4s%2.2s%2.2s";
		const char* FORMAT_SITE    = " %3ld %3.3s %2ld %3.3s %c%4ld%c %3.3s %c%4ld%c %3.3s %c%4ld%c %3.3s %c%4ld%c";
		const char* FORMAT_SLTBRG  = "      %4.4s%c%3.3s %c%4ld%c               %4.4s%c%3.3s %c%4ld%c  %6ld%6ld";
		const char* FORMAT_SOURCE  = "  %2ld%-60.60s";
		const char* FORMAT_SPRSDE  = "  %2ld %9.9s %4.4s %4.4s %4.4s %4.4s %4.4s %4.4s %4.4s %4.4s %4.4s ";
		const char* FORMAT_SSBOND  = " %3ld %3.3s %c %4ld%c   %3.3s %c %4ld%c                       %6ld %6ld";
		const char* FORMAT_TER     = "%5ld      %3.3s %c%4ld%c";
		const char* FORMAT_TITLE   = "  %2ld%60.60s";
		const char* FORMAT_TURN    = " %3ld %3.3s %3.3s %c%4ld%c %3.3s %c%4ld%c    %-30.30s";
		const char* FORMAT_TVECT   = " %3ld%10f%10f%10f%30.30s";

		const char* RECORD_TAG_ANISOU = "ANISOU";
		const char* RECORD_TAG_ATOM   = "ATOM  ";
		const char* RECORD_TAG_AUTHOR = "AUTHOR";
		const char* RECORD_TAG_CAVEAT = "CAVEAT";
		const char* RECORD_TAG_CISPEP = "CISPEP";
		const char* RECORD_TAG_COMPND = "COMPND";
		const char* RECORD_TAG_CONECT = "CONECT";
		const char* RECORD_TAG_CON061 = "CONECT";
		const char* RECORD_TAG_CON062 = "CONECT";
		const char* RECORD_TAG_CON063 = "CONECT";
		const char* RECORD_TAG_CON064 = "CONECT";
		const char* RECORD_TAG_CON06  = "CONECT";
		const char* RECORD_TAG_CRYST1 = "CRYST1";
		const char* RECORD_TAG_DBREF  = "DBREF ";
		const char* RECORD_TAG_END    = "END   ";
		const char* RECORD_TAG_ENDMDL = "ENDMDL";
		const char* RECORD_TAG_EXPDTA = "EXPDTA";
		const char* RECORD_TAG_FORMUL = "FORMUL";
		const char* RECORD_TAG_FTNOTE = "FTNOTE";
		const char* RECORD_TAG_HEADER = "HEADER";
		const char* RECORD_TAG_HELIX  = "HELIX ";
		const char* RECORD_TAG_HET    = "HET   ";
		const char* RECORD_TAG_HETATM = "HETATM";
		const char* RECORD_TAG_HETNAM = "HETNAM";
		const char* RECORD_TAG_HETSYN = "HETSYN";
		const char* RECORD_TAG_HYDBND = "HYDBND";
		const char* RECORD_TAG_JRNL   = "JRNL  ";
		const char* RECORD_TAG_KEYWDS = "KEYWDS";
		const char* RECORD_TAG_LINK   = "LINK  ";
		const char* RECORD_TAG_MASTER = "MASTER";
		const char* RECORD_TAG_MODEL  = "MODEL ";
		const char* RECORD_TAG_MODRES = "MODRES";
		const char* RECORD_TAG_MTRIX1 = "MTRIX1";
		const char* RECORD_TAG_MTRIX2 = "MTRIX2";
		const char* RECORD_TAG_MTRIX3 = "MTRIX3";
		const char* RECORD_TAG_OBSLTE = "OBSLTE";
		const char* RECORD_TAG_ORIGX1 = "ORIGX1";
		const char* RECORD_TAG_ORIGX2 = "ORIGX2";
		const char* RECORD_TAG_ORIGX3 = "ORIGX3";
		const char* RECORD_TAG_REMARK = "REMARK";
		const char* RECORD_TAG_REVDAT = "REVDAT";
		const char* RECORD_TAG_SCALE1 = "SCALE1";
		const char* RECORD_TAG_SCALE2 = "SCALE2";
		const char* RECORD_TAG_SCALE3 = "SCALE3";
		const char* RECORD_TAG_SEQADV = "SEQADV";
		const char* RECORD_TAG_SEQRES = "SEQRES";
		const char* RECORD_TAG_SHEET  = "SHEET ";
		const char* RECORD_TAG_SIGATM = "SIGATM";
		const char* RECORD_TAG_SIGUIJ = "SIGUIJ";
		const char* RECORD_TAG_SITE   = "SITE  ";
		const char* RECORD_TAG_SLTBRG = "SLTBRG";
		const char* RECORD_TAG_SOURCE = "SOURCE";
		const char* RECORD_TAG_SPRSDE = "SPRSDE";
		const char* RECORD_TAG_SSBOND = "SSBOND";
		const char* RECORD_TAG_TER    = "TER   ";
		const char* RECORD_TAG_TITLE  = "TITLE ";
		const char* RECORD_TAG_TURN   = "TURN  ";
		const char* RECORD_TAG_TVECT  = "TVECT ";

		const RecordTypeFormat RECORD_TYPE_FORMAT [] =
		{
			{ RECORD_TYPE__UNKNOWN,"      ", FORMAT_UNKNOWN },
			{ RECORD_TYPE__ANISOU, "ANISOU", FORMAT_ANISOU },
			{ RECORD_TYPE__ATOM,   "ATOM  ", FORMAT_ATOM   },
			{ RECORD_TYPE__AUTHOR, "AUTHOR", FORMAT_AUTHOR },
			{ RECORD_TYPE__CAVEAT, "CAVEAT", FORMAT_CAVEAT },
			{ RECORD_TYPE__CISPEP, "CISPEP", FORMAT_CISPEP },
			{ RECORD_TYPE__COMPND, "COMPND", FORMAT_COMPND },
			{ RECORD_TYPE__CONECT, "CONECT", FORMAT_CONECT },
			{ RECORD_TYPE__CON06,  "CONECT", FORMAT_CON06  },
			{ RECORD_TYPE__CON061, "CONECT", FORMAT_CON061 },
			{ RECORD_TYPE__CON062, "CONECT", FORMAT_CON062 },
			{ RECORD_TYPE__CON063, "CONECT", FORMAT_CON063 },
			{ RECORD_TYPE__CON064, "CONECT", FORMAT_CON064 },
			{ RECORD_TYPE__CRYST1, "CRYST1", FORMAT_CRYST1 },
			{ RECORD_TYPE__DBREF,  "DBREF ", FORMAT_DBREF  },
			{ RECORD_TYPE__END,    "END   ", FORMAT_END    },
			{ RECORD_TYPE__ENDMDL, "ENDMDL", FORMAT_ENDMDL },
			{ RECORD_TYPE__EXPDTA, "EXPDTA", FORMAT_EXPDTA },
			{ RECORD_TYPE__FORMUL, "FORMUL", FORMAT_FORMUL },
			{ RECORD_TYPE__FTNOTE, "FTNOTE", FORMAT_FTNOTE },
			{ RECORD_TYPE__HEADER, "HEADER", FORMAT_HEADER },
			{ RECORD_TYPE__HELIX,  "HELIX ", FORMAT_HELIX  },
			{ RECORD_TYPE__HET,    "HET   ", FORMAT_HET    },
			{ RECORD_TYPE__HETATM, "HETATM", FORMAT_HETATM },
			{ RECORD_TYPE__HETNAM, "HETNAM", FORMAT_HETNAM },
			{ RECORD_TYPE__HETSYN, "HETSYN", FORMAT_HETSYN },
			{ RECORD_TYPE__HYDBND, "HYDBND", FORMAT_HYDBND },
			{ RECORD_TYPE__JRNL,   "JRNL  ", FORMAT_JRNL   },
			{ RECORD_TYPE__KEYWDS, "KEYWDS", FORMAT_KEYWDS },
			{ RECORD_TYPE__LINK,   "LINK  ", FORMAT_LINK   },
			{ RECORD_TYPE__MASTER, "MASTER", FORMAT_MASTER },
			{ RECORD_TYPE__MODEL,  "MODEL ", FORMAT_MODEL  },
			{ RECORD_TYPE__MODRES, "MODRES", FORMAT_MODRES },
			{ RECORD_TYPE__MTRIX1, "MTRIX1", FORMAT_MTRIX1 },
			{ RECORD_TYPE__MTRIX2, "MTRIX2", FORMAT_MTRIX2 },
			{ RECORD_TYPE__MTRIX3, "MTRIX3", FORMAT_MTRIX3 },
			{ RECORD_TYPE__OBSLTE, "OBSLTE", FORMAT_OBSLTE },
			{ RECORD_TYPE__ORIGX1, "ORIGX1", FORMAT_ORIGX1 },
			{ RECORD_TYPE__ORIGX2, "ORIGX2", FORMAT_ORIGX2 },
			{ RECORD_TYPE__ORIGX3, "ORIGX3", FORMAT_ORIGX3 },
			{ RECORD_TYPE__REMARK, "REMARK", FORMAT_REMARK },
			{ RECORD_TYPE__REVDAT, "REVDAT", FORMAT_REVDAT },
			{ RECORD_TYPE__SCALE1, "SCALE1", FORMAT_SCALE1 },
			{ RECORD_TYPE__SCALE2, "SCALE2", FORMAT_SCALE2 },
			{ RECORD_TYPE__SCALE3, "SCALE3", FORMAT_SCALE3 },
			{ RECORD_TYPE__SEQADV, "SEQADV", FORMAT_SEQADV },
			{ RECORD_TYPE__SEQRES, "SEQRES", FORMAT_SEQRES },
			{ RECORD_TYPE__SHEET,  "SHEET ", FORMAT_SHEET },
			{ RECORD_TYPE__SIGATM, "SIGATM", FORMAT_SIGATM },
			{ RECORD_TYPE__SIGUIJ, "SIGUIJ", FORMAT_SIGUIJ },
			{ RECORD_TYPE__SITE,   "SITE  ", FORMAT_SITE },
			{ RECORD_TYPE__SLTBRG, "SLTBRG", FORMAT_SLTBRG },
			{ RECORD_TYPE__SOURCE, "SOURCE", FORMAT_SOURCE },
			{ RECORD_TYPE__SPRSDE, "SPRSDE", FORMAT_SPRSDE },
			{ RECORD_TYPE__SSBOND, "SSBOND", FORMAT_SSBOND },
			{ RECORD_TYPE__TER,    "TER   ", FORMAT_TER },
			{ RECORD_TYPE__TITLE,  "TITLE ", FORMAT_TITLE },
			{ RECORD_TYPE__TURN,   "TURN  ", FORMAT_TURN },
			{ RECORD_TYPE__TVECT,  "TVECT ", FORMAT_TVECT },
		};

	} // namespace PDB

} // namespace BALL

