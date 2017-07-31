class RepresentationManager
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/representationManager.h>
	#include <list>
	using std::list;
%End
	public:

	RepresentationManager(MainControl* = 0) throw();
	RepresentationManager(const RepresentationManager&) throw();

	bool operator == (const RepresentationManager&) const throw();
	void clear() throw();
	void remove(Representation&, bool = true) throw();
	void insert(Representation&, bool = true) throw();
	const list<Representation*>& getRepresentations() const throw();
	Size getNumberOfRepresentations() const throw();
	Representation* createRepresentation() throw();
	bool has(const Representation&) const throw();

	list<Representation*> removedComposite(const Composite&, bool = true) throw();
	list<Representation*> getRepresentationsOf(const Composite&) throw();
	void rebuildAllRepresentations() throw();
	bool removeClippingPlane(ClippingPlane*);
	void insertClippingPlane(ClippingPlane*);
	void storeRepresentations(INIFile&);
	void focusRepresentation(const Representation&);
	bool willBeUpdated(const Representation&) const throw();
	bool updateRunning() const throw();
	bool isBeeingRendered(const Representation*) const;
	bool startRendering(Representation*);
	void finishedRendering(Representation*);
};
