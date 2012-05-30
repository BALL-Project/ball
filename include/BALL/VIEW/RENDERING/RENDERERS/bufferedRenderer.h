// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_RENDERING_RENDERERS_BUFFEREDRENDERER_H
#define BALL_VIEW_RENDERING_RENDERERS_BUFFEREDRENDERER_H

#ifndef BALL_VIEW_RENDERING_RENDERERS_RENDERER_H
# include <BALL/VIEW/RENDERING/RENDERERS/renderer.h>
#endif

#ifndef BALL_VIEW_RENDERING_RENDERTARGET_H
# include <BALL/VIEW/RENDERING/renderTarget.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/** Interface for Buffered Rendering.
		 *  BufferedRenderers don't create their own window. Instead, they render the
		 *  scene to a FrameBuffer which is provided by a RenderTarget.
		 *  The FrameBuffer contains a pointer to a memory area where the results will
		 *  be stored, as well as the pixel format and the resolution.
		 *  Before rendering can start, you have to choose an output format using the
		 *  setFrameBufferFormat() function. After that, you can call renderToBuffer(),
		 *  as long as the format of the RenderTarget is the same that you set before.
		 */
		class BALL_VIEW_EXPORT BufferedRenderer
			: public Renderer
		{
			public:

				BufferedRenderer()
					: offset_(0),
						stride_(0)
				{ }

				/** Tries to choose a format for buffered rendering.
				 *  @return true if the format could be set,
				 *  @return false if it is not supported.
				 */
				bool setFrameBufferFormat(const FrameBufferFormat &format)
				{
					if (supports(format))
					{
						bufferFormat = format;
						formatUpdated();					

						return true;
					}

					return false;
				}

				/** Render the current frame to the target's buffer using the \link Stage \endlink setup.
				 *  Throws FrameBufferFormatException if the RenderTarget's format is not
				 *  the one that you set before with setFrameBufferFormat().
				 *  @throw Exception::InvalidFormat if the FrameBufferFormat is not supported by the renderer
				 *  @throw Exception::NoBufferAvailable if the frame buffer was not correctly initialized
				 */
				void renderToBuffer(RenderTarget* renderTarget, const Stage& stage)
				{
					prepareBufferedRendering(stage);

					FrameBufferPtr buffer = renderTarget->getBuffer();
					assert(buffer);

					if (buffer->getFormat() != bufferFormat)
					{
						throw Exception::InvalidFormat(__FILE__, __LINE__, "Buffer with wrong framebuffer format supplied");
					}

					renderToBufferImpl( buffer );

					renderTarget->releaseBuffer(buffer);
				}

				/** Checks if a particular PixelFormat is supported by the renderer. */
				virtual bool supports(const PixelFormat &format) const = 0;

				/** Returns a supported resolution within the given limits.
				 *  The x resolution will be between min.x and max.x and the y resolution
				 *  will be between min.y and max.y.
				 *  The returned resolution must be supported by the renderer with the
				 *  requested PixelFormat.
				 *  @throw Exception::FormatUnsupported if the FrameBufferFormat is not supported by the renderer
				 */
				virtual Resolution getSupportedResolution(const Resolution &min, const Resolution &max,
				                                          const PixelFormat &format) const = 0;

				/** Set an offset into the data buffer 
				 */
				virtual void setOffset(Size offset) {offset_ = offset;}

				/** Set a stride for the data buffer
				 */
				virtual void setStride(Size stride) {stride_ = stride;}

			protected:

				/** Checks if a particular FrameBufferFormat is supported by the renderer.
				 *  This checks the combination of pixel format and resolution.
				 *  If this returns true, the renderer is able to write to a buffer in
				 *  this format.
				 */
				virtual bool supports(const FrameBufferFormat &format) const = 0;

				/** This is called once the FrameBufferFormat has been set so the renderer
				 *  can perform any initialization steps that are needed.
				 */
				virtual void formatUpdated() = 0;

				/** Prepare for the rendering, taking into account settings in Stage, eg. Camera, Lights, Background colors, etc.	
				 */
				virtual void prepareBufferedRendering(const Stage& stage) = 0;

				/** Render to the frame buffer.
				 *  @pre The frame buffer's format is the one stored in bufferFormat and
				 *       prepareBufferedRendering() has been called.
				 */
				virtual void renderToBufferImpl(FrameBufferPtr buffer) = 0;            

				/** Returns the current FrameBufferFormat. */
				const FrameBufferFormat &getFrameBufferFormat() const { return bufferFormat; }

				Size offset_;
				Size stride_;

			private:

				FrameBufferFormat bufferFormat;
		};
	} //namespace VIEW

} //namespace BALL


#endif // BALL_VIEW_RENDERING_BUFFEREDRENDERER_H
