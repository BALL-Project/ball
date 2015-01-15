#ifndef FRAGMENTBUILDER_H
#define FRAGMENTBUILDER_H



using namespace OpenBabel;
using namespace BALL;
using namespace std;

class FragmentBuilder{
public:
	FragmentBuilder();
	
	~FragmentBuilder();
	/**
	 *
	 * @brief buildLinker a flexible linker fragment
	 * @param linker_lst
	 */
	void buildLinker(Fragment &linker_frag, ConnectionMap &link_lib);
};



#endif // FRAGMENTBUILDER_H
