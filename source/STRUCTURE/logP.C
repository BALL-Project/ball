#include <BALL/STRUCTURE/logP.h>

using namespace std;

namespace BALL
{
	LogP::LogP()
	{
		molecular_similarity_ = new MolecularSimilarity("fragments/functionalGroups.smarts");

		addRule("[$([CH4]),$([CH3]C),$([CH2](C)C)]",0.14441);  // C1
		addRule("[$([CH](C)(C)C),$([C](C)(C)(C)C)]",0.00000);  // C2
		addRule("[$([CH3][#7,#8,#15,#16,#9,Cl,Br,I]),$([CH2X4][#7,#8,#15,#16,#9,Cl,Br,I])]",-0.2035);      // C3
		addRule("[$([CH1X4][#7,#8,#15,#16,#9,Cl,Br,I]),$([CH0X4][#7,#8,#15,#16,#9,Cl,Br,I])]",-0.2051);    // C4
		addRule("[C]=[A;!#1;!#6]",-0.2783);  // C5, ali. heteroatom
		addRule("[$([CH2]=C),$([CH1](=C)A),$([CH0](=C)(A)A),$([C](=C)=C)]",0.1551);   // C6
		addRule("[CX2]#A",0.00170);  // C7
		addRule("[CH3]c",0.08452);  // C8
		addRule("[CH3][a;!#1;!#6]",-0.1444);  // C9, aro. heteroatom
		addRule("[CH2X4]a",-0.0516);  // C10
		addRule("[CHX4]a",0.1193);  // C11
		addRule("[CH0X4]a",-0.0967);  // C12
		addRule("[cH0]-[!#6;!#7;!#8;!#16;!#9;!Cl;!Br;!I]",-0.5443);  // C13
			//"[$([c][#5],$([c][#14]),$([c][#15]),$([c][#33]),$([c][#34]),$([c][#50]),$([c][#80])]"  // C13 alternative
		addRule("[c][#9]",0.0000);  // C14
		addRule("[c][#17]",0.2450);  // C15
		addRule("[c][#35]",0.1980);  // C16
		addRule("[c][#53]",0.0000);  // C17
		addRule("[cH]",0.1581);  // C18
		addRule("[c](:a)(:a):a",0.2955);  // C19
		addRule("[c](:a)(:a)-a",0.2713);  // C20
		addRule("[c](:a)(:a)-C",0.1360);  // C21
		addRule("[c](:a)(:a)-N",0.4619);  // C22
		addRule("[c](:a)(:a)-O",0.5437);  // C23
		addRule("[c](:a)(:a)-S",0.1893);  // C24
		addRule("[$([c](:a)(:a)=C),$([c](:a)(:a)=N),$([c](:a)(:a)=O)]",-0.8186);  // C25
		addRule("[$([C](=C)(a)A),$([C](=C)(c)a),$([CH](=C)a),$([C]=c)]",0.2640);  // C26
		addRule("[CX4][!#5;!#7;!#8;!#15;!#16;!#9;!Cl;!Br;!I]",0.2148);  // C27
		addRule("remaining [C,c]",0.08129);

		addRule("[$([H,h][#6]),$([H,h][H,h])]",0.1230);  // H1, hydrocarbon
		addRule("[$([H,h]O[CX4]),$([H,h]Oc),$([H,h]O[!#6;!#7;!#8;!16]),$([H,h][!#6;!#7;!#8])]",-0.2677);  // H2, alcohol
		addRule("[$([H,h][#7]),$([H,h]O[#7])]",0.2142);  // H3, amine
		addRule("[$([H,h]OC=[#6]),$([H,h]OC=[#7]),$([H,h]OC=O),$([H,h]OC=S),$([H,h]OO),$([H,h]OS)]",0.2980);  // H4, acid
		addRule("remaining [H,h]",0.1125);

		addRule("[NH2+0]A",-1.0190);  // N1
		addRule("[NH+0](A)A",-0.7096);  // N2
		addRule("[NH2+0]a",-1.0270);  // N3
		addRule("[$([NH+0](A)a),$([NH+0](a)a)]",-0.5188);  // N4
		addRule("[$([NH+0]=A),$([NH+0]=a)]",0.08387);  // N5
		addRule("[$([N+0](=A)A),$([N+0](=A)a),$([N+0](=a)A),$([N+0](=a)a)]",0.1836);  // N6
		addRule("[N+0](A)(A)A",-0.3187);  // N7
		addRule("[$([N+0](a)(A)A),$([N+0](a)(a)A),$([N+0](a)(a)a)]",-0.4458);  // N8
		addRule("[N+0]#A",0.01508);  // N9
		addRule("[$([NH3+*]),$([NH2+*]),$([NH+*])]",-1.950);  // N10
		addRule("[n+0]",-0.3239);  // N11
		addRule("[n+*]",-1.119);  // N12
		addRule("[$([NH0+*](A)(A)(A)A),$([NH0+*](=A)(A)A),$([NH0+*](=A)(A)a),$([NH0+*](=[#6])=[#7])]",-0.3396);  // N13
		addRule("[$([N+*]#A),$([N-*]),$([N+*](=[N-*])=N)]",0.2887);  // N14
		addRule("remaining [N,n]",-0.4806);

		addRule("[o]",0.1552);  // O1
		addRule("[$([OH]),$([OH2])]",-0.2893);  // O2, alcohol
		addRule("[$([O](C)C),$([O](C)[A;N,O,P,S,F,Cl,Br,I]),$([O]([A;N,O,P,S,F,Cl,Br,I])[A;N,O,P,S,F,Cl,Br,I])]",-0.0684);  // O3, ali. ether
		addRule("[$([O](A)a),$([O](a)a)]",-0.4195);  // O4, aro. ether
		addRule("[$([O]=[#8]),$([O]=[#7]),$([OX1-*][#7])]",0.0335);  // O5, oxide
		addRule("[OX1-*][#16]",-0.3339);  // O6, oxide
		addRule("[OX1-*][!N;!S]",-1.189);  // O7, oxide
		addRule("[O]=c",0.1788);  // O8, aro. carbonyl
		addRule("[$([O]=[CH]C),$([O]=C(C)C),$([O]=C(C)[A;!#1;!#6]),$([O]=[CH]N),$([O]=[CH]O),$([O]=[CH2]),$([O]=[CX2]=O)]",-0.1525);  // O9, carbonyl ali.
		addRule("[$([O]=[CH]c),$([O]=C(C)c),$([O]=C(c)c),$([O]=C(c)[a;!#1;!#6]),$([O]=C(c)[A;!#1;!#6]),$([O]=C(C)[a;!#1;!#6])]",0.1129);  // O10, carbonyl aro.
		addRule("[$([O]=C([A;!#1;!#6])[A;!#1;!#6]),$([O]=C([A;!#1;!#6])[a;!#1;!#6]),$([O]=C([a;!#1;!#6])[a;!#1;!#6])]",0.4833);  // O11, carbonyl heteroatom
		addRule("[O-1]C(=O)",-1.326);  // O12, acid
		addRule("remaining [O,o]",-0.1188);

		addRule("[#9-0]",0.4202);  // flourine
		addRule("remaining [#9]",-2.996);

		addRule("[#17-0]",0.6895);  // chlorine
		addRule("remaining [#17]",-2.996);

		addRule("[#35-0]",0.8456);  // bromine
		addRule("remaining [#35]",-2.996);

		addRule("[#53-0]",0.8857);  // iodine
		addRule("remaining [#53]",-2.996);

		addRule("[#15]",0.8612);  // phosphorous

		addRule("[S-0]",0.6482); // aliphatic sulfur
		addRule("[$([S-*]),$([S-*])]",-0.0024); // ionic sulfur
		addRule("[s]",0.6237);  // aromatic sulfur

		addRule("[B,Si,Ga,Ge,As,Se,Sn,Te,Pb,Ne,Ar,Kr,Xe,Rn]",-0.3808); // remaining p-block elements

		addRule("[Fe,Cu,Zn,Tc,Cd,Pt,Au,Hg]",-0.0025);  // remaining d-block elements
	}


	LogP::~LogP()
	{
		 delete molecular_similarity_;
	}


	void LogP::addRule(String smarts, double value)
	{
		rules_.push_back(make_pair(smarts,value));
	}


	double LogP::calculate(const String& usmile)
	{
		double logP = 0;
		Size match_sum = 0;

		for(Size i=0; i<rules_.size(); i++)
		{
			if(!rules_[i].first.hasPrefix("remaining"))
			{
				Size matches;
				molecular_similarity_->matchSmarts(usmile,rules_[i].first,matches);
				logP += matches*rules_[i].second;
				match_sum += matches;
//				if(matches>0) cout<<rules_[i].first<<" : "<<matches<<endl;
			}
			else
			{
				Size no_atoms;
				String smarts = rules_[i].first.after("remaining");
				smarts.trim();
				molecular_similarity_->matchSmarts(usmile,smarts,no_atoms);
				if(no_atoms>match_sum)
				{
					logP += (no_atoms-match_sum)*rules_[i].second;
				}

// 				if(match_sum>no_atoms)
// 				{
// 					cout<<"Error: Atoms "<<smarts<<" were matches mulitple times!!"<<endl;
// 					cout<<match_sum<<"  "<<no_atoms<<endl;
// 				}

				match_sum=0;
			}
		}

		return logP;
	}
}
