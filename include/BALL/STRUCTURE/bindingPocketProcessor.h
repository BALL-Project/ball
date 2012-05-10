#ifndef BALL_STRUCTURE_BINDINGPOCKETPROCESSOR_H
#define BALL_STRUCTURE_BINDINGPOCKETPROCESSOR_H

#ifndef BALL_CONCEPT_PROCESSOR_H
# include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
# include <BALL/KERNEL/residue.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
# include <BALL/MATHS/vector3.h> 
#endif

#ifndef BALL_KERNEL_SYSTEM_H
# include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_DATATYPE_HASHGRID_H
# include <BALL/DATATYPE/hashGrid.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#include <vector>
#include <string>

#include <fstream>

namespace BALL
{

  /** This processor searches for binding pockets in a given atom container. The pockets are detected
	 *  with the PASS method (Fast Feature Selection Using Fractal Dimension (2000) Caetano Traina, Jr., Agma Traina, et al)
   *  \ingroup StructureMiscellaneous
   */
  class BALL_EXPORT BindingPocketProcessor 
    : public UnaryProcessor<Composite>
  {
  
  public:
    
    /** Options
     */
    struct Option
    {
      /// use only heavy atoms for computation
      static const String HEAVY_ONLY;
      /// try to parse ini-file
      static const String PARSE_INI_FILE;
      /// name of the ini-file
      static const String INI_FILE_NAME;
 
      /// atom radius of individual elements
      static const String RADIUS_HYDROGEN;
      static const String RADIUS_OXYGEN;
      static const String RADIUS_NITROGEN;
      static const String RADIUS_CARBON;
      static const String RADIUS_SULFUR;

      /// probe sphere radius for computation with hydrogen atoms 
      static const String PROBE_SPHERE_RADIUS;
      /// probe sphere radius for heavy atoms only
      static const String PROBE_SPHERE_RADIUS_HYDROGEN_FREE;
      /// probe radius for colision of probes in one layer
      static const String PROBE_LAYER_RADIUS;
      /// burial count threshold for computation with hydrogen atoms
      static const String BURIAL_COUNT_THRESHOLD;
      /// burial count threshold for heavy atoms only      
      static const String BURIAL_COUNT_THRESHOLD_HYDROGEN_FREE;
      /// radius for the computation of the burial count
      static const String BURIAL_COUNT_RADIUS;
      /// options for the computation of the probe weight
      static const String PW_SQUARE_WELL;
      static const String PW_GAUSSIAN_WIDTH;
      static const String PW_MAX_DISTANCE;
      /// maximum number of active site points
      static const String MAX_ASPS;
      // additional factor for the computation of the collision of two spheres 
      static const String CLASH_FACTOR;
      /// minumum distance between two active site points
      static const String ASP_SEPERATION;
    };
    
    /** Defaults
     */
    struct Default
    {
      static const bool HEAVY_ONLY;
      static const bool PARSE_INI_FILE;
      static const String INI_FILE_NAME;

      static const double RADIUS_HYDROGEN;
      static const double RADIUS_OXYGEN;
      static const double RADIUS_NITROGEN;
      static const double RADIUS_CARBON;
      static const double RADIUS_SULFUR;
      
      static const double PROBE_SPHERE_RADIUS;
      static const double PROBE_SPHERE_RADIUS_HYDROGEN_FREE;
      static const double PROBE_LAYER_RADIUS;         
      static const Position BURIAL_COUNT_THRESHOLD;
      static const Position BURIAL_COUNT_THRESHOLD_HYDROGEN_FREE;      
      static const double BURIAL_COUNT_RADIUS;
      static const double PW_SQUARE_WELL;
      static const double PW_GAUSSIAN_WIDTH;
      static const double PW_MAX_DISTANCE;
      static const Position MAX_ASPS;
      static const double CLASH_FACTOR;
      static const double ASP_SEPERATION;
    };
    
    /** standard constructor
     */
    BindingPocketProcessor();
    
    /** constructor with options
     */
    BindingPocketProcessor(Options& new_options);		
    
    /** copy constructor
     */
    BindingPocketProcessor(const BindingPocketProcessor& pp);

    /** destructor
     */
    virtual ~BindingPocketProcessor();

    /** assignment operator
     */
    const BindingPocketProcessor& operator=(const BindingPocketProcessor& pp);
   
    /// Finish .
    virtual bool finish();

    virtual Processor::Result operator() (Composite &composite);

    /** Returns the position and burial counts of the calculated actice site points
     */
    const vector<std::pair<Vector3,double> >& getActiveSitePoints() const;
    
    /** Returns a vector of vectors, each containing one layer with position and probe weight of its spheres 
     */
    const vector<vector<std::pair<Vector3,double> > >& getLayers() const;

    /** options for BALLPass 
     */
    Options options;

  private:
    
		/** This class implements a sphere
		 *  each sphere belongs to a layer and possesses a radius, burial count and probe weight
		 */
    class Sphere_
    {
    public:
      /// Default constructor.
      Sphere_();
      
      /** position vector
       */
      BALL::Vector3 position;
      
      /** radius
       */
      float radius;
      
      /** burial count 
       */
      Position b_count;
      
      /** probe weight
       */
      float probe_weight;	
      
      /** layer, the sphere belongs to
       */
      Position layer;
      
      /** smaller operator for sort, applying the probe weight
       */
      inline bool operator<(const Sphere_& sp) const {return probe_weight > sp.probe_weight;};
      
      /** ids for identification
       */
      int id;
      static int global_id; 
      Position a,b,c,d;
      
      /** equality operator using a,b,c and d
       */
      inline bool operator==(const Sphere_& sp) const {return a == sp.a && b == sp.b && c == sp.c && d == sp.d;};
    };  
    
    /** compares two spheres
     */
    class SphereComparator_
		{
			public:
				int operator()(const Sphere_& sp1, const Sphere_& sp2) const
				{
					if (sp1.a != sp2.a)
						return sp1.a < sp2.a;
					if (sp1.b != sp2.b)
						return sp1.b < sp2.b;
					if (sp1.c != sp2.c)
						return sp1.c < sp2.c;
					if (sp1.d != sp2.d)
						return sp1.d < sp2.d;
					return 0;
				};
		};
    
    /** current layer
     */ 
    Position layer_;
    
    /** output operator for Sphere
		 *  TODO: implement!
     */
    friend std::ostream& operator<<(std::ostream& os, const BindingPocketProcessor::Sphere_& /*sp*/) {return os;};
    
    /** store all spheres
     */
    HashGrid3<Sphere_> sphere_grid_;
    
    /** store protein atoms
     */
    HashGrid3<Sphere_> protein_grid_;
    
    /** store all sphere layers
     */
    HashGrid3<Sphere_> layer_grid_;
    
    /** needed for sphere collision
     */
    HashGrid3<Sphere_> clayer_grid_;
    
    /** need for computation of burial count
     */
    HashGrid3<Sphere_> BC_grid_;  
    
    /** need for computation of probe weight
     */
    HashGrid3<Sphere_> pw_grid_;  

    /** store all possible new spheres
     */
    HashGrid3<Sphere_> candidates_grid_;
    
    std::vector<Sphere_> sorted_spheres_;
    
    /** first layer radius
     */
    double sphere_rad_;
    
    /** sphere radius for following layers
     */
    double second_layer_rad_;
    
    /** radius for collision with protein atoms
     */
    double protein_clash_rad_;
    
    /** threshold for burial count filer
     */
    double BC_threshold_;
    
    /** number of newly found spheres
     */
    Position found_new_;
    
    double c_factor_;
    
    /** store all possible new spheres
     */
    std::vector<Sphere_> candidates;
  
    std::vector<std::pair<Vector3,double> >  asps_;
 
    std::vector<std::vector<std::pair<Vector3,double> > > layers_; 
  
    /** Determine the radius of an element
		 *
		 * @param element the full name of the element for which a radius should be looked up. Example: "Oxygen"
		 * @throw IllegalArgument if an unknown element has been passed
		 * @return The radius of the specified element
     */
    double radius_(const String& element);
    
    /** calculates two spheres if possible
     */
    void trySphere__(HashGridBox3<Sphere_>::DataIterator data_it, 
		    HashGridBox3<Sphere_>::DataIterator data_it2, 
		    HashGridBox3<Sphere_>::DataIterator data_it3);
    
    /** compute sphere layers
     */
    void addSphere_s_();
    
    /** geometric computation
     */
    Vector3 calculateT_(HashGridBox3<Sphere_>::DataIterator data_it, 
			HashGridBox3<Sphere_>::DataIterator data_it2);
    
    /** test if three spheres produce two offspring spheres
     */
    void testFirst_(HashGrid3<Sphere_>& hg,HashGridBox3<Sphere_>::DataIterator data_it,
		    Position x, Position y, Position z);
    
    void testSecond_(HashGrid3<Sphere_>& hg,HashGridBox3<Sphere_>::DataIterator data_it,
		     HashGridBox3<Sphere_>::DataIterator data_it2,
		     Position x, Position y, Position z,Position step);
    
    /** test clash with all existing spheres
     */
    bool testSphereClash_(Vector3& v);
    
    /** test clash with protein atoms
     */
    bool testProteinClash_(Vector3& v);
    
    /** test for burial count
     */
    bool testBC_(Sphere_& sp);
    
    /** remove redundant spheres
     */
    void weedCandidates_();
    
    /** read parameter file
     */
    bool parse_();
    
    
    bool second_it_;
    
  };   
  
} //namesspace BALL

#endif // BALL_STRUCTURE_BINDINGPOCKETPROCESSOR_H
