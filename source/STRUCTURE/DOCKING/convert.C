#include "convert.h"
#include <sstream>

using namespace std;

string convert_i(int i)
{
  std::stringstream ss;
  std::string str;
  ss << i;
  ss >> str;
  return str;
}
  
// converts string to int
int convert_s(const std::string& s)
{
  std::stringstream ss;
  int i;
  ss << s;
  ss >> i;
  return(i);
} 


string doubleToString(double data) 
{ 
    stringstream myStream; 

    myStream << data; 

    return myStream.str(); 
} 
