// $I: compositeDescriptor.C,v 1.3 2001/02/04 16:14:26 hekl Exp $

#include <BALL/VIEW/GUI/KERNEL/compositeDescriptor.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		CompositeDescriptor::CompositeDescriptor()
			throw()
			:	name_("unkown"),
				name_ptr_(&name_),
				center_(0.0, 0.0, 0.0),
				center_ptr_(&center_),
				quaternion_(),
				quaternion_ptr_(&quaternion_),
				composite_(0),
				entities_(),
				object_collector_(0),
				primitive_manager_(0),
				shallow_copy_(true),
				shallow_copies_(),
				parent_(0)
		{
		}
			
		CompositeDescriptor::CompositeDescriptor
			(const CompositeDescriptor& composite_descriptor, bool deep)
			throw()
			:	name_(composite_descriptor.name_),
				center_(composite_descriptor.center_),
				quaternion_(composite_descriptor.quaternion_),
				entities_(),
				object_collector_(0),
				primitive_manager_(0),
				shallow_copy_(!deep),
				shallow_copies_()
		{
			if (deep == true)
			{
				name_ptr_ = &name_;
				center_ptr_ = &center_;
				quaternion_ptr_ = &quaternion_;

				composite_ = (Composite *)composite_descriptor.composite_->create();

				parent_ = 0;
			}
			else
			{
				name_ptr_ = (String *)&(composite_descriptor.name_);
				center_ptr_ = (Vector3 *)&(composite_descriptor.center_);
				quaternion_ptr_ = (Quaternion *)&(composite_descriptor.quaternion_);
				composite_ = composite_descriptor.composite_;

				((CompositeDescriptor *)&composite_descriptor)
					->addShallowCompositeDescriptor_(this);

				parent_ = (CompositeDescriptor *)&composite_descriptor;
			}
		}

		CompositeDescriptor::~CompositeDescriptor()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this << " of class " 
					<< RTTI::getName<CompositeDescriptor>() << endl;
			#endif 

			destroy();
		}

		void CompositeDescriptor::clear()
			throw()
		{
			// clear all entities
			HashMap<GLPrimitiveManager *, GLEntityDescriptor *>::Iterator entity_iterator;
			for (entity_iterator = entities_.begin();
					 entity_iterator != entities_.end();
					 ++entity_iterator)
			{
				(entity_iterator->second)->clear();
			}

			// reset internal values
			object_collector_ = 0;
			primitive_manager_ = 0;

			/*
			name_ptr_ = &name_;
			center_ptr_ = &center_;
			quaternion_ptr_ = &quaternion_;
			*/
		}
			
		void CompositeDescriptor::destroy()
			throw()
		{
			clear();

			// destroys all entities
			HashMap<GLPrimitiveManager *, GLEntityDescriptor *>::Iterator entity_iterator;
			for (entity_iterator = entities_.begin();
					 entity_iterator != entities_.end();
					 ++entity_iterator)
			{
				delete (entity_iterator->second);
			}

			entities_.clear();

			list<CompositeDescriptor*>::iterator it = shallow_copies_.begin();

			// destruction of shallow copies
			for (;it != shallow_copies_.end();++it)
			{
				#ifdef BALL_VIEW_DEBUG
					cout << "destructing shallow copy: " << *it << endl;
				#endif

				(*it)->setParentCompositeDescriptor_(0);

				delete *it;
			}  

			shallow_copies_.clear();

			// destruction of composites
			if (shallow_copy_ == false)
			{
				#ifdef BALL_VIEW_DEBUG
					cout << "destructing composite: " << (void *)composite_<< endl;
				#endif

				delete composite_;
			}
			else
			{
				if (parent_ != 0)
				{
					#ifdef BALL_VIEW_DEBUG
					cout << "removing shallow copy ´" << (void *)this << "´ from parent: " 
						<< (void *)parent_ << endl;
					#endif

					parent_->removeShallowCompositeDescriptor_(this);
				}
			}
		}
			
		void CompositeDescriptor::set
			(CompositeDescriptor& composite_descriptor, bool deep)
			throw()
		{
			destroy();

			name_ = composite_descriptor.name_;
			center_ = composite_descriptor.center_;
			quaternion_ = composite_descriptor.quaternion_;
			shallow_copy_ = (!deep);

			if (deep == true)
			{
				name_ptr_ = &name_;
				center_ptr_ = &center_;
				quaternion_ptr_ = &quaternion_;

				composite_ = (Composite *)composite_descriptor.composite_->create();

				parent_ = 0;
			}
			else
			{
				name_ptr_ = (String *)&(composite_descriptor.name_);
				center_ptr_ = (Vector3 *)&(composite_descriptor.center_);
				quaternion_ptr_ = (Quaternion *)&(composite_descriptor.quaternion_);
				composite_ = composite_descriptor.composite_;
				composite_descriptor.addShallowCompositeDescriptor_(this);
				parent_ = &composite_descriptor;
			}
		}

		const CompositeDescriptor& CompositeDescriptor::operator =
			(CompositeDescriptor& composite_descriptor)
			throw()
		{
			set(composite_descriptor);

			return *this;
		}

		void CompositeDescriptor::get
			(CompositeDescriptor& composite_descriptor, bool deep)
			throw()
		{
			composite_descriptor.set(*this, deep);
		}

		void CompositeDescriptor::swap
			(CompositeDescriptor&  /* composite_descriptor */)
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void CompositeDescriptor::update()
			throw()
		{
			// update entities
			EntityHashMap::Iterator entity_it;

			for (entity_it = entities_.begin();
					 entity_it != entities_.end();
					 ++entity_it)
			{
				entity_it->second->update();
			}

			list<CompositeDescriptor *>::iterator it = shallow_copies_.begin();

			for (; it != shallow_copies_.end(); ++it)
			{
				(*it)->update();
			}
		}

		void CompositeDescriptor::compileEntity(GLEntityDescriptor* entity)
			throw()
		{
			// first a primitiveManager and a object collector must be registered
			if (primitive_manager_ == 0
					|| object_collector_ == 0)
			{
				return;
			}

			// compile the entity
			entity->clear();
			entity->cancelUpdate();

			// collect all geometrical object from composite
			object_collector_->setRootComposite(getComposite());

			getComposite()->apply(*object_collector_);

      #ifdef BALL_VIEW_DEBUG
			  cout << "compiling entity ... ";
      #endif

			// save gl matrix stack
			glPushMatrix();
			glLoadIdentity();

			list<GLObject*>::iterator it;

			/* generating static display list */
			entity->getStaticDisplayList()->startDefinition();

			for (it = object_collector_->getStaticList().begin();
					 it != object_collector_->getStaticList().end(); 
					 ++it)
			{
				(*it)->setGLPrimitiveManager(*primitive_manager_);
				
				(*it)->draw();
			}

			entity->getStaticDisplayList()->endDefinition();
			
			/* generating static always front display list */
			entity->getStaticAlwaysFrontDisplayList()->startDefinition();

			for (it = object_collector_->getStaticAlwaysFrontList().begin();
					 it != object_collector_->getStaticAlwaysFrontList().end(); 
					 ++it)
			{
				(*it)->setGLPrimitiveManager(*primitive_manager_);
				
				(*it)->draw();
			}

			entity->getStaticAlwaysFrontDisplayList()->endDefinition();
			
			/* generating static wireframe display list */
			entity->getStaticWireframeDisplayList()->startDefinition();

			for (it = object_collector_->getStaticWireframeList().begin();
					 it != object_collector_->getStaticWireframeList().end(); 
					 ++it)
			{
				(*it)->setGLPrimitiveManager(*primitive_manager_);
				
				(*it)->draw();
			}

			entity->getStaticWireframeAlwaysFrontDisplayList()->endDefinition();

			/* generating static wireframe always front display list */
			entity->getStaticWireframeAlwaysFrontDisplayList()->startDefinition();

			for (it = object_collector_->getStaticWireframeAlwaysFrontList().begin();
					 it != object_collector_->getStaticWireframeAlwaysFrontList().end(); 
					 ++it)
			{
				(*it)->setGLPrimitiveManager(*primitive_manager_);
				
				(*it)->draw();
			}

			entity->getStaticWireframeAlwaysFrontDisplayList()->endDefinition();

			/* generating dynamic display list */
			entity->getDynamicDisplayList()->startDefinition();
			
			for (it = object_collector_->getDynamicList().begin();
					 it != object_collector_->getDynamicList().end(); 
					 ++it)
			{
				(*it)->setGLPrimitiveManager(*primitive_manager_);
				
				(*it)->draw();
			}
			
			entity->getDynamicDisplayList()->endDefinition();

			/* generating dynamic always front display list */
			entity->getDynamicAlwaysFrontDisplayList()->startDefinition();
			
			for (it = object_collector_->getDynamicAlwaysFrontList().begin();
					 it != object_collector_->getDynamicAlwaysFrontList().end(); 
					 ++it)
			{
				(*it)->setGLPrimitiveManager(*primitive_manager_);
				
				(*it)->draw();
			}
			
			entity->getDynamicAlwaysFrontDisplayList()->endDefinition();

			/* generating transparent display list */
			entity->getTransparentDisplayList()->startDefinition();
			
			for (it = object_collector_->getTransparentList().begin();
					 it != object_collector_->getTransparentList().end(); 
					 ++it)
			{
				(*it)->setGLPrimitiveManager(*primitive_manager_);
				
				(*it)->draw();
			}

			entity->getDynamicDisplayList()->endDefinition();
			
			/* generating transparent always front display list */
			entity->getTransparentAlwaysFrontDisplayList()->startDefinition();
			
			for (it = object_collector_->getTransparentAlwaysFrontList().begin();
					 it != object_collector_->getTransparentAlwaysFrontList().end(); 
					 ++it)
			{
				(*it)->setGLPrimitiveManager(*primitive_manager_);
				
				(*it)->draw();
			}

			entity->getDynamicAlwaysFrontDisplayList()->endDefinition();
			
      #ifdef BALL_VIEW_DEBUG
			  cout << "ok." << endl;
      #endif

			// restore Matrix stack
			glPopMatrix();
		}

		void CompositeDescriptor::drawEntity(bool dynamic)
			throw()
		{
			GLEntityDescriptor* entity = 0;

			// an entity exists already for the given primitive_manager (scene)
			// then use the entities associated to it
			if (entities_.has(primitive_manager_))
			{
				EntityHashMap::Iterator it = entities_.find(primitive_manager_);

				entity = it->second;
			}
			else // create a new entity
			{
				entity = new GLEntityDescriptor();

				entities_.insert(EntityHashMap::ValueType(primitive_manager_, entity));
			}

			// compile entity if an update is needed
			if (entity->needUpdate())
			{
				compileEntity(entity);
			}

			// draw only dynamic objects
			if (dynamic)
			{
				glDisable(GL_LIGHTING);

				entity->getDynamicDisplayList()->draw();

				// always front objects
				glDisable(GL_DEPTH_TEST);
				entity->getDynamicAlwaysFrontDisplayList()->draw();
				glEnable(GL_DEPTH_TEST);

				glEnable(GL_LIGHTING);
			}
			else // draw static objects
			{
				glDisable(GL_LIGHTING);
				entity->getStaticWireframeDisplayList()->draw();
				glEnable(GL_LIGHTING);
				entity->getStaticDisplayList()->draw();
				/* enable blending */
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);
				/* make the depth buffer read only */
				glDepthMask(GL_FALSE);
				entity->getTransparentDisplayList()->draw();
				/* restore to normal mode */
				glDepthMask(GL_TRUE);
				/* disable blenging */
				glDisable(GL_BLEND);

				// always front objects
				glDisable(GL_DEPTH_TEST);
				entity->getStaticAlwaysFrontDisplayList()->draw();
				glDisable(GL_LIGHTING);
				entity->getStaticWireframeAlwaysFrontDisplayList()->draw();
				glEnable(GL_LIGHTING);
				/* enable blending */
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);
				/* make the depth buffer read only */
				glDepthMask(GL_FALSE);
				entity->getTransparentAlwaysFrontDisplayList()->draw();
				/* restore to normal mode */
				glDepthMask(GL_TRUE);
				/* disable blenging */
				glDisable(GL_BLEND);

				glEnable(GL_DEPTH_TEST);
			}
		}

		void CompositeDescriptor::drawDirect(bool dynamic, bool with_names)
			throw()
		{
			if (primitive_manager_ == 0
					|| object_collector_ == 0)
			{
				return;
			}

			if (dynamic)
			{
				drawDirectDynamic_(with_names);
			}
			else
			{
				drawDirectStatic_(with_names);
			}
		}

		void CompositeDescriptor::drawDirectStatic_(bool with_names)
		{
			object_collector_->setRootComposite(getComposite());
									
			getComposite()->apply(*object_collector_);
																		
			glDisable(GL_LIGHTING);
									
			List<GLObject *>::Iterator it;

			// static wireframe objects
			for (it = object_collector_->getStaticWireframeList().begin();
					 it != object_collector_->getStaticWireframeList().end();
					 ++it)
			{
				(*it)->setGLPrimitiveManager(*primitive_manager_);

				(*it)->draw(with_names);
			}

			glEnable(GL_LIGHTING);

			// static objects
			for (it = object_collector_->getStaticList().begin();
					 it != object_collector_->getStaticList().end(); 
					 ++it)
			{
				(*it)->setGLPrimitiveManager(*primitive_manager_);
									
				(*it)->draw(with_names);
			}

			// enable blending 
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);
			// make the depth buffer read only 
			glDepthMask(GL_FALSE);
									
			// transparent objects
			for (it  = object_collector_->getTransparentList().begin();
					 it != object_collector_->getTransparentList().end();
					 ++it)
			{
				(*it)->setGLPrimitiveManager(*primitive_manager_);
				
				(*it)->draw(with_names);
			}
			
			// restore to normal mode 
			glDepthMask(GL_TRUE);
			// disable blenging 
			glDisable(GL_BLEND);


			// always front objects
			glDisable(GL_DEPTH_TEST);

			// always front static objects
			for (it = object_collector_->getStaticAlwaysFrontList().begin();
					 it != object_collector_->getStaticAlwaysFrontList().end(); 
					 ++it)
			{
				(*it)->setGLPrimitiveManager(*primitive_manager_);
									
				(*it)->draw(with_names);
			}

			glDisable(GL_LIGHTING);
			// static wireframe always front objects
			for (it = object_collector_->getStaticWireframeAlwaysFrontList().begin();
					 it != object_collector_->getStaticWireframeAlwaysFrontList().end();
					 ++it)
			{
				(*it)->setGLPrimitiveManager(*primitive_manager_);

				(*it)->draw(with_names);
			}
			glEnable(GL_LIGHTING);

			// enable blending 
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);
			// make the depth buffer read only 
			glDepthMask(GL_FALSE);
									
			// transparent objects
			for (it  = object_collector_->getTransparentAlwaysFrontList().begin();
					 it != object_collector_->getTransparentAlwaysFrontList().end();
					 ++it)
			{
				(*it)->setGLPrimitiveManager(*primitive_manager_);
				
				(*it)->draw(with_names);
			}
			
			// restore to normal mode 
			glDepthMask(GL_TRUE);
			// disable blenging 
			glDisable(GL_BLEND);

			glEnable(GL_DEPTH_TEST);
		}

		void CompositeDescriptor::drawDirectDynamic_(bool with_names)
		{
			glDisable(GL_LIGHTING);

			object_collector_->setRootComposite(getComposite());
			
			getComposite()->apply(*object_collector_);
							
			List<GLObject *>::Iterator it;
			
			// dynamic objects
			for (it = object_collector_->getDynamicList().begin();
					 it != object_collector_->getDynamicList().end();
					 ++it)
			{
				(*it)->setGLPrimitiveManager(*primitive_manager_);
				
				(*it)->draw(with_names);
			}

			// always front objects
			glDisable(GL_DEPTH_TEST);

			// always front dynamic objects
			for (it = object_collector_->getDynamicAlwaysFrontList().begin();
					 it != object_collector_->getDynamicAlwaysFrontList().end();
					 ++it)
			{
				(*it)->setGLPrimitiveManager(*primitive_manager_);
				
				(*it)->draw(with_names);
			}

			glEnable(GL_DEPTH_TEST);	

			glEnable(GL_LIGHTING);
		}

		bool CompositeDescriptor::isValid() const
			throw()
		{
			list<CompositeDescriptor*>::const_iterator it = shallow_copies_.begin();

			for (; it != shallow_copies_.end(); ++it)
			{
				if ((*it)->getParentCompositeDescriptor_() != this)
				{
					return false;
				}
			}

			return true;
		}

		void CompositeDescriptor::dump
			(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);

			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "name:                  " << *name_ptr_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "center:                " << *center_ptr_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "quaternion:            " << *quaternion_ptr_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "composite:             " << (void *)composite_ << endl;
			
			entities_.dump(s, depth + 1);
			
			BALL_DUMP_DEPTH(s, depth);
			s << "shallow copy:          " << shallow_copy_ << endl;

			// ?????: keine dump-Methode fuer STL-Listen!
			//shallow_copies_.dump(s, depth);

			BALL_DUMP_DEPTH(s, depth);
			s << "parent descriptor:     " 
					 << (void *)parent_ << endl;

			BALL_DUMP_STREAM_SUFFIX(s);     
		}

		void CompositeDescriptor::read(istream &/* s */)
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void CompositeDescriptor::write(ostream &/*s*/) const
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/compositeDescriptor.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL
