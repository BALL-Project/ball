// $Id: glObjectCollector.h,v 1.1 2000/09/23 12:49:34 hekl Exp $

#ifndef BALL_VIEW_OPENGL_FUNCTOR_GLOBJECTCOLLECTOR_H
#define BALL_VIEW_OPENGL_FUNCTOR_GLOBJECTCOLLECTOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_OPENGL_KERNEL_GLOBJECT_H
#	include <BALL/VIEW/OPENGL/KERNEL/glObject.h>
#endif


namespace BALL
{
	
	namespace VIEW
	{

		class GLObjectCollector
			: public UnaryProcessor<Composite>
		{
			public:

			// --- CONSTRUCTORS

			GLObjectCollector();

			GLObjectCollector(const GLObjectCollector& GL_object_collector, bool deep = true);

			virtual ~GLObjectCollector();

			virtual void clear();

			virtual void destroy();

			// --- ASSIGNMENT METHODS

			void set(const GLObjectCollector& GL_object_collector, bool deep = true);

			GLObjectCollector& operator = (const GLObjectCollector& GL_object_collector);

			void get(GLObjectCollector& GL_object_collector, bool deep = true) const;

			void swap(GLObjectCollector& GL_object_collector);

			// --- ACCESSORS: INSPECTORS and MUTATORS

			void appendToStaticList(GLObject* GL_object);
			list<GLObject *>& getStaticList();
			const list<GLObject *>& getStaticList() const;

			void appendToStaticAlwaysFrontList(GLObject* GL_object);
			list<GLObject *>& getStaticAlwaysFrontList();
			const list<GLObject *>& getStaticAlwaysFrontList() const;

			void appendToStaticWireframeList(GLObject* GL_object);
			list<GLObject *>& getStaticWireframeList();
			const list<GLObject *>& getStaticWireframeList() const;

			void appendToStaticWireframeAlwaysFrontList(GLObject* GL_object);
			list<GLObject *>& getStaticWireframeAlwaysFrontList();
			const list<GLObject *>& getStaticWireframeAlwaysFrontList() const;

			void appendToDynamicList(GLObject* GL_object);
			list<GLObject *>& getDynamicList();
			const list<GLObject *>& getDynamicList() const;

			void appendToDynamicAlwaysFrontList(GLObject* GL_object);
			list<GLObject *>& getDynamicAlwaysFrontList();
			const list<GLObject *>& getDynamicAlwaysFrontList() const;

			void appendToTransparentList(GLObject* GL_object);
			list<GLObject *>& getTransparentList();
			const list<GLObject *>& getTransparentList() const;

			void appendToTransparentAlwaysFrontList(GLObject* GL_object);
			list<GLObject *>& getTransparentAlwaysFrontList();
			const list<GLObject *>& getTransparentAlwaysFrontList() const;

			void setRootComposite(const Composite* composite);

			const Composite* getRootComposite() const;

			virtual bool start();
			
			virtual bool finish();
			
			// --- OPERATORS

			virtual Processor::Result operator()
				(Composite& composite);

			// --- DEBUGGERS and DIAGNOSTICS

			virtual bool isValid() const;

			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const;

			// --- STORERS

			virtual void read(std::istream& s);

			virtual void write(std::ostream& s) const;

			
			private:

			Composite* composite_;

			list<GLObject *> static_list_;
			list<GLObject *> static_always_front_list_;
			list<GLObject *> static_wireframe_list_;
			list<GLObject *> static_wireframe_always_front_list_;

			list<GLObject *> dynamic_list_;
			list<GLObject *> dynamic_always_front_list_;

			list<GLObject *> transparent_list_;
			list<GLObject *> transparent_always_front_list_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/OPENGL/FUNCTOR/glObjectCollector.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_OPENGL_FUNCTOR_GLOBJECTCOLLECTOR_H 
