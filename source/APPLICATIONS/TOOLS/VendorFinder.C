// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/molDB.h>
#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/DATATYPE/options.h>
#include "version.h"

using namespace BALL;
using namespace std;

void optimizePurchase(list<pair<Size, list<MolDB::VendorInfo> > >& result_list, map<String, double> & prices, ostream& out);

int main(int argc, char* argv[])
{
	CommandlineParser parpars("VendorFinder", "search vendors for compounds", VERSION, String(__DATE__), "Convert, combine and store");
	parpars.registerParameter("i", "input file containing compounds", INFILE, true);
	parpars.registerParameter("d", "database name", STRING, true);
	parpars.registerParameter("u", "database username", STRING, true);
	parpars.registerParameter("host", "database host", STRING, true);
	parpars.registerParameter("port", "database port", BALL::INT, false, "3306");
	parpars.registerParameter("p", "database password", STRING, true);
	parpars.registerParameter("o", "output text file", OUTFILE);
	parpars.registerFlag("opt", "optimize purchase, i.e. select cheapest sources and sort by vendor");
	String man = "This tool can be used to fetch information about vendors for each compound in the given input file from a data base.\n\nOf course, vendors can only be found if they have been stored in the database that is to be used here. Use DBImporter with molecules files obtained from the compound vendors of your choice in order to create such a database.\n\nOutput of this tool is a text-file containing a list of vendor-name and vendor's compound-ID for each compound in the input file for which vendor-information was found in the database.";
	parpars.setToolManual(man);
	parpars.setSupportedFormats("i",MolFileFactory::getSupportedFormats());
	parpars.setSupportedFormats("o","txt");
	parpars.parse(argc, argv);

	String user = parpars.get("u");
	String db_name = parpars.get("d");
	String host = parpars.get("host");
	String password = parpars.get("p");
	int port = 3306;
	if (parpars.get("port") != CommandlineParser::NOT_FOUND)
	{
		port = parpars.get("port").toInt();
	}

	GenericMolFile* input = MolFileFactory::open(parpars.get("i"));
	MolDB db(host, user, password, db_name, port);

	list<pair<Size, list<MolDB::VendorInfo> > > all_vendor_infos;

	bool optimize = (parpars.get("opt")!=CommandlineParser::NOT_FOUND);

	ostream* out = &cout;
	ofstream file_out;
	if (parpars.has("o"))
	{
		file_out.open(parpars.get("o").c_str(), ios::out);
		out = &file_out;
	}

	Size rank = 1;
	if (!optimize)
	{
		*out <<"#rank   database-ID   vendor-name   vendor's compound-ID   MW"<<endl;
		*out <<"-----------------------------------------------------------------"<<endl;
	}
	for (Molecule* mol = input->read(); mol; mol = input->read(), rank++)
	{
		list<MolDB::VendorInfo> vendor_info;
		db.fetchVendorInformation(mol, vendor_info);

		if (!optimize)
		{
			for (list < MolDB::VendorInfo > ::iterator it = vendor_info.begin(); it != vendor_info.end(); it++)
			{
				*out <<"#"<<rank<<"   "<<it->database_compound_ID<<"   "<<it->vendor_name<<"   "<<it->vendors_compound_ID<<"   "<<it->molecular_weight<<endl;
			}
		}
		else
		{
			all_vendor_infos.push_back(make_pair(rank, vendor_info));
		}
		delete mol;
	}

	if (optimize)
	{
		map<String, double> prices;
		prices.insert(make_pair("Asinex", 29));
		prices.insert(make_pair("ChemBridge", 38));
		prices.insert(make_pair("Pharmeks", 54));
		prices.insert(make_pair("ChemDiv", 60));
		prices.insert(make_pair("Enamine", 66));
		prices.insert(make_pair("InterBioScreen", 70));
		prices.insert(make_pair("TimTec", 115));
		prices.insert(make_pair("Ambinter", 150));
		optimizePurchase(all_vendor_infos, prices, *out);
	}

	delete input;
}



void optimizePurchase(list<pair<Size, list<MolDB::VendorInfo> > >& result_list, map<String, double> & prices, ostream& out)
{
	map<String, list<pair<MolDB::VendorInfo, Size> > > selected_vendors;
	set<String> cmp_set;

	/// Find minimal price for each compound
	for (list < pair < Size, list < MolDB::VendorInfo > > > ::iterator it1 = result_list.begin(); it1 != result_list.end(); it1++)
	{
		const Size& rank = it1->first;

		list<MolDB::VendorInfo>::iterator bestprice_it;
		double min = 100000;
		for (list < MolDB::VendorInfo > ::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++)
		{
			map<String, double>::iterator price_it = prices.find(it2->vendor_name);
			if (price_it != prices.end() && price_it->second < min)
			{
				min = price_it->second;
				bestprice_it = it2;
			}
		}

		// prevent multiple buying of one and the same compound
		if (cmp_set.find(bestprice_it->database_compound_ID) == cmp_set.end())
		{
			map<String, list<pair<MolDB::VendorInfo, Size> > >::iterator search_it = selected_vendors.find(bestprice_it->vendor_name);

			if (search_it == selected_vendors.end())
			{
				list<pair<MolDB::VendorInfo, Size> > vlist;
				vlist.push_back(make_pair(*bestprice_it, rank));
				selected_vendors.insert(make_pair(bestprice_it->vendor_name, vlist));
			}
			else
			{
				search_it->second.push_back(make_pair(*bestprice_it, rank));
			}

			cmp_set.insert(bestprice_it->database_compound_ID);
		}
	}

	/// Output information sorted by vendor
	double total_price = 0;
	out<<"#rank   database-ID   vendor-name   vendor's compound-ID   MW"<<endl;
	out<<"-----------------------------------------------------------------"<<endl<<endl;
	for (map < String, list < pair < MolDB::VendorInfo, Size > > > ::iterator it1 = selected_vendors.begin(); it1 != selected_vendors.end(); it1++)
	{
		out<<endl<<"-------------- "<<it1->first<<" ----------------------"<<endl;
		double subtotal_price = 0;

		for (list < pair < MolDB::VendorInfo, Size > > ::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++)
		{
			const Size& rank = it2->second;
			out<<"#"<<rank<<"   "<<it2->first.database_compound_ID<<"   "<<it2->first.vendor_name<<"   "<<it2->first.vendors_compound_ID<<"   "<<it2->first.molecular_weight<<" g/mol"<<endl;

			map<String, double>::iterator price_it = prices.find(it2->first.vendor_name);
			if (price_it != prices.end())
			{
				subtotal_price += price_it->second;
			}
		}
		out<<" approximate price = "<<subtotal_price<<endl;
		total_price += subtotal_price;
	}
	out<<endl<<"========================================="<<endl;
	out<<"approximate total price = "<<total_price<<endl<<endl;
}
