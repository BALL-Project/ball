// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/RENDERING/pixelFormat.h>

namespace BALL 
{

	namespace VIEW 
	{

        const PixelFormat PixelFormat::RGB_24(
            ChannelFormat(ChannelFormat::RED_CHANNEL,
                          ChannelFormat::UNSIGNED_INT_CHANNEL, 8),
            ChannelFormat(ChannelFormat::GREEN_CHANNEL, 
                          ChannelFormat::UNSIGNED_INT_CHANNEL, 8),
            ChannelFormat(ChannelFormat::BLUE_CHANNEL, 
                          ChannelFormat::UNSIGNED_INT_CHANNEL, 8));

        const PixelFormat PixelFormat::BGR_24(
            ChannelFormat(ChannelFormat::BLUE_CHANNEL,
                          ChannelFormat::UNSIGNED_INT_CHANNEL, 8),
            ChannelFormat(ChannelFormat::GREEN_CHANNEL, 
                          ChannelFormat::UNSIGNED_INT_CHANNEL, 8),
            ChannelFormat(ChannelFormat::RED_CHANNEL, 
                          ChannelFormat::UNSIGNED_INT_CHANNEL, 8));


        const PixelFormat PixelFormat::RGB_32(
            ChannelFormat(ChannelFormat::RED_CHANNEL, 
                          ChannelFormat::UNSIGNED_INT_CHANNEL, 8),
            ChannelFormat(ChannelFormat::GREEN_CHANNEL, 
                          ChannelFormat::UNSIGNED_INT_CHANNEL, 8),
            ChannelFormat(ChannelFormat::BLUE_CHANNEL, 
                          ChannelFormat::UNSIGNED_INT_CHANNEL, 8),
            ChannelFormat(ChannelFormat::EMPTY_CHANNEL, 
                          ChannelFormat::UNSIGNED_INT_CHANNEL, 8));

        const PixelFormat PixelFormat::RGBA_32(
            ChannelFormat(ChannelFormat::RED_CHANNEL, 
                          ChannelFormat::UNSIGNED_INT_CHANNEL, 8),
            ChannelFormat(ChannelFormat::GREEN_CHANNEL, 
                          ChannelFormat::UNSIGNED_INT_CHANNEL, 8),
            ChannelFormat(ChannelFormat::BLUE_CHANNEL, 
                          ChannelFormat::UNSIGNED_INT_CHANNEL, 8),
            ChannelFormat(ChannelFormat::ALPHA_CHANNEL, 
                          ChannelFormat::UNSIGNED_INT_CHANNEL, 8));

        const PixelFormat PixelFormat::BGR_32(
            ChannelFormat(ChannelFormat::BLUE_CHANNEL, 
                          ChannelFormat::UNSIGNED_INT_CHANNEL, 8),
            ChannelFormat(ChannelFormat::GREEN_CHANNEL, 
                          ChannelFormat::UNSIGNED_INT_CHANNEL, 8),
            ChannelFormat(ChannelFormat::RED_CHANNEL, 
                          ChannelFormat::UNSIGNED_INT_CHANNEL, 8),
            ChannelFormat(ChannelFormat::EMPTY_CHANNEL, 
                          ChannelFormat::UNSIGNED_INT_CHANNEL, 8));

        const PixelFormat PixelFormat::BGRA_32(
            ChannelFormat(ChannelFormat::BLUE_CHANNEL, 
                          ChannelFormat::UNSIGNED_INT_CHANNEL, 8),
            ChannelFormat(ChannelFormat::GREEN_CHANNEL, 
                          ChannelFormat::UNSIGNED_INT_CHANNEL, 8),
            ChannelFormat(ChannelFormat::RED_CHANNEL, 
                          ChannelFormat::UNSIGNED_INT_CHANNEL, 8),
            ChannelFormat(ChannelFormat::ALPHA_CHANNEL, 
                          ChannelFormat::UNSIGNED_INT_CHANNEL, 8));


        const PixelFormat PixelFormat::RGB_3_2_2(
            ChannelFormat(ChannelFormat::RED_CHANNEL,
                          ChannelFormat::UNSIGNED_INT_CHANNEL, 3),
            ChannelFormat(ChannelFormat::GREEN_CHANNEL, 
                          ChannelFormat::UNSIGNED_INT_CHANNEL, 2),
            ChannelFormat(ChannelFormat::BLUE_CHANNEL, 
                          ChannelFormat::UNSIGNED_INT_CHANNEL, 2));



        const PixelFormat PixelFormat::RGBF_96(
            ChannelFormat(ChannelFormat::RED_CHANNEL,
                          ChannelFormat::FLOAT_CHANNEL, sizeof(float)*8),
            ChannelFormat(ChannelFormat::GREEN_CHANNEL, 
                          ChannelFormat::FLOAT_CHANNEL, sizeof(float)*8),
            ChannelFormat(ChannelFormat::BLUE_CHANNEL, 
                          ChannelFormat::FLOAT_CHANNEL, sizeof(float)*8));


        void PixelFormat::print(std::ostream &o) const
        {
            o<<'[';
            const unsigned int nc = getNumChannels();

            // output channel IDs
            if (nc == 1) {
                switch (channels[0].id) {
                    case ChannelFormat::NO_CHANNEL: break;
                    case ChannelFormat::EMPTY_CHANNEL:     o << "EMPTY"; break;
                    case ChannelFormat::RED_CHANNEL:       o << "RED"; break;
                    case ChannelFormat::GREEN_CHANNEL:     o << "GREEN"; break;
                    case ChannelFormat::BLUE_CHANNEL:      o << "BLUE"; break;
                    case ChannelFormat::LUMINANCE_CHANNEL: o << "LUMINANCE"; break;
                    case ChannelFormat::ALPHA_CHANNEL:     o << "ALPHA"; break;
                    case ChannelFormat::DEPTH_CHANNEL:     o << "DEPTH"; break;
                    case ChannelFormat::Y_CHANNEL:         o << "Y"; break;
                    case ChannelFormat::U_CHANNEL:         o << "U"; break;
                    case ChannelFormat::V_CHANNEL:         o << "V"; break;
                }
            }
            else {
                for (unsigned int i = 0; i < nc; i++) {
                    const ChannelFormat &cf = getChannel(i);
                    switch (cf.id) {
                        case ChannelFormat::NO_CHANNEL: break;
                        case ChannelFormat::EMPTY_CHANNEL:     o << '-'; break;
                        case ChannelFormat::RED_CHANNEL:       o << 'R'; break;
                        case ChannelFormat::GREEN_CHANNEL:     o << 'G'; break;
                        case ChannelFormat::BLUE_CHANNEL:      o << 'B'; break;
                        case ChannelFormat::LUMINANCE_CHANNEL: o << 'L'; break;
                        case ChannelFormat::ALPHA_CHANNEL:     o << 'A'; break;
                        case ChannelFormat::DEPTH_CHANNEL:     o << 'D'; break;
                        case ChannelFormat::Y_CHANNEL:         o << 'Y'; break;
                        case ChannelFormat::U_CHANNEL:         o << 'U'; break;
                        case ChannelFormat::V_CHANNEL:         o << 'V'; break;
                    }
                }
            }

            o<<" ";

            // output bit-sizes and type
            // i - signed int
            // u - unsigned int
            // f - float
            for (unsigned int i = 0; i < nc; i++) {
                const ChannelFormat &cf = channels[i];
                o << cf.bitSize;
                switch (cf.type) {
                    case ChannelFormat::SIGNED_INT_CHANNEL: o << 'i'; break;
                    case ChannelFormat::UNSIGNED_INT_CHANNEL: o << 'u'; break;
                    case ChannelFormat::FLOAT_CHANNEL: o << 'f'; break;
                }
                if (i != nc-1) // if not last, write separator
                    o<<'-';
            }

            o<<' ';
            
            o<<computeBitSize()<<"bit(s) "
             <<computeByteSize()<<"byte(s)";

            o<<']';

        }

    } //namespace VIEW

} // namespace BALL
