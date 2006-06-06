
#ifndef BALL_STRUCTURE_RINGCLUSTERER_H
#define BALL_STRUCTURE_RINGCLUSTERER_H

#include <vector>
#include <BALL/STRUCTURE/ringAnalyser.h> /// ???? notwendig?


namespace BALL
{

	class Atom;

	/**
		* \brief class, providing a method for agglomerative clustering of single rings to ringsystems
		* 
		*/
	class RingClusterer
	{
	public:

		/**
		* \brief Default-Constructor  
		*/
		RingClusterer();

		/**
		* \brief Destructor  
		*/
		~RingClusterer();

		/**
		* \brief performs an agglomerative clustering of single rings to ringsystems
		* @param rings the smallest set of smallest rings of the input molecule
		* @return all ringsystems of the input molecule
		*/
		vector<vector<vector<Atom*> > > clusterRings(vector<vector<Atom*> >& rings);
	};

} // namespace BALL
 
#endif // BALL_STRUCTURE_RINGCLUSTERER_H
