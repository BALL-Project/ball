	/* Summary: Algorithm to do fast hashing of a pointer
	 * The result of the hash function is a number in the range 
	 * [0..(number_of_slots-1)].
	 */
	HashIndex Hashing::hashPointer(void *const void_ptr)
	{
		double d = ((double)((unsigned long)void_ptr)) * 0.6180339887;
		Index index = (Index)(5832641097.37287 * (d - (double)((unsigned long)d)));

		return ((index < 0) ? -index : index);
	}

	/* Summary: Algorithm to do fast hashing of variable length text     
	 * strings. The result of the hash function is a number in the range 
	 * [0..255]. This algorithm was published in CACM, 6/90. 
	 */
	HashIndex Hashing::hashString(register const char *s)
	{
		static const unsigned char pseudo_random_permuted_key[256] = 
		{ 
			1  ,87 ,49 ,12 ,176,178,102,166,121,193,6  ,84 ,249,230,44 ,163,
			14 ,197,213,181,161,85 ,218,80 ,64 ,239,24 ,226,236,142,38 ,200,
			110,177,104,103,141,253,255,50 ,77 ,101,81 ,18 ,45 ,96 ,31 ,222,
			25 ,107,190,70 ,86 ,237,240,34 ,72 ,242,20 ,214,244,227,149,235,
			97 ,234,57 ,22 ,60 ,250,82 ,175,208,5  ,127,199,111,62 ,135,248,
			174,169,211,58 ,66 ,154,106,195,245,171,17 ,187,182,179,0  ,243,
			132,56 ,148,75 ,128,133,158,100,130,126,91 ,13 ,153,246,216,219,
			119,68 ,223,78 ,83 ,88 ,201,99 ,122,11 ,92 ,32 ,136,114,52 ,10 ,
			138,30 ,48 ,183,156,35 ,61 ,26 ,143,74 ,251,94 ,129,162,63 ,152,
			170,7  ,115,167,241,206,3  ,150,55 ,59 ,151,220,90 ,53 ,23 ,131,
			125,173,15 ,238,79 ,95 ,89 ,16 ,105,137,225,224,217,160,37 ,123,
			118,73 ,2  ,157,46 ,116,9  ,145,134,228,207,212,202,215,69 ,229,
			27 ,188,67 ,124,168,252,42 ,4  ,29 ,108,21 ,247,19 ,205,39 ,203,
			233,40 ,186,147,198,192,155,33 ,164,191,98 ,204,165,180,117,76 ,
			140,36 ,210,172,41 ,54 ,159,8  ,185,232,113,196,231,47 ,146,120,
			51 ,65 ,28 ,144,254,221,93 ,189,194,139,112,43 ,71 ,109,184,209 
		};

		register Index index = 0;

		for(;	*s != '\0'; )
			index = pseudo_random_permuted_key[index ^ (*s++)];

		return index;
	}

	/* Summary: A portable one word summary of a string.
	 * Taken from: Tcl, DOC++ (McHashTable.cpp)
	 */
	HashIndex Hashing::hashString2(register const char *s)
	{
		register Index index = 0;

		for (register char c = *s++; c != '\0'; index += (index << 3) + c);
		
		return index;
	}

	/* Summary: A portable one word summary of a string.
	 * Taken from: DOC++ (nametable.cpp)
	 */
	HashIndex Hashing::hashString3(register const char *s)
	{
		register Index index = 0;

		while (*s != '\0')
		{
			index *= 'A';
			index += (*s++ - '0');
		}
		
		return index;
	}

	/* Summary: A portable adaptation of Peter Weinberger's (PJW) (AT&T Bell Labs) 
	 * generic hashing algorithm based on Allen Holub's version. Accepts a pointer 
	 * to a string to be hashed.
	 * Taken from: Dr. Dobb's Journal, April 1996, p.26
	 */
	HashIndex Hashing::hashPJWString(register const char *s)
	{
		register Index index = 0;
		register Index temp_index;

#		define BALL_BITS_IN_HASHVALUE_   (sizeof(Index) * CHAR_BIT)
#		define BALL_THREE_QUARTERS_      ((Index)((BALL_BITS_IN_HASHVALUE_ * 3) / 4))
#		define BALL_ONE_EIGHTH_          ((Index)(BALL_BITS_IN_HASHVALUE_ / 8))
#		define BALL_HIGH_BITS_           (~((Index)(~0) >> BALL_ONE_EIGHTH_))

		for (; *s; s++)
		{
			index = (index << BALL_ONE_EIGHTH_) + *s;
			if ((temp_index = index & BALL_HIGH_BITS_) != 0)
			{
				index = (index ^ (temp_index >> BALL_THREE_QUARTERS_)) & ~BALL_HIGH_BITS_;
			}
		}

#		undef BALL_BITS_IN_HASHVALUE_
#		undef BALL_THREE_QUARTERS_   
#		undef BALL_ONE_EIGHTH_       
#		undef BALL_HIGH_BITS_

		return index;
	}

	/* Summary: The published hash algorithm used in the UNIX ELF format for
	 * object files. Accepts a pointer to a string to be hashed.
	 * Assumes a long pointer to have 4 bytes of 8 bits.
	 * Taken from: Dr. Dobb's Journal, April 1996, p.26
	 */
	HashIndex Hashing::hashElfString(register const char *s)
	{
		register unsigned long l = 0;
		register unsigned long temp;

		while(*s)
		{
			l = (l << 4) + *s++;
			if ((temp = l & 0xF0000000L))
			{
				l ^= temp >> 24;
			}
			l &= ~temp;
		}

		return (Index)l;
	}
