// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_RENDERING_PIXELFORMAT_H
#define BALL_VIEW_RENDERING_PIXELFORMAT_H

#include <BALL/COMMON/global.h>

#include <iostream>

namespace BALL 
{
	
	namespace VIEW
	{
    
		struct ChannelFormat {

			enum ChannelID {
				NO_CHANNEL = 0, // no channel info
				EMPTY_CHANNEL,  // used for specifying empty space in a pixel
				RED_CHANNEL,
				GREEN_CHANNEL,
				BLUE_CHANNEL,
				LUMINANCE_CHANNEL,
				ALPHA_CHANNEL,
				DEPTH_CHANNEL,
				Y_CHANNEL,
				U_CHANNEL,
				V_CHANNEL
			};

			enum ChannelType {
				SIGNED_INT_CHANNEL,
				UNSIGNED_INT_CHANNEL,
				FLOAT_CHANNEL
			};

			ChannelID id          : 8;
			ChannelType type      : 8;
			unsigned int bitSize  : 8;

			ChannelFormat() : id(NO_CHANNEL) { }

			ChannelFormat(ChannelID id, ChannelType type, unsigned int bitSize) :
				id(id), type(type), bitSize(bitSize)
			{ }

			bool operator==(const ChannelFormat &f) const {
				return id == f.id && type == f.type && bitSize == f.bitSize;
			}

			bool operator!=(const ChannelFormat &f) const {
				return id != f.id || type != f.type || bitSize != f.bitSize;
			}

		};


		class PixelFormat {
		public:
	        
			enum { MAX_NUMBER_OF_CHANNELS = 4 };
	        
			// Flags
			enum {
				// framebuffer data structure
				PLANAR_FRAMEBUFFER                  = 1<<1 // otherwise interlaced
			};


			PixelFormat() : numChannels(0), flags(0) { }

	#define _RTSG_FB_SETCH(i,v)                     \
			this->channels[(i)] = v;                \
			if (v.id != ChannelFormat::NO_CHANNEL)  \
				++numChannels


			PixelFormat(const ChannelFormat &c0,
						unsigned int flags = 0) :
				numChannels(0), flags(flags)
			{
				_RTSG_FB_SETCH(0,c0);
			}

			PixelFormat(const ChannelFormat &c0,
						const ChannelFormat &c1,
						unsigned int flags = 0) :
				numChannels(0), flags(flags)
			{
				_RTSG_FB_SETCH(0,c0);
				_RTSG_FB_SETCH(1,c1);
			}

			PixelFormat(const ChannelFormat &c0,
						const ChannelFormat &c1,
						const ChannelFormat &c2,
						unsigned int flags = 0) :
				numChannels(0), flags(flags)
			{
				_RTSG_FB_SETCH(0,c0);
				_RTSG_FB_SETCH(1,c1);
				_RTSG_FB_SETCH(2,c2);
			}

			PixelFormat(const ChannelFormat &c0,
						const ChannelFormat &c1,
						const ChannelFormat &c2,
						const ChannelFormat &c3,
						unsigned int flags = 0) :
				numChannels(0), flags(flags)
			{
				_RTSG_FB_SETCH(0,c0);
				_RTSG_FB_SETCH(1,c1);
				_RTSG_FB_SETCH(2,c2);
				_RTSG_FB_SETCH(3,c3);
			}
	        
	#undef _RTSG_FB_SETCH

			unsigned int getNumChannels() const { return numChannels; }

			const ChannelFormat &getChannel(unsigned int i) const
			{ 
				return channels[i];
			}

			ChannelFormat &getChannel(unsigned int i)
			{ 
				return channels[i];
			}
	        
			const ChannelFormat &operator[](unsigned int i) const 
			{
				return channels[i];
			}

			ChannelFormat &operator[](unsigned int i)  
			{
				return channels[i];
			}

			unsigned int getFlags() const { return flags; }

			void setFlags(unsigned int f) { flags = f; }

			unsigned int computeBitSize() const {
				unsigned int bitSize = 0;
				for (unsigned int i = 0; i < numChannels; i++) {
					bitSize += channels[i].bitSize;
				}
				return bitSize;
			}

			unsigned int computeByteSize() const {
				unsigned int bitSize = computeBitSize();
				return (bitSize / 8) + ((bitSize % 8) > 0 ? 1 : 0) ;
			}
	        
			PixelFormat &operator=(const PixelFormat &f)
			{
				numChannels = f.numChannels;
				flags = f.flags;
				for (unsigned int i = 0; i < numChannels; i++) {
					channels[i] = f.channels[i];
				}
				return *this;
			}

			bool operator==(const PixelFormat &f) const
			{
				if (numChannels != f.numChannels || flags != f.flags)
					return false;
				for (unsigned int i = 0; i < numChannels; i++) {
					if (channels[i] != f.channels[i])
						return false;
				}
				return true;
			}

			bool operator!=(const PixelFormat &f) const {
				return !(*this == f);
			}

			void print(std::ostream &o) const;

			static const PixelFormat RGB_24;
			static const PixelFormat BGR_24;

			static const PixelFormat RGB_32;
			static const PixelFormat RGBA_32;
			static const PixelFormat BGR_32;
			static const PixelFormat BGRA_32;


			static const PixelFormat RGB_3_2_2;


			static const PixelFormat RGBF_96;

		private:
			ChannelFormat channels[MAX_NUMBER_OF_CHANNELS];
			unsigned int numChannels;
			unsigned int flags;
		};

		inline std::ostream &operator<<(std::ostream &o, const PixelFormat &f)
		{
			f.print(o);
			return o;
		}

	} // namespace VIEW
    
} // namespace BALL

#endif //BALL_VIEW_RENDERING_PIXELFORMAT_H
