// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#include <BALL/DATATYPE/options.h>

#ifndef DOCKINGALGORITHM_H
#define DOCKINGALGORITHM_H


namespace BALL
{

	namespace Docking
	{
		class Docking;

		/** A class defining the interface for docking algorithms.

				This is a first iteration.

		*/
		class DockingAlgorithm
		{

			public:

			
				/** Attributes
				*/
				Options options;


				/** Operations
				*/
				void setup(System &system1,System &system2 ,Options &options)
					throw();

				/**
				*/
				void start()
					throw();


				/**
				*/
				void pause()
					throw();


				/**
				*/
				void proceed()
					throw();


				/**
				*/
				void abort()
					throw();

				/**
				*/
				bool hasFinished() const
					throw();

				/**
				*/
				float getProgress() const
					throw();
		};

	}

}

#endif
