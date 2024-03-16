#include <iostream>

#include <map1.h>
#include <ShaderFileCenter.h>
#include <filesystem.h>
#include <ShaderMap.h>
static void finder(string code);

static std::vector<std::string> splitString(const std::string& str);
void main()
{
	ShaderFileManager* ma= ShaderFileManager::getShaderManger();
	string code =ma->getShaderFile(FileSystem::getShaderPath("bloom/skybox.vert"));
	//finder(code);

	ShaderPSO *pso = new ShaderPSO(0, FileSystem::getShaderPath("bloom/skybox.vert"));
	pso->analysiscode(code);
	int ix = 1;
}

/*
string fcode = code;
	int bind = 0;
	int lcoa = 0;
	int idx = fcode.find("layout");
	while (idx < fcode.length()&&idx>0)
	{
		fcode = fcode.substr(idx + 6);
		string befor = fcode.substr(0, fcode.find(")"));
		int x = befor.find("location");
		if (x>0) {
			lcoa++;
		}
		int y = befor.find("binding");
		if (y>0) {
			bind++;

		}
		fcode = fcode.substr(fcode.find(")") + 1);
		idx = fcode.find("layout");

	}
	int xx = 1;

*/
static void finder(string code) {
	string gcode = code;
	gcode = gcode.substr(0, gcode.find("void"));
	std::istringstream iss(gcode);

	std::string line;
	struct Param
	{
		string type;
		string proname;
		string name;
		string size;
	};
	struct SubParam {
		string proptype;
		string propname;
	};
	HashTable<std::string, std::vector<SubParam>>* structMap = new HashTable<std::string, std::vector<SubParam>>();
	HashTable<int, Param>* inLocation = new HashTable<int, Param>();
	HashTable<int, Param>* outLocation = new HashTable<int, Param>();
	HashTable<int, Param>* binding = new HashTable<int, Param>();
	std::vector<Param> pushconstant;
	std::vector<Param> uniforms;
	while (std::getline(iss,line)) {
		if (line =="") {
			continue;
		}
		int versionindex = line.find("version");
		if (versionindex>0&&versionindex<line.length()) {
			std::vector<string> params= splitString(line);
			std::cout << "currtent version:" << params[1] << endl;
			continue;
		}
		int start = line.find(";");
		//get type
		string	basecode = line;
		//
		std::vector<string> slots =  splitString(basecode);
		string type = slots[0];
		
	   //get ()
		string indexparam = basecode.substr(basecode.find("(")+1);
		indexparam = indexparam.substr(0,indexparam.find(")"));
		std::vector<string> inexsarray = splitString(indexparam);
	   // after )
		string paramsstr = basecode.substr(basecode.find(")")+1);
		// unifrom/in/out   mat/float    proname size
		std::vector<string> paramsarr = splitString(paramsstr);
		
		if (start>0 && start < line.length()) {
			//type:Layout /Uniform /in-out
			paramsarr[2] = paramsarr[2].substr(0, paramsarr[2].find(";"));
			string size = "1";
			int sx =paramsarr[2].find("[");
			if (sx > 0 && sx < paramsarr[2].length()) {
				string nu = paramsarr[2].substr(paramsarr[2].find("[")+1);
				nu = nu.substr(0,nu.find("]"));
				size = nu;
				paramsarr[2] = paramsarr[2].substr(paramsarr[2].find("[")+1, paramsarr[2].find("]"));
			}

			Param param = {paramsarr[0],paramsarr[1],paramsarr[2],size};
			
			if (type=="layout") {
				if (inexsarray[0]=="location") {
					if (paramsarr[0]=="in") {
						std::cout << "loaction in:" << inexsarray[2] << "\n" << endl;
						inLocation->add(stoi(inexsarray[2]), param);
						std::cout << "paramsarr0:" << paramsarr[0] << "\t paramsarr1:" << paramsarr[1] << "\t paramsarr2:" << paramsarr[2] << endl;
					}
					else {
						std::cout << "loaction out:" << inexsarray[2] << "\n" << endl;
						outLocation->add(stoi(inexsarray[2]), param);
						std::cout << "paramsarr0:" << paramsarr[0] << "\t paramsarr1:" << paramsarr[1] << "\t paramsarr2:" << paramsarr[2] << endl;

					}
				}
				else if (inexsarray[0]=="binding") {
						std::cout << "binding:"<<inexsarray[2]<<"\n" << endl;
						binding->add(stoi(inexsarray[2]),param);
						std::cout << "paramsarr0:" << paramsarr[0] << "\t paramsarr1:" << paramsarr[1] << "\t paramsarr2:" << paramsarr[2] << endl;

				}
				else if (inexsarray[0]=="push_constant") {
						std::cout << "push_constant:\n" << endl;
						std::cout << "paramsarr0:" << paramsarr[0] << "\t paramsarr1:" << paramsarr[1] << "\t paramsarr2:" << paramsarr[2] << endl;

						pushconstant.push_back(param);
				}
			
			}
			else if(type == "uniform") {
				std::cout << "uniform:\n" << endl;
				std::cout << "paramsarr0:" << paramsarr[0] << "\t paramsarr1:" << paramsarr[1] << "\t paramsarr2:" << paramsarr[2] << endl;

				uniforms.push_back(param);
			
			}

			

		}
		else {	
			//TO DO : resovle struct map
			int begins = line.find("{");
			while (begins <0) {
				std::getline(iss, line);
				begins = line.find("{");
			}
			int ends = line.find("}");
			bool foundend = false;
			bool shouldend = false;
			while (ends<0&&!shouldend) {
				int subdx  = line.find(";");
				if (subdx < 0) {
					
					
					std::getline(iss,line);
					if (!foundend) {
						ends = line.find("}");
						if (ends>0 && ends <line.length()) {
							foundend = true;
						}
					}

					if (foundend) {
						int send =line.find(";");
						if (send>0 && send<<line.length()) {
							//type:Layout /Uniform /in-out
							string end = line.substr(0,line.find(";"));
							end = end.substr(end.find("}")+1);
							end = end.substr(0, end.find(";"));
							string size = "1";
							int sx = end.find("[");
							if (sx > 0 && sx < end.length()) {
								string nu = end.substr(end.find("[") + 1);
								nu = nu.substr(0, nu.find("]"));
								size = nu;
								end = end.substr(end.find("[") + 1, end.find("]"));
							}
							if (end!="") {
								Param param = { paramsarr[0],paramsarr[1],end,size };
								if (type == "layout") {
									if (inexsarray[0] == "location") {
										if (paramsarr[0] == "in") {
											inLocation->add(stoi(inexsarray[2]), param);

										}
										else {
											outLocation->add(stoi(inexsarray[2]), param);

										}
									}
									else if (inexsarray[0] == "binding") {
										binding->add(stoi(inexsarray[2]), param);
									}
									else if (inexsarray[0] == "push_constant") {

										pushconstant.push_back(param);
									}

								}
								else if (type == "uniform") {
									uniforms.push_back(param);

								}
							
							}
							
							shouldend = true;
						}
					}
					
					continue;
				}
				string vlauer = line.substr(line.find(";"));
				std::vector<string> subparam = splitString(line);
				SubParam subp = {subparam[0],subparam[1]};
				std::cout << paramsarr[1]<<":\t"  << subparam[0] << ":" << subparam[1] << endl;
				
				 
				std::vector<SubParam> params = *structMap->get(paramsarr[1]);
				params.push_back(subp);
				std::getline(iss, line);
				if (!foundend) {
					ends = line.find("}");
					if (ends > 0 && ends < line.length()) {
						foundend = true;
					}
				}

				if (foundend) {
					int send = line.find(";");
					if (send > 0 && send << line.length()) {
						//type:Layout /Uniform /in-out
						string end = line.substr(0, line.find(";"));
						end = end.substr(end.find("}") + 1);
						end = end.substr(0, end.find(";"));
						string size = "1";
						int sx = end.find("[");
						if (sx > 0 && sx < end.length()) {
							string nu = end.substr(end.find("[") + 1);
							nu = nu.substr(0, nu.find("]"));
							size = nu;
							end = end.substr(end.find("[") + 1, end.find("]"));
						}

						Param param = { paramsarr[0],paramsarr[1],end,size };
						if (end != "") {
							Param param = { paramsarr[0],paramsarr[1],end,size };
							if (type == "layout") {
								if (inexsarray[0] == "location") {
									if (paramsarr[0] == "in") {
										inLocation->add(stoi(inexsarray[2]), param);

									}
									else {
										outLocation->add(stoi(inexsarray[2]), param);

									}
								}
								else if (inexsarray[0] == "binding") {
									binding->add(stoi(inexsarray[2]), param);
								}
								else if (inexsarray[0] == "push_constant") {

									pushconstant.push_back(param);
								}

							}
							else if (type == "uniform") {
								uniforms.push_back(param);

							}

						}
						shouldend = true;
					}
				}
			
			}
		
		}
	
	}

}

static std::vector<std::string> splitString(const std::string& str) {
	std::vector<std::string> result;
	std::stringstream ss(str);
	std::string token;

	while (ss >> token) {
		result.push_back(token);
	}

	return result;
}