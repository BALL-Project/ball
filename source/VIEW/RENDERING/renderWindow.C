// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/COMMON/exception.h>
#include <BALL/COMMON/logStream.h>
#include <BALL/DATATYPE/string.h>

#include <BALL/VIEW/RENDERING/renderWindow.h>

namespace BALL
{
	namespace VIEW
	{
		template<typename taPixelDatatype>
			TRenderWindow<taPixelDatatype>::TRenderWindow()
			:m_pfm((BALLVIEW_IS_SAME_TYPE(taPixelDatatype, char) ? PixelFormat::RGBA_32 : PixelFormat::RGBF_96)),
			m_minimalWidth(2),
			m_minimalHeight(2),
			do_not_resize_(false)
			{
				//
			}

		template<typename taPixelDatatype>
			TRenderWindow<taPixelDatatype>::~TRenderWindow()                
			{
				//
			}

		template<typename taPixelDatatype>
			FrameBufferPtr TRenderWindow<taPixelDatatype>::getBuffer() throw(BALL::Exception::NoBufferAvailable)        
			{
				if((m_fmt.getWidth() < m_minimalWidth) || (m_fmt.getHeight() < m_minimalHeight))
				{
					BALL::Log.error() << "Raytracing window was not properly resized before getBuffer() was called" << std::endl;
					throw BALL::Exception::NoBufferAvailable(__FILE__, __LINE__, BALL::String("Window was not properly resized"));
				}

				// Put boost::lock here !!!
				if(m_bufferLocked)
				{
					throw BALL::Exception::NoBufferAvailable(__FILE__, __LINE__, String("Buffer is being accessed by another object"));                
				}
				else
				{
					m_bufferLocked = true;
					m_framebuffer = FrameBufferPtr(new FrameBuffer(m_pixels.get(), m_fmt));                
					return m_framebuffer;                
				}
			}

		template<typename taPixelDatatype>
			FrameBufferFormat TRenderWindow<taPixelDatatype>::getFormat() const
			{
				return m_fmt;
			}

		template<typename taPixelDatatype>
			void TRenderWindow<taPixelDatatype>::releaseBuffer(FrameBufferPtr /*buffer*/)
			{
				m_bufferLocked = false;
			}		

		template<typename taPixelDatatype>
			bool TRenderWindow<taPixelDatatype>::init()
			{				
				m_fmt = FrameBufferFormat(0, 0, m_pfm);
				m_bufferLocked = false;
				return true;
			}

		template<typename taPixelDatatype>
			bool TRenderWindow<taPixelDatatype>::resize(const unsigned int width, const unsigned int height)
			{
				if((width < m_minimalWidth) || (height < m_minimalHeight))
				{
					return false;
				}

				m_fmt.setWidth(width);
				m_fmt.setPitch(width * m_pfm.computeByteSize());			
				m_fmt.setHeight(height);

				m_pixels = t_PixelPtr(new taPixelDatatype[m_fmt.getWidth() * m_fmt.getHeight() * m_pfm.getNumChannels()]);
				memset(m_pixels.get(), '\0', m_fmt.getWidth() * m_fmt.getHeight() * m_pfm.computeByteSize());

				return true;
			}

		template<typename taPixelDatatype>
			void TRenderWindow<taPixelDatatype>::refresh()
			{
				if(m_bufferLocked)
				{
					return;
				}
			}


		// Explicit specializations to be able to have window defined in .cpp file
		template class TRenderWindow<char>;
		template class TRenderWindow<float>;

	} // namespace VIEW

} // namespace BALL

