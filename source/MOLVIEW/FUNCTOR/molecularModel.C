// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularModel.C,v 1.2 2003/08/26 09:18:01 oliver Exp $

#include <BALL/MOLVIEW/FUNCTOR/molecularModel.h>

using namespace std;

namespace BALL
{
	namespace MOLVIEW
	{
		MolecularModelProcessor::MolecularModelProcessor()
			throw()
			:	ModelProcessor(),
				model_connector_(const_cast<BaseModelConnector*>((BaseModelConnector*)&RTTI::getDefault<BaseModelConnector>()))
		{
		}

		MolecularModelProcessor::MolecularModelProcessor
						(const MolecularModelProcessor& base_model_processor)
			throw()
			:	ModelProcessor(base_model_processor),
				model_connector_(base_model_processor.model_connector_)
		{
		}

		MolecularModelProcessor::~MolecularModelProcessor()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info () << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<MolecularModelProcessor>() << std::endl;
			#endif 

			clear();
		}

		void MolecularModelProcessor::clear()
			throw()
		{
			model_connector_ = const_cast<BaseModelConnector*>
				((BaseModelConnector*)&RTTI::getDefault<BaseModelConnector>());

			ModelProcessor::clear();
		}

		void MolecularModelProcessor::set(const MolecularModelProcessor& base_model_processor)
			throw()
		{
			model_connector_ = base_model_processor.model_connector_;
		}

		const MolecularModelProcessor& MolecularModelProcessor::operator = 
			(const MolecularModelProcessor& base_model_processor)
			throw()
		{
			set(base_model_processor);
			return *this;
		}

		void MolecularModelProcessor::swap(MolecularModelProcessor& base_model_processor)
			throw()
		{
			BaseModelConnector *model_connector = model_connector_;
			model_connector_ = base_model_processor.model_connector_;
			base_model_processor.model_connector_ = model_connector;
		}

		bool MolecularModelProcessor::start()
		{
			model_connector_->setGeometricObjectsList(&geometric_objects_);
			return true;
		}

		bool MolecularModelProcessor::isValid() const
			throw()
		{
			return (model_connector_->isValid());
		}

		void MolecularModelProcessor::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "Properties: -------------------------------" << endl;

			dump_(s, depth + 1);

			BALL_DUMP_DEPTH(s, depth);
			s << "-----------------------------------------------------" << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void MolecularModelProcessor::dump_(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			BALL_DUMP_STREAM_SUFFIX(s);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/molecularModel.iC>
#		endif

	} // namespace MOLVIEW
} // namespace BALL
