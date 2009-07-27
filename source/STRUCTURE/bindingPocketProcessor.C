#include <BALL/STRUCTURE/bindingPocketProcessor.h>

#include <BALL/KERNEL/PTE.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/FORMAT/HINFile.h>




namespace BALL
{

  const String BindingPocketProcessor::Option::HEAVY_ONLY = "heavy_only";
  const String BindingPocketProcessor::Option::PARSE_INI_FILE = "parse_ini_file";
  const String BindingPocketProcessor::Option::INI_FILE_NAME  = "ini_file_name";  

  const String BindingPocketProcessor::Option::RADIUS_HYDROGEN = "radius_hydrogen";
  const String BindingPocketProcessor::Option::RADIUS_OXYGEN = "radius_oxygen";
  const String BindingPocketProcessor::Option::RADIUS_NITROGEN = "radius_nitrogen";
  const String BindingPocketProcessor::Option::RADIUS_CARBON = "radius_carbon";
  const String BindingPocketProcessor::Option::RADIUS_SULFUR = "radius_sulfur";

  const String BindingPocketProcessor::Option::PROBE_SPHERE_RADIUS = "probe_sphere_radius";
  const String BindingPocketProcessor::Option::PROBE_SPHERE_RADIUS_HYDROGEN_FREE = "probe_sphere_radius_hydrogen_free";
  const String BindingPocketProcessor::Option::PROBE_LAYER_RADIUS = "probe_layer_radius";      
  const String BindingPocketProcessor::Option::BURIAL_COUNT_THRESHOLD = "burial_count_threshold";
  const String BindingPocketProcessor::Option::BURIAL_COUNT_THRESHOLD_HYDROGEN_FREE = "burial_count_threshold_hydrogen_free";
  const String BindingPocketProcessor::Option::BURIAL_COUNT_RADIUS = "burial_count_radius";
  const String BindingPocketProcessor::Option::PW_SQUARE_WELL = "pw_square_well";
  const String BindingPocketProcessor::Option::PW_GAUSSIAN_WIDTH = "pw_gauassian_width";
  const String BindingPocketProcessor::Option::PW_MAX_DISTANCE = "pw_max_distance";
  const String BindingPocketProcessor::Option::MAX_ASPS = "max_asps";
  const String BindingPocketProcessor::Option::CLASH_FACTOR = "clash_factor";
  const String BindingPocketProcessor::Option::ASP_SEPERATION = "asp_seperation";


  const bool BindingPocketProcessor::Default::HEAVY_ONLY = true;
  const bool BindingPocketProcessor::Default::PARSE_INI_FILE = true; 
  const String BindingPocketProcessor::Default::INI_FILE_NAME = "pass.ini"; 
  
 
  const double BindingPocketProcessor::Default::RADIUS_HYDROGEN = 1.2;
  const double BindingPocketProcessor::Default::RADIUS_OXYGEN = 1.52;
  const double BindingPocketProcessor::Default::RADIUS_NITROGEN = 1.55;
  const double BindingPocketProcessor::Default::RADIUS_CARBON = 1.7;
  const double BindingPocketProcessor::Default::RADIUS_SULFUR = 1.8;

  const double BindingPocketProcessor::Default::PROBE_SPHERE_RADIUS = 1.5;
  const double BindingPocketProcessor::Default::PROBE_SPHERE_RADIUS_HYDROGEN_FREE = 1.8;
  const double BindingPocketProcessor::Default::PROBE_LAYER_RADIUS = 0.7;      
  const Position BindingPocketProcessor::Default::BURIAL_COUNT_THRESHOLD = 75;
  const Position BindingPocketProcessor::Default::BURIAL_COUNT_THRESHOLD_HYDROGEN_FREE = 55;
  const double BindingPocketProcessor::Default::BURIAL_COUNT_RADIUS = 8.0;
  const double BindingPocketProcessor::Default::PW_SQUARE_WELL = 2.0;
  const double BindingPocketProcessor::Default::PW_GAUSSIAN_WIDTH = 1.0;
  const double BindingPocketProcessor::Default::PW_MAX_DISTANCE = 4.0;
  const Position BindingPocketProcessor::Default::MAX_ASPS = 10;
  const double BindingPocketProcessor::Default::CLASH_FACTOR = 0.95;
  const double BindingPocketProcessor::Default::ASP_SEPERATION = 4.0;

  BindingPocketProcessor::BindingPocketProcessor():
    layer_(0),
    sphere_rad_(0),
    second_layer_rad_(0),
    protein_clash_rad_(0),
    BC_threshold_(0),
    found_new_(0),
    c_factor_(0),
    second_it_(0)
  {
    options.setDefaultBool(Option::HEAVY_ONLY,Default::HEAVY_ONLY);
    options.setDefaultBool(Option::PARSE_INI_FILE,Default::PARSE_INI_FILE);
    options.setDefaultReal(Option::RADIUS_HYDROGEN,Default::RADIUS_HYDROGEN);
    options.setDefaultReal(Option::RADIUS_OXYGEN,Default::RADIUS_OXYGEN);
    options.setDefaultReal(Option::RADIUS_NITROGEN,Default::RADIUS_NITROGEN);
    options.setDefaultReal(Option::RADIUS_CARBON,Default::RADIUS_CARBON);
    options.setDefaultReal(Option::RADIUS_SULFUR,Default::RADIUS_SULFUR);
    options.setDefaultReal(Option::PROBE_SPHERE_RADIUS,Default::PROBE_SPHERE_RADIUS);
    options.setDefaultReal(Option::PROBE_SPHERE_RADIUS_HYDROGEN_FREE,Default::PROBE_SPHERE_RADIUS_HYDROGEN_FREE);
    options.setDefaultReal(Option::PROBE_LAYER_RADIUS,Default::PROBE_LAYER_RADIUS);
    options.setDefaultInteger(Option::BURIAL_COUNT_THRESHOLD,Default::BURIAL_COUNT_THRESHOLD);
    options.setDefaultInteger(Option::BURIAL_COUNT_THRESHOLD_HYDROGEN_FREE,Default::BURIAL_COUNT_THRESHOLD_HYDROGEN_FREE);
    options.setDefaultReal(Option::BURIAL_COUNT_RADIUS,Default::BURIAL_COUNT_RADIUS);
    options.setDefaultReal(Option::PW_SQUARE_WELL,Default::PW_SQUARE_WELL);
    options.setDefaultReal(Option::PW_GAUSSIAN_WIDTH,Default::PW_GAUSSIAN_WIDTH);
    options.setDefaultReal(Option::PW_MAX_DISTANCE,Default::PW_MAX_DISTANCE);
    options.setDefaultInteger(Option::MAX_ASPS,Default::MAX_ASPS);
    options.setDefaultReal(Option::CLASH_FACTOR,Default::CLASH_FACTOR);
    options.setDefaultReal(Option::ASP_SEPERATION,Default::ASP_SEPERATION);
  }

  BindingPocketProcessor::BindingPocketProcessor(Options& new_options):
    layer_(0),
    sphere_rad_(0),
    second_layer_rad_(0),
    protein_clash_rad_(0),
    BC_threshold_(0),
    found_new_(0),
    c_factor_(0),
    second_it_(0)		
  {
    options = new_options;

    if (!options.has(Option::HEAVY_ONLY))
      options.setDefaultBool(Option::HEAVY_ONLY,Default::HEAVY_ONLY);
 
    if (!options.has(Option::PARSE_INI_FILE))   
      options.setDefaultBool(Option::PARSE_INI_FILE,Default::PARSE_INI_FILE);

    if (!options.has(Option::RADIUS_HYDROGEN))   
      options.setDefaultReal(Option::RADIUS_HYDROGEN,Default::RADIUS_HYDROGEN);

    if (!options.has(Option::RADIUS_OXYGEN))   
      options.setDefaultReal(Option::RADIUS_OXYGEN,Default::RADIUS_OXYGEN);

    if (!options.has(Option::RADIUS_NITROGEN))   
      options.setDefaultReal(Option::RADIUS_NITROGEN,Default::RADIUS_NITROGEN);

    if (!options.has(Option::RADIUS_CARBON))   
      options.setDefaultReal(Option::RADIUS_CARBON,Default::RADIUS_CARBON);

    if (!options.has(Option::RADIUS_SULFUR))   
      options.setDefaultReal(Option::RADIUS_SULFUR,Default::RADIUS_SULFUR);

    if (!options.has(Option::PROBE_SPHERE_RADIUS))   
      options.setDefaultReal(Option::PROBE_SPHERE_RADIUS,Default::PROBE_SPHERE_RADIUS);

    if (!options.has(Option::PROBE_SPHERE_RADIUS_HYDROGEN_FREE))   
      options.setDefaultReal(Option::PROBE_SPHERE_RADIUS_HYDROGEN_FREE,Default::PROBE_SPHERE_RADIUS_HYDROGEN_FREE);

    if (!options.has(Option::PROBE_LAYER_RADIUS))   
      options.setDefaultReal(Option::PROBE_LAYER_RADIUS,Default::PROBE_LAYER_RADIUS);

    if (!options.has(Option::BURIAL_COUNT_THRESHOLD))   
      options.setDefaultInteger(Option::BURIAL_COUNT_THRESHOLD,Default::BURIAL_COUNT_THRESHOLD);

    if (!options.has(Option::BURIAL_COUNT_THRESHOLD_HYDROGEN_FREE))   
      options.setDefaultInteger(Option::BURIAL_COUNT_THRESHOLD_HYDROGEN_FREE,Default::BURIAL_COUNT_THRESHOLD_HYDROGEN_FREE);

    if (!options.has(Option::BURIAL_COUNT_RADIUS))   
      options.setDefaultReal(Option::BURIAL_COUNT_RADIUS,Default::BURIAL_COUNT_RADIUS);
    
    if (!options.has(Option::PW_SQUARE_WELL))   
      options.setDefaultReal(Option::PW_SQUARE_WELL,Default::PW_SQUARE_WELL);
    
    if (!options.has(Option::PW_GAUSSIAN_WIDTH))   
      options.setDefaultReal(Option::PW_GAUSSIAN_WIDTH,Default::PW_GAUSSIAN_WIDTH);
    
    if (!options.has(Option::PW_MAX_DISTANCE))   
      options.setDefaultReal(Option::PW_MAX_DISTANCE,Default::PW_MAX_DISTANCE);
    
    if (!options.has(Option::MAX_ASPS))   
      options.setDefaultInteger(Option::MAX_ASPS,Default::MAX_ASPS);
    
    if (!options.has(Option::CLASH_FACTOR))   
      options.setDefaultReal(Option::CLASH_FACTOR,Default::CLASH_FACTOR);
    
    if (!options.has(Option::ASP_SEPERATION))   
      options.setDefaultReal(Option::ASP_SEPERATION,Default::ASP_SEPERATION);
  } 


  BindingPocketProcessor::~BindingPocketProcessor()		
  {
  }

  BindingPocketProcessor::BindingPocketProcessor(const BindingPocketProcessor& pp):
    UnaryProcessor<BALL::Composite>(pp),
    layer_(pp.layer_),
    sphere_grid_(pp.sphere_grid_),
    protein_grid_(pp.protein_grid_),
    layer_grid_(pp.layer_grid_),
    clayer_grid_(pp.clayer_grid_),
    BC_grid_(pp.BC_grid_),
    pw_grid_(pp.pw_grid_),  
    candidates_grid_(pp.candidates_grid_),
    sorted_spheres_(pp.sorted_spheres_),
    sphere_rad_(pp.sphere_rad_),
    second_layer_rad_(pp.second_layer_rad_),
    protein_clash_rad_(pp.protein_clash_rad_),
    BC_threshold_(pp.BC_threshold_),
    found_new_(pp.found_new_),
    c_factor_(pp.c_factor_),
    candidates(pp.candidates),
    asps_(pp.asps_),
    layers_(pp.layers_),
    second_it_(pp.second_it_)
  {
  }

 
  const BindingPocketProcessor& BindingPocketProcessor::operator=(const BindingPocketProcessor& pp)
	{
		if (this == &pp)
			return *this;

		layer_ = pp.layer_;
		sphere_grid_ = pp.sphere_grid_;
		protein_grid_ = pp.protein_grid_;
		layer_grid_ = pp.layer_grid_;
		clayer_grid_ = pp.clayer_grid_;
		BC_grid_ = pp.BC_grid_;
		pw_grid_ = pp.pw_grid_;  
		candidates_grid_ = pp.candidates_grid_;
		sorted_spheres_ = pp.sorted_spheres_;
		sphere_rad_ = pp.sphere_rad_;
		second_layer_rad_ = pp.second_layer_rad_;
		protein_clash_rad_ = pp.protein_clash_rad_;
		BC_threshold_ = pp.BC_threshold_;
		found_new_ = pp.found_new_;
		c_factor_ = pp.c_factor_;
		candidates = pp.candidates;
		second_it_ = pp.second_it_;
		asps_ = pp.asps_;
		layers_ = pp.layers_;

		return *this;
	}
  
  Processor::Result BindingPocketProcessor::operator() (Composite &composite)
	{
		AtomIterator ai;

		BoundingBoxProcessor bp;

		/** start with layer 0
		 */
		layer_ = 0;

		asps_.clear();

		layers_.clear();

		// do we have a system?
		if (RTTI::isKindOf<System>(composite))
		{
			System *s = RTTI::castTo<System>(composite);
			s->apply(bp);
			ai = s->beginAtom();
		}
		else if (RTTI::isKindOf<Protein>(composite))
		{
			Protein *s = RTTI::castTo<Protein>(composite);
			s->apply(bp);
			ai = s->beginAtom();
		}
		else if (RTTI::isKindOf<Chain>(composite))
		{
			Molecule *s = RTTI::castTo<Molecule>(composite);
			s->apply(bp);
			ai = s->beginAtom();
		}

		// ri doesn't seem to exist
		if (!(+ai)) 
		{
			return Processor::CONTINUE;
		}

		/** compute options
		 */ 
		double max_rad = std::max(options.getReal(Option::RADIUS_HYDROGEN),options.getReal(Option::RADIUS_OXYGEN));
		max_rad = std::max(max_rad,options.getReal(Option::RADIUS_NITROGEN));
		max_rad = std::max(max_rad,options.getReal(Option::RADIUS_CARBON));   
		max_rad = std::max(max_rad,options.getReal(Option::RADIUS_SULFUR));   

		if (options.getBool(Option::HEAVY_ONLY))
		{
			protein_clash_rad_ = options.getReal(Option::PROBE_SPHERE_RADIUS_HYDROGEN_FREE);
			BC_threshold_ = options.getInteger(Option::BURIAL_COUNT_THRESHOLD_HYDROGEN_FREE); 
		}
		else
		{    
			protein_clash_rad_ = options.getReal(Option::PROBE_SPHERE_RADIUS);
			BC_threshold_ = options.getInteger(Option::BURIAL_COUNT_THRESHOLD);
		}

		sphere_rad_ = protein_clash_rad_;
		c_factor_ = options.getReal(Option::CLASH_FACTOR);
		second_layer_rad_ = options.getReal(Option::PROBE_LAYER_RADIUS); 

		double max_sphere_rad = 0.0;

		/** calculate maximal sphere radius
		 */
		if (sphere_rad_ > second_layer_rad_)
			max_sphere_rad = sphere_rad_;
		else
			max_sphere_rad = second_layer_rad_;

		/** clear all grids 
		 */
		sphere_grid_.clear();
		protein_grid_.clear();
		BC_grid_.clear();
		layer_grid_.clear();
		clayer_grid_.clear();
		pw_grid_.clear();

		Vector3 lower = bp.getLower();
		Vector3 upper = bp.getUpper();

		lower -= Vector3(5.0,5.0,5.0);
		upper += Vector3(5.0,5.0,5.0);

		upper -= lower;

		sphere_grid_ = HashGrid3<Sphere_>(lower,upper,2 * max_rad + 2 * sphere_rad_ + 0.1);   
		protein_grid_ = HashGrid3<Sphere_>(lower,upper,max_rad + sphere_rad_ + 0.1);

		BC_grid_ = HashGrid3<Sphere_>(lower,upper,options.getReal(Option::BURIAL_COUNT_RADIUS)+0.1);

		// smaller distance possible ?!?!?!?
		layer_grid_ = HashGrid3<Sphere_>(lower,upper,2 * max_rad + 2 * sphere_rad_ + 0.1);
		clayer_grid_ = HashGrid3<Sphere_>(lower,upper,2 * max_sphere_rad + 0.1);
		pw_grid_ = HashGrid3<Sphere_>(lower,upper,options.getReal(Option::PW_MAX_DISTANCE)+0.1);
		candidates_grid_ = HashGrid3<Sphere_>(lower,upper,options.getReal(Option::PROBE_LAYER_RADIUS)+0.1);

		/** insert protein atoms as spheres
		 */
		for(;+ai;++ai)
		{
			String element_name = ai->getElement().getName();

			if (!(options.getBool(Option::HEAVY_ONLY)) || element_name != "Hydrogen")
			{
				Sphere_ tmp;

				/** initialize ids 
				 */
				tmp.a = tmp.b = tmp.c = tmp.d = tmp.id = Sphere_::global_id++;
				tmp.position = ai->getPosition();
				tmp.radius = radius_(element_name);

				sphere_grid_.insert(tmp.position,tmp);
				protein_grid_.insert(tmp.position,tmp);
				BC_grid_.insert(tmp.position,tmp);

				sorted_spheres_.push_back(tmp);
			}
		}


		return Processor::BREAK;

	}

  
  double BindingPocketProcessor::radius_(const String& element)
	{
		double radius(.0);

		if(element == "Hydrogen")
			radius = Default::RADIUS_HYDROGEN;
		else if(element == "Oxygen")
			radius = Default::RADIUS_OXYGEN;
		else if(element == "Nitrogen")
			radius = Default::RADIUS_NITROGEN;
		else if(element == "Carbon")
			radius = Default::RADIUS_CARBON;
		else if(element  == "Sulphur"  || element == "Sulfur")
			radius = Default::RADIUS_SULFUR; 
		else
		{
			Log.error() << "error: unknown element: "  << element << std::endl;
			throw(Exception::InvalidArgument(__FILE__, __LINE__, element));
		}

		return radius;
	}
 

  bool BindingPocketProcessor::finish()
	{
		second_it_ = false;

		/** calculate Sphere_s
		 */
		addSphere_s_();

		vector<Sphere_> v;

		HashGrid3<Sphere_>::BoxIterator box_it = pw_grid_.beginBox();

		/** compute probe weights
		 */
		for (;+box_it;box_it++)
		{

			HashGridBox3<Sphere_>::DataIterator data_it = box_it->beginData();

			for (;+data_it;data_it++)
			{
				data_it->probe_weight = 0.0;

				Position x_pos,y_pos,z_pos;

				pw_grid_.getIndices(*box_it,x_pos,y_pos,z_pos);

				Position start_x = x_pos;
				Position start_y = y_pos;
				Position start_z = z_pos;

				if (start_x != 0)
					start_x--;

				if (start_y != 0)
					start_y--;

				if (start_z != 0)
					start_z--;

				static double r0 = options.getReal(Option::PW_SQUARE_WELL);
				static double d0 = options.getReal(Option::PW_GAUSSIAN_WIDTH);

				for (Position x = start_x; x < x_pos + 2 && x < pw_grid_.getSizeX(); x++)
					for (Position y = start_y; y < y_pos + 2 && y < pw_grid_.getSizeY(); y++) 
						for (Position z = start_z; z < z_pos + 2 && z < pw_grid_.getSizeZ(); z++)
						{
							HashGridBox3<Sphere_>* box_it = pw_grid_.getBox(x,y,z);

							HashGridBox3<Sphere_>::DataIterator data_it2 = box_it->beginData();

							for (;+data_it2;data_it2++)
							{
								double distance = (data_it->position - data_it2->position).getLength();

								if (distance < r0)
									data_it->probe_weight += data_it2->b_count;
								else
									data_it->probe_weight += data_it2->b_count * exp(-((distance-r0)*(distance-r0))/(d0*d0));
							}
						}

				v.push_back(*data_it);
			}
		}

		/** sort v with regard to probe weight
		 */ 
		sort(v.begin(),v.end());

		static double rasp = options.getReal(Option::BURIAL_COUNT_RADIUS);

		/** calculate asps
		 */
		for (Position x = 0; x < v.size() && asps_.size() <= (Position)options.getInteger(Option::ASP_SEPERATION); x++)
		{
			bool insert = true;

			for (Position y = 0; y < asps_.size(); y++)

				/** if distance to existing ASP is smaler then RASP
				 */
				if ((v[x].position - asps_[y].first).getLength() < rasp)
				{
					insert = false;
					break;
				}

			if (insert)
			{
				asps_.push_back(std::pair<Vector3,double>(v[x].position,v[x].probe_weight));
			}
		}

		layers_.resize(layer_-1);

		box_it = layer_grid_.beginBox();

		for (;+box_it;box_it++)
		{
			HashGridBox3<Sphere_>::DataIterator data_it = box_it->beginData();

			for (;+data_it;data_it++)
			{
				layers_[data_it->layer].push_back(std::pair<Vector3,double>(data_it->position,data_it->b_count));
			}
		}

		return true;
	}

  const vector<std::pair<Vector3,double> >& BindingPocketProcessor::getActiveSitePoints() const
  {
    return asps_;
  }
  
  const vector<vector<std::pair<Vector3,double> > >& BindingPocketProcessor::getLayers() const
  {
    return layers_;
  }
    
  void BindingPocketProcessor::addSphere_s_()
	{
		found_new_ = 0; 

		SphereComparator_ cmp;

		/** compute first layer
		 */
		for (Position x = 0; x < sphere_grid_.getSizeX(); x++)
			for (Position y = 0; y < sphere_grid_.getSizeY(); y++)
				for (Position z = 0; z < sphere_grid_.getSizeZ(); z++)
				{
					HashGridBox3<Sphere_>* box_it = sphere_grid_.getBox(x,y,z);

					HashGridBox3<Sphere_>::DataIterator data_it = box_it->beginData();

					for (;+data_it;data_it++)
					{
						Position xx_start = x;
						Position yy_start = y;
						Position zz_start = z;

						xx_start--;
						yy_start--;
						zz_start--;

						if (xx_start > x)
							xx_start++;
						if (yy_start > y)
							yy_start++;
						if (zz_start > z)
							zz_start++;

						for (Position xx = xx_start;xx < (x + 2) && xx < sphere_grid_.getSizeX(); xx++)
							for (Position yy = yy_start;yy < (y + 2) && yy < sphere_grid_.getSizeY(); yy++)		  
								for (Position zz = zz_start;zz < (z + 2) && zz < sphere_grid_.getSizeZ(); zz++)
								{
									HashGridBox3<Sphere_>* box_it2 = sphere_grid_.getBox(xx,yy,zz);	
									HashGridBox3<Sphere_>::DataIterator data_it2 = box_it2->beginData();

									for (;+data_it2;data_it2++)
									{

										if (cmp(*data_it,*data_it2) && (data_it->position-data_it2->position).getLength() < 2 * sphere_rad_ + data_it->radius + data_it2->radius)
										{
											Position xxx_start = x;
											Position yyy_start = y;
											Position zzz_start = z;

											xxx_start--;
											yyy_start--;
											zzz_start--;

											if (xxx_start > x)
												xxx_start++;
											if (yyy_start > y)
												yyy_start++;
											if (zzz_start > z)
												zzz_start++;

											for (Position xxx = xxx_start;xxx < (x + 2) && xxx < sphere_grid_.getSizeX(); xxx++)
												for (Position yyy = yyy_start;yyy < (y + 2) && yyy < sphere_grid_.getSizeY(); yyy++)		  
													for (Position zzz = zzz_start;zzz < (z + 2) && zzz < sphere_grid_.getSizeZ(); zzz++)
													{
														HashGridBox3<Sphere_>* box_it3 = sphere_grid_.getBox(xxx,yyy,zzz);	
														HashGridBox3<Sphere_>::DataIterator data_it3 = box_it3->beginData();

														for (;+data_it3;++data_it3)
														{
															if (cmp(*data_it2,*data_it3) && 
																	(data_it->position-data_it3->position).getLength() < 2 * sphere_rad_ + data_it->radius + data_it3->radius &&
																	(data_it2->position-data_it3->position).getLength() < 2 * sphere_rad_ + data_it2->radius + data_it3->radius)
															{
																trySphere__(data_it,data_it2,data_it3);
															}
														}
													}
										}
									}
								}
					}
				}

		weedCandidates_();

		second_it_ = true;

		layer_++;

		/** adjust sphere radius
		 */
		sphere_rad_ = second_layer_rad_;

		/** compute second, third, etc. layers
		 */
		while (found_new_)// && int(layer_) < max_layers)
		{

			found_new_ = 0;

			for (Position x = 0; x < layer_grid_.getSizeX(); x++)
				for (Position y = 0; y < layer_grid_.getSizeY(); y++)
					for (Position z = 0; z < layer_grid_.getSizeZ(); z++)
					{
						HashGridBox3<Sphere_>* box_it = layer_grid_.getBox(x,y,z);

						HashGridBox3<Sphere_>::DataIterator data_it = box_it->beginData();

						for (;+data_it;data_it++)
						{
							Position xx_start = x;
							Position yy_start = y;
							Position zz_start = z;

							xx_start--;
							yy_start--;
							zz_start--;

							if (xx_start > x)
								xx_start++;
							if (yy_start > y)
								yy_start++;
							if (zz_start > z)
								zz_start++;

							for (Position xx = xx_start;xx < (x + 2) && xx < layer_grid_.getSizeX(); xx++)
								for (Position yy = yy_start;yy < (y + 2) && yy < layer_grid_.getSizeY(); yy++)		  
									for (Position zz = zz_start;zz < (z + 2) && zz < layer_grid_.getSizeZ(); zz++)
									{
										HashGridBox3<Sphere_>* box_it2 = layer_grid_.getBox(xx,yy,zz);	
										HashGridBox3<Sphere_>::DataIterator data_it2 = box_it2->beginData();

										for (;+data_it2;data_it2++)
										{

											if (cmp(*data_it,*data_it2) && (data_it->position-data_it2->position).getLength() < 2 * sphere_rad_ + data_it->radius + data_it2->radius)
											{

												Position xxx_start = x;
												Position yyy_start = y;
												Position zzz_start = z;

												xxx_start--;
												yyy_start--;
												zzz_start--;

												if (xxx_start > x)
													xxx_start++;
												if (yyy_start > y)
													yyy_start++;
												if (zzz_start > z)
													zzz_start++;

												for (Position xxx = xxx_start;xxx < (x + 2) && xxx < layer_grid_.getSizeX(); xxx++)
													for (Position yyy = yyy_start;yyy < (y + 2) && yyy < layer_grid_.getSizeY(); yyy++)		  
														for (Position zzz = zzz_start;zzz < (z + 2) && zzz < layer_grid_.getSizeZ(); zzz++)
														{
															HashGridBox3<Sphere_>* box_it3 = layer_grid_.getBox(xxx,yyy,zzz);	
															HashGridBox3<Sphere_>::DataIterator data_it3 = box_it3->beginData();

															for (;+data_it3;++data_it3)
															{
																if (cmp(*data_it2,*data_it3) && 
																		(data_it->position-data_it3->position).getLength() < 2 * sphere_rad_ + data_it->radius + data_it3->radius &&
																		(data_it2->position-data_it3->position).getLength() < 2 * sphere_rad_ + data_it2->radius + data_it3->radius)
																{
																	trySphere__(data_it,data_it2,data_it3);
																}
															}
														}
											}
										}
									}
						}
					}

			weedCandidates_();

			layer_++;
		}

		//HACK
		layer_++;
	}

  
  Vector3 BindingPocketProcessor::calculateT_(HashGridBox3<Sphere_>::DataIterator data_it, 
			       HashGridBox3<Sphere_>::DataIterator data_it2)
  {
    Vector3 Rvektor = data_it2->position - data_it->position;

    float Rdistance = Rvektor.getLength();
    
    float Rdsq = Rdistance * Rdistance * 2;
 
    float r1 = data_it->radius + sphere_rad_;
    float r2 = data_it2->radius + sphere_rad_; 

    return (data_it->position + data_it2->position)*0.5 + Rvektor * (r1*r1 - r2*r2)/Rdsq;
  }
  
  bool BindingPocketProcessor::testSphereClash_(Vector3& v)
  {
    HashGridBox3<Sphere_>* box = clayer_grid_.getBox(v);  
    
    Position x_pos,y_pos,z_pos;

    clayer_grid_.getIndices(*box,x_pos,y_pos,z_pos);
  
    Position start_x = x_pos;
    Position start_y = y_pos;
    Position start_z = z_pos;

    if (start_x != 0)
      start_x--;
    
    if (start_y != 0)
      start_y--;
    
    if (start_z != 0)
      start_z--;
  
    /** test for clash with existing spheres
     */
    for (Position x = start_x; x < x_pos + 2 && x < clayer_grid_.getSizeX(); x++)
      for (Position y = start_y; y < y_pos + 2 && y < clayer_grid_.getSizeY(); y++) 
        for (Position z = start_z; z < z_pos + 2 && z < clayer_grid_.getSizeZ(); z++)
	  {
	    HashGridBox3<Sphere_>* box_it = clayer_grid_.getBox(x,y,z);
	    
	    HashGridBox3<Sphere_>::DataIterator data_it = box_it->beginData();

	    for (;+data_it;data_it++)
	      if ((v - data_it->position).getLength() < c_factor_ * 2 * sphere_rad_)
		return false;
	  }
    
    return true;
  }
  
  bool BindingPocketProcessor::testProteinClash_(Vector3& v)
  {
    HashGridBox3<Sphere_>* box = protein_grid_.getBox(v);  
   
    Position x_pos,y_pos,z_pos;
 
    protein_grid_.getIndices(*box,x_pos,y_pos,z_pos);
  
    Position start_x = x_pos;
    Position start_y = y_pos;
    Position start_z = z_pos;

    if (start_x != 0)
      start_x--;
    
    if (start_y != 0)
      start_y--;
    
    if (start_z != 0)
      start_z--;
    
    /** test for protein clash
     */
    for (Position x = start_x; x < x_pos + 2 && x < protein_grid_.getSizeX(); x++)
      for (Position y = start_y; y < y_pos + 2 && y < protein_grid_.getSizeY(); y++) 
        for (Position z = start_z; z < z_pos + 2 && z < protein_grid_.getSizeZ(); z++)
	  {
	    HashGridBox3<Sphere_>* box_it = protein_grid_.getBox(x,y,z);
	    
	    HashGridBox3<Sphere_>::DataIterator data_it = box_it->beginData();

	    for (;+data_it;data_it++)
	      if ((v - data_it->position).getLength() < c_factor_ * (protein_clash_rad_ + data_it->radius))
					return false;
	  } 
    
    return true;
  }
    
  bool BindingPocketProcessor::testBC_(Sphere_& sp)
  {
        
    HashGridBox3<Sphere_>* box = BC_grid_.getBox(sp.position);  
    
    Position BC = 0;
 
    Position x_pos,y_pos,z_pos;
    
    BC_grid_.getIndices(*box,x_pos,y_pos,z_pos);
  
    Position start_x = x_pos;
    Position start_y = y_pos;
    Position start_z = z_pos;

    if (start_x != 0)
      start_x--;
    
    if (start_y != 0)
      start_y--;
    
    if (start_z != 0)
      start_z--;
    
    static double rbc = options.getReal(Option::BURIAL_COUNT_RADIUS);

    /** compute burial count
     */
    for (Position x = start_x; x < x_pos + 2 && x < BC_grid_.getSizeX(); x++)
		{
      for (Position y = start_y; y < y_pos + 2 && y < BC_grid_.getSizeY(); y++) 
			{
        for (Position z = start_z; z < z_pos + 2 && z < BC_grid_.getSizeZ(); z++)
				{
					HashGridBox3<Sphere_>* box_it = BC_grid_.getBox(x,y,z);

					HashGridBox3<Sphere_>::DataIterator data_it_prot = box_it->beginData();

					for (;+data_it_prot;data_it_prot++)
					{
						if ((sp.position - data_it_prot->position).getLength() < rbc)
						{
							BC++;
						}    

					}
				}
			}
		}

    if (BC < BC_threshold_)
      return false;
    
    sp.b_count = BC;
    
    return true;
  }  
  

  
  
  void BindingPocketProcessor::trySphere__(HashGridBox3<Sphere_>::DataIterator data_it, 
			    HashGridBox3<Sphere_>::DataIterator data_it2, 
			    HashGridBox3<Sphere_>::DataIterator data_it3)
	{

		/** geometric computations according to pass paper
		 */
		Vector3 z = (data_it2->position - data_it->position)%(data_it3->position - data_it->position); 
		z.normalize();

		Vector3 y = z % (data_it2->position - data_it->position); 
		y.normalize();

		Vector3 Tik = calculateT_(data_it,data_it3);
		Vector3 Tij = calculateT_(data_it,data_it2);

		Vector3 U = y*((Tik -Tij)*(Tik - data_it->position))/((Tik - data_it->position)*y);

		Vector3 Rb = Tij  + U;

		float h = (data_it->radius + sphere_rad_) * (data_it->radius + sphere_rad_) - (Rb - data_it->position).getLength() * (Rb - data_it->position).getLength(); 

		if (h < 0.0)
			return;

		h = sqrt(h);

		Vector3 pos1 = Rb + z*h;
		Vector3 pos2 = Rb - z*h;

		/** test first sphere for sphere- and protein-clash
		 */ 
		if (testSphereClash_(pos1) && testProteinClash_(pos1))
		{
			Sphere_ tmp;
			tmp.position = pos1;
			tmp.layer = layer_;

			tmp.a = data_it->id;
			tmp.b = data_it2->id;
			tmp.c = data_it3->id;
			tmp.d = 0;

			/** test for burial count
			 */
			if (testBC_(tmp))
			{
				candidates.push_back(tmp);
				candidates_grid_.insert(pos1,tmp);
			}
		}

		/** same for second sphere
		 */
		if (testSphereClash_(pos2) && testProteinClash_(pos2))
		{
			Sphere_ tmp;
			tmp.position = pos2;
			tmp.layer = layer_;

			tmp.a = data_it->id;
			tmp.b = data_it2->id;
			tmp.c = data_it3->id;
			tmp.d = 1;	

			if (testBC_(tmp))
			{
				candidates.push_back(tmp);
				candidates_grid_.insert(pos2,tmp);
			}
		}
	}
  

  
  void BindingPocketProcessor::weedCandidates_()
	{

		sort(candidates.begin(),candidates.end(),SphereComparator_());

		/** remove redundant candidates
		 */
		for (Position c = 0; c < candidates.size(); ++c)
		{

			bool insert = true;

			HashGridBox3<Sphere_>* box = candidates_grid_.getBox(candidates[c].position);//->beginBox();

			Position x_pos,y_pos,z_pos;

			candidates_grid_.getIndices(*box,x_pos,y_pos,z_pos);

			Position start_x = x_pos;
			Position start_y = y_pos;
			Position start_z = z_pos;

			if (start_x != 0)
				start_x--;

			if (start_y != 0)
				start_y--;

			if (start_z != 0)
				start_z--;

			static double rweed = options.getReal(Option::PROBE_LAYER_RADIUS);
			

			for (Position x = start_x; x < x_pos + 2 && x < candidates_grid_.getSizeX(); x++)
				for (Position y = start_y; y < y_pos + 2 && y < candidates_grid_.getSizeY(); y++) 
					for (Position z = start_z; z < z_pos + 2 && z < candidates_grid_.getSizeZ(); z++)
					{
						HashGridBox3<Sphere_>* box_it = candidates_grid_.getBox(x,y,z);

						HashGridBox3<Sphere_>::DataIterator data_it = box_it->beginData();

						for (;+data_it; data_it++)
						{
							if ((candidates[c].position - data_it->position).getLength() < rweed)
							{
								if (candidates[c] == *data_it)
									continue;

								insert = false;

								goto exit_loops;
							}
						}
					}

exit_loops:

			if (insert)
			{
				/** insert new sphere
				 */
				candidates[c].radius = second_layer_rad_;
				candidates[c].id = Sphere_::global_id++;
				found_new_++;
				layer_grid_.insert(candidates[c].position,candidates[c]);
				clayer_grid_.insert(candidates[c].position,candidates[c]);
				pw_grid_.insert(candidates[c].position,candidates[c]);

			}	
			else
			{
				candidates_grid_.remove(candidates[c].position,candidates[c]);

			}
		}

		candidates.clear();
		candidates_grid_.clear();
	}
  

  
  // TODO: give filename through options or constructor
  bool BindingPocketProcessor::parse_()
	{
	  return options.readOptionFile(options.get(Option::INI_FILE_NAME));
	}
  
  int BindingPocketProcessor::Sphere_::global_id(0);
  
  BindingPocketProcessor::Sphere_::Sphere_()
  {
  }
}
