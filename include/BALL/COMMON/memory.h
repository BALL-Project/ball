// $Id: memory.h,v 1.2 2000/01/10 15:50:54 oliver Exp $

#ifndef BALL_COMMON_MEMORY_H
#define BALL_COMMON_MEMORY_H

#ifndef BALL_CONFIG_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif

#include <stdio.h>
#include <stdlib.h>

namespace BALL 
{

	/**	Memory manager
	*/
	class MemoryLogManager
	{
			friend void* operator new (size_t bytes);
			friend void* operator new[] (size_t bytes);
			friend void  operator delete (void* ptr);
			friend void  operator delete[] (void* ptr);

		public:

			typedef unsigned long AllocHandle;
			typedef unsigned long AllocBytes;
			typedef unsigned long Sentinel;
			typedef unsigned char HeaderSize;
			typedef unsigned char TailSize;
			typedef long AllocMemorySize;
			typedef long AllocMemoryOverhead;

			struct AllocBlockPrefix
			{
				AllocHandle alloc_handle;
				AllocBytes 	alloc_bytes;
				Sentinel 		sentinel;
			};

			struct AllocBlockSuffix
			{
				Sentinel sentinel;
			};

			MemoryLogManager(const char* filename);

			~MemoryLogManager();

			void enable();

			void disable();

			bool isEnabled() const;

			bool isDisabled() const;

			static bool isBlockValid(void* ptr);

			FILE* getFILE()
			{
				return log_;
			}

		protected:

		private:

			FILE* log_;
			AllocHandle 						alloc_handle_;
			static const Sentinel 	prefix_sentinel_;
			static const Sentinel 	suffix_sentinel_;
			static const HeaderSize header_size_;
			static const TailSize		tail_size_;
			AllocMemorySize 				alloc_memory_size_;
			AllocMemoryOverhead 		alloc_memory_overhead_;
			AllocMemorySize 				max_alloc_size_;
			AllocMemoryOverhead 		max_alloc_overhead_;
			bool										memory_logging_enabled_;
	};

}; // namespace BALL

#endif // BALL_COMMON_MEMORY_H
