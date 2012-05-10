// ----------------------------------------------------
// $Maintainer: Marc Röttig $
// $Authors: Marc Röttig, Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_DOCKING_RESULT_H
#define BALL_DOCKING_RESULT_H

#include <BALL/DOCKING/COMMON/receptor.h>
#include <BALL/DOCKING/COMMON/flexibleMolecule.h>
#include <vector>
#include <map>


namespace BALL
{
	/**     Result class.
			  This class represents a result that occur within virtual
			  screening workflows, like docking results or rescoring results.
	 */
	class BALL_EXPORT Result
	{
		public:

			enum BALL_EXPORT Method
			{
				UNKNOWN = -1,
				RECEPTORIMPORT,
				LIGANDIMPORT,
				DOCKING,
				RESCORING,
				MDRESCORING,
				CONSENSUSRESCORING,
				MOLECULE_CHECK
			};

			class BALL_EXPORT ResultData
			{
				public:
					ResultData();
					ResultData(int result_type);
					void setReceptorConformationId(String id);
					void setReceptorConformation(Conformation* conf);
					bool hasReceptorConformationId() const;
					String  getReceptorConformationId() const;
					void setLigandConformationId(String id);
					void setLigandConformation(Conformation* conf);
					String  getLigandConformationId() const;
					bool hasLigandConformationId() const;
					void setScoringMethod(int method);
					bool hasScoringMethod() const;
					int  getScoringMethod() const;
					void setEnergy(double e);
					double getEnergy() const;
					bool hasEnergy() const;
					bool check() const;
					void setConformation(Conformation* conf);
					Conformation* getConformation();
				private:
					Conformation* conformation;
					bool    has_rec_id;
					bool    has_lig_id;
					bool    has_energy;
					bool    has_method;
					String     rec_conf_id;
					String     lig_conf_id;
					int     scoringmethod;
					double  energy;
					int     resulttype;
			};

			static Result::Method getMethod(int i);
			static Result* makeLigandImportResult();
			static Result* makeReceptorImportResult();
			static Result* makeDockingResult();
			static Result* makeRescoringResult();
			BALL_CREATE(Result)
			Result();

			/* Constructor.
			 */
			Result(const Result::Method& _method);

			/** Copy constructor */
			Result(const Result& res);

			/** Copy constructor that copies only those entries whose IDs can be found in the given HashSet. */
			Result(const Result& res, const HashSet<String>& IDs);

			/* Destructor.
			 */
			virtual ~Result();

			/** Clear all ResultData from this object. \n
			Method information and description will be retained. */
			void clear();

			/** Copy the content of 'res' into this Result object */
			void operator = (const Result& res);

			/** Add the content of 'res' to this Result object */
			void operator += (const Result& res);

			/** Get the Method of the Result.
					@return Method
			 */
			Result::Method getMethod();

			/** Get the Method string of the Result.
					@return Method string
			 */
			String getMethodString();

			/** Set the Method of the Result.
					@param Method
			 */
			void setMethod(Result::Method _method);

			/** Get the list of all input conformation UIDs.
					@return vector of UIDs
			 */
			const vector < String > * getInputConformations();

			/** Get the input conformation of the result for a specific Ligand.
					@param  pointer to Ligand
					@return pointer to input Conformation
			 */
			Conformation* getInputConformer(Ligand* lig) throw(Exception::GeneralException);

			/** Get the first output conformation of the result for a specific Ligand.
					@param  pointer to Ligand
					@return  pointer to first output Conformation
			 */
			Conformation* getFirstOutputConformation(Ligand* lig) throw(Exception::GeneralException);

			/** Get the all output conformation of the result for a specific Ligand.
					@param  pointer to Ligand
					@return  vector of ResultData for each output conformation
			 */
			vector < Result::ResultData > getOutputConformations(Ligand* lig);

			const HashSet<String>* getInputIds();
			HashSet<String> getOutputIds();
			ResultData getOutputData(String id);
			bool hasOutputData(String id);

			/** Add a result entry for a Ligand conformation.
					@param  pointer to Ligand input conformation
					@param  ResultData
			 */
			void add(Conformation* lig_conf_in, ResultData &rd);

			/** Add a result entry for a Ligand conformation.
					@param  pointer to Ligand input conformation
					@param  pointer to Ligand output conformation
					@param  scoring method
					@param  energy
					@param  receptor conformation
			 */
			void add(Conformation* lig_conf_in, Conformation* lig_conf_out, int scoringmethod, double energy, Conformation* rec_conf);

			void add(const String& lig_conf_in, const String& lig_conf_out, int scoringmethod, double energy, const String& rec_conf);

			/** Add a result entry for a Ligand conformation.
					@param  Ligand input conformation String
					@param  ResultData
			 */
			void add(String lig_conf_id_in, Result::ResultData &rd);

			/** Get result entries for a Ligand conformation String.
					@param  Ligand input conformation String
					@return  ResultData
			 */
			const vector<Result::ResultData>* get(String inpose_id);

			static String toString(const ResultData &rd);

			/** Remove the result data for all conformations of the given FlexibleMolecule from this Result object. */
			void erase(const FlexibleMolecule* flexmol);

			void erase(const String& ID);

			void sort(const list<String>& input_order, list<String>& output_order);

			void setTimestamp(const String& timestamp);

			const String& getTimestamp();

			void setDescription(const String& text);

			const String& getDescription();

			void setToolInfo(const String& toolinfo);

			const String& getToolInfo();

			const HashMap<String, vector<ResultData> >* getData();

		protected:

			String timestamp_;
			String toolinfo_;
			Result::Method method;
			HashMap<String, vector<ResultData> >     result_data_;
			HashMap<String, ResultData> result_output_data_;
			vector < String > input_conformations_;
			HashSet < String > input_conformations_map_;

			/** Optional String describing the method or approach that was use to generate this result (e.g. the name of the docking or rescoring algorithm) */
			String description_;
	};
}
#endif /* BALL_DOCKING_RESULT_H */
