%{
#include <BALL/STRUCTURE/smilesParser.h>

using namespace BALL;

#include "smilesParserParser.h"
#include <string.h>

%}

%option nounput

%%

[0-9]	{
						SmilesParser::state.current_parser->state.char_count++;
						yylval.number = yytext[0] - '0';
						return TK_DIGIT;
}

He|Li|Be|Ne|Na|Mg|Al|Si|Ar|K|Ca|Sc|Ti|V|Cr|Mn|Fe|Co|Ni|Cu|Zn|Ga|Ge|As|Se|Kr|Rb|Sr|Y|Zr|Nb|Mo|Tc|Ru|Rh|Pd|Ag|Cd|In|Sn|Sb|Te|Xe|Cs|Ba|Lu|Hf|Ta|W|Re|Os|Ir|Pt|Au|Hg|Tl|Pb|Bi|Po|At|Rn|Fr|Ra|Lr|Rf|Db|Sg|Bh|Hs|Mt|Uun|Uuu|Uub|Uut|Uuq|Uup|Uuh|Uus|Uuo|La|Ce|Pr|Nd|Pm|Sm|Eu|Gd|Tb|Dy|Ho|Er|Tm|Yb|Ac|Th|Pa|U|Np|Pu|Am|Cm|Bk|Cf|Es|Fm|Md|No {
						SmilesParser::state.current_parser->state.char_count += strlen(yytext);
						yylval.text = yytext;
						return TK_ATOM;
}

b|c|n|o|p|s|f|i|br|cl {
						SmilesParser::state.current_parser->state.char_count += strlen(yytext);
						yylval.text = yytext;
            return TK_ORG_SUBSET_ATOM;
}

B|C|N|O|P|S|F|I|Br|Cl {
						SmilesParser::state.current_parser->state.char_count += strlen(yytext);
						yylval.text = yytext;
            return TK_ORG_SUBSET_ATOM;
}

@TH {
						SmilesParser::state.current_parser->state.char_count += 2;
						return TK_CHIRAL_CLASS_TH;
}

@AL {
						SmilesParser::state.current_parser->state.char_count += 2;
						return TK_CHIRAL_CLASS_AL;
}

@SP {
						SmilesParser::state.current_parser->state.char_count += 2;
						return TK_CHIRAL_CLASS_SP;
}

@TB {
						SmilesParser::state.current_parser->state.char_count += 2;
						return TK_CHIRAL_CLASS_TB;
}

@OH {
						SmilesParser::state.current_parser->state.char_count += 2;
						return TK_CHIRAL_CLASS_OH;
}

H	{
						SmilesParser::state.current_parser->state.char_count++;
            return TK_HYDROGEN;
}

-|=|\+|#|:|\.|@|\[|\]|\/|\\|\(|\)	{
						SmilesParser::state.current_parser->state.char_count++;
						return yytext[0];
}

.	{
						SmilesParser::state.current_parser->state.char_count++;
						throw Exception::ParseError
							(__FILE__, __LINE__, 
							 String("unknown symbol '") + yytext[0] + "' at position " + String(SmilesParser::state.current_parser->state.char_count), "");
	
}

%%

YY_BUFFER_STATE buffer;
void initBuffer(const char* buf)
{
	buffer = yy_scan_string(buf);
}

void delBuffer()
{
	yy_delete_buffer(buffer);
}
