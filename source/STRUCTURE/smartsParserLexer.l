%{
#include <BALL/STRUCTURE/smartsParser.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/COMMON/parserDefinitions.h>

using namespace BALL;

#include "smartsParserParser.h"

%}

%option nounput
%option noyywrap

%%

[0-9]	{
						SmartsParser::state.current_parser->state.char_count++;
						SmartsParserlval.number = yytext[0] - '0';
						return TK_DIGIT;
}

He|Li|Be|Ne|Na|Mg|Al|Si|Ar|K|Ca|Sc|Ti|V|Cr|Mn|Fe|Co|Ni|Cu|Zn|Ga|Ge|As|Se|Kr|Rb|Sr|Y|Zr|Nb|Mo|Tc|Ru|Rh|Pd|Ag|Cd|In|Sn|Sb|Te|Xe|Cs|Ba|Lu|Hf|Ta|W|Re|Os|Ir|Pt|Au|Hg|Tl|Pb|Bi|Po|At|Rn|Fr|Ra|Lr|Rf|Db|Sg|Bh|Hs|Mt|Uun|Uuu|Uub|Uut|Uuq|Uup|Uuh|Uus|Uuo|La|Ce|Pr|Nd|Pm|Sm|Eu|Gd|Tb|Dy|Ho|Er|Tm|Yb|Ac|Th|Pa|U|Np|Pu|Am|Cm|Bk|Cf|Es|Fm|Md|No {
						SmartsParser::state.current_parser->state.char_count += strlen(yytext);
						SmartsParserlval.text = yytext;
						return TK_ATOM;
}

br|cl|b|c|n|o|p|s|f|i {
						SmartsParser::state.current_parser->state.char_count += strlen(yytext);
						SmartsParserlval.text = yytext;
            return TK_ORG_SUBSET_ATOM;
}

Cl|Br|B|C|N|O|P|S|F|I {
						SmartsParser::state.current_parser->state.char_count += strlen(yytext);
						SmartsParserlval.text = yytext;
            return TK_ORG_SUBSET_ATOM;
}

A	{					SmartsParser::state.current_parser->state.char_count += 1;
						return TK_ALIPHATIC_ATOM;
}

a	{					SmartsParser::state.current_parser->state.char_count += 1;
						return TK_AROMATIC_ATOM;
}

R	{
						SmartsParser::state.current_parser->state.char_count += 1;
						return TK_ATOM_NUM_RINGS;
}

r	{					
						SmartsParser::state.current_parser->state.char_count += 1;
						return TK_ATOM_RING_SIZE;
}

v {					SmartsParser::state.current_parser->state.char_count += 1;
						return TK_ATOM_VALENCE;
}

X	{					SmartsParser::state.current_parser->state.char_count += 1;
						return TK_ATOM_CONNECTED;
}

x {					SmartsParser::state.current_parser->state.char_count += 1;
						return TK_ATOM_RING_CONNECTED;
}

D	{					SmartsParser::state.current_parser->state.char_count += 1;
						return TK_ATOM_DEGREE;
}

\*	{				SmartsParser::state.current_parser->state.char_count += 1;
						return TK_ANY_ATOM;
}

@TH {
						SmartsParser::state.current_parser->state.char_count += 2;
						return TK_CHIRAL_CLASS_TH;
}

@AL {
						SmartsParser::state.current_parser->state.char_count += 2;
						return TK_CHIRAL_CLASS_AL;
}

@SP {
						SmartsParser::state.current_parser->state.char_count += 2;
						return TK_CHIRAL_CLASS_SP;
}

@TB {
						SmartsParser::state.current_parser->state.char_count += 2;
						return TK_CHIRAL_CLASS_TB;
}

@OH {
						SmartsParser::state.current_parser->state.char_count += 2;
						return TK_CHIRAL_CLASS_OH;
}

H	{
						SmartsParser::state.current_parser->state.char_count++;
            return TK_HYDROGEN_EXPLICIT;
}

h	{
						SmartsParser::state.current_parser->state.char_count++;
						return TK_HYDROGEN_IMPLICIT;
}

\$ {
						SmartsParser::state.current_parser->state.char_count++;
						return TK_RECURSIVE;
}

\-|=|~|:|\\|\/|\\\?|\/\?|@|#|\[|\]|\(|\)|!|\+|\.|,|;|& {
						SmartsParser::state.current_parser->state.char_count += strlen(yytext);
						SmartsParserlval.text = yytext;
						return yytext[0];
}

.	{
						SmartsParser::state.current_parser->state.char_count++;
						throw Exception::ParseError
							(__FILE__, __LINE__, 
							 String("unknown symbol '") + yytext[0] + "' at position " + String(SmartsParser::state.current_parser->state.char_count), "");
	
}

%%

YY_BUFFER_STATE SmartsParser_buffer;
void SmartsParser_initBuffer(const char* buf)
{
	SmartsParser_buffer = SmartsParser_scan_string(buf);
}

void SmartsParser_delBuffer()
{
	SmartsParser_delete_buffer(SmartsParser_buffer);
}

void SmartsParser_destroy()
{
#ifdef BALL_HAS_YYLEX_DESTROY
	SmartsParserlex_destroy();
#endif
}
