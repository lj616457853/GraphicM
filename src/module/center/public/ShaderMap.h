#ifndef  SHADER_MANERGER
#define SHADER_MANERGER
#include <iostream>
#include <vector>
#include <map>
#include <ShaderFileCenter.h>
using namespace std;

struct Attribute
{
	string protype;
	string proname;
};

struct Property
{	
	string type;
	string filed;
	string fname;
	string size;

};
struct Layout
{
	int point;

	Property pro;
};

class ShaderPSO {

private:
	std::vector<Layout> binding;
	
	std::vector<Layout> inlocation;

	std::vector<Layout> outlocation;
	
	HashTable<std::string, std::vector<Attribute>>* attributeMap;


	std::vector<Property> pushconstant;
	std::vector<Property> uniforms;

public:
	int ID;

	string spath;

	string version;

	ShaderPSO(int ID,string path);

	void analysiscode(string code);


	void analysisAttribute(string &line, string &type, std::vector<string> &inexsarray, std::vector<string> &paramsarr, std::istringstream &iss);



	void analysisProperties(string &line, string& type, std::vector<string>& inexsarray, std::vector<string>& paramsarr);

	std::vector<std::string> splitString(const std::string& str) {
		std::vector<std::string> result;
		std::stringstream ss(str);
		std::string token;

		while (ss >> token) {
			result.push_back(token);
		}

		return result;
	}

};



#endif // ! SHADER_MANERGER
