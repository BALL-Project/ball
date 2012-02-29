#ifndef BALL_STRUCTURE_MUTATOR_H
#define BALL_STRUCTURE_MUTATOR_H

#include <BALL/COMMON/debug.h>
#include <BALL/COMMON/global.h>

namespace BALL
{
	class String;
	class FragmentDB;

	class BALL_EXPORT Mutator
	{
		public:
			/**
			 * This enum is used in order to finetune the behaviour of the 
			 * mutate method. Currently it only offers control over the automatic
       * use of the optimizer after successfully mutating.
			 */
			enum MutatorOptions { NONE = 0, OPTIMIZE = 1 };

			/**
			 * The default constructor can be used to pass a FragmentDB instance to the
			 * Mutator. If 0 is passed, a default FragmentDB is instanciated once setup()
			 * has been called
			 */
			Mutator(FragmentDB* db = 0);
			virtual ~Mutator();

			/*
			 * Set a new FragmentDB instance that shall be used to obtain
			 * the new bases. If 0 is passed, a default instance will be automatically
			 * created when calling DNAMutator::mutate(). The passed instance will
			 * <b>not</b> be deleted upon destruction.
			 */
			void setFragmentDB(FragmentDB* db);

			/**
			 * Calling this method explicitly circumvents lazy loading of the FragmentDB.
			 * This method has no effect if mutate has already been called or if valid instances
			 * of the fragmentDB has been passed via the constructor.
			 *
			 * @warning If you called setup() and then set the FragmentDB to NULL via setFragmentDB()
			 *          you will need to call setup() again in order to prevent lazy loading.
			 */
			virtual void setup();

			/**
			 * Adds a mutation to the list of scheduled mutations. Note that it is usually
			 * not advisable to conduct too many changes on your structure, as with a
			 * growing number mutations results become more and more unreliable.
			 *
			 * @throw NotInitialized This exception is thrown whenever the Mutator needs
			 *        further information. Usually the Molecule which to mutate needs to
			 *        be supplied.
			 */
			virtual void addMutation(Index i, const String& new_frag_name) = 0;

			/**
			 * Clear the list of scheduled mutations
			 */
			virtual void clearMutations() = 0;

			/**
			 * Apply the list of scheduled mutations.
			 *
			 * @note This will modify your original structure and most likely
			 *       render iterators and pointers invalid.
			 */
			void mutate(MutatorOptions opts = NONE);

			/**
			 * Optimize the structure that was obtained during minimization
			 */
			virtual bool optimize() = 0;

		protected:
			FragmentDB* db_;

			virtual void mutate_impl_(MutatorOptions opts) = 0;

		private:
			bool keep_db_;
			void freeDB_();
	};

#ifndef BALL_DEBUG
# include <BALL/STRUCTURE/mutator.iC>
#endif

}

#endif //BALL_STRUCTURE_MUTATOR_H

