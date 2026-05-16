// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: hashGrid.C,v 1.2 2002/02/27 12:21:11 sturm Exp $

#include <BALL/DATATYPE/hashGrid.h>

namespace BALL
{
	namespace __private
	{
		// `signed char` — see hashGrid.h for portability rationale (ARM gcc
		// defaults `char` to unsigned, breaks -Wnarrowing on -1 literals).
		const signed char neighbour_table_[27][3] =
		{
			{ 0,  0,  0 }, { 0,  0, -1 }, { 0,  0,  1 },
			{ 0, -1, -1 }, { 0, -1,  0 }, { 0, -1,  1 },
			{ 0,  1, -1 }, { 0,  1,  0 }, { 0,  1,  1 },
			{-1,  0, -1 }, {-1,  0,  0 }, {-1,  0,  1 },
			{-1, -1, -1 }, {-1, -1,  0 }, {-1, -1,  1 },
			{-1,  1, -1 }, {-1,  1,  0 }, {-1,  1,  1 },
			{ 1,  0, -1 }, { 1,  0,  0 }, { 1,  0,  1 },
			{ 1, -1, -1 }, { 1, -1,  0 }, { 1, -1,  1 },
			{ 1,  1, -1 }, { 1,  1,  0 }, { 1,  1,  1 }
		};
	}
}
