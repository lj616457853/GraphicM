#include <ShaderMap.h>


ShaderPSO::ShaderPSO(int id,string path):ID(id) {
	this->spath = path;
	attributeMap = new HashTable<string, std::vector<Attribute>>(100);

}
void ShaderPSO::analysiscode(string code) {
	string gcode = code;
	gcode = gcode.substr(0, gcode.find("void"));
	std::istringstream iss(gcode);
	std::string line;
	while (std::getline(iss, line)) {
		if (line == "") {
			continue;
		}
		int versionindex = line.find("version");
		if (versionindex > 0 && versionindex < line.length()) {
			std::vector<string> params = splitString(line);
			this->version = params[1];
			std::cout << "currtent version:" << params[1] << endl;

			continue;
		}
		int start = line.find(";");
		//get type
		string	basecode = line;
		//
		std::vector<string> slots = splitString(basecode);
		string type = slots[0];

		//get ()
		string indexparam = basecode.substr(basecode.find("(") + 1);
		indexparam = indexparam.substr(0, indexparam.find(")"));
		std::vector<string> inexsarray = splitString(indexparam);
		// after )
		string paramsstr = basecode.substr(basecode.find(")") + 1);
		// unifrom/in/out   mat/float    proname size
		std::vector<string> paramsarr = splitString(paramsstr);
		if (start > 0 && start < line.length()) {
			analysisProperties(line, type, inexsarray, paramsarr);
		}
		else {
			analysisAttribute(line, type, inexsarray, paramsarr, iss);
		}
	}
}

void ShaderPSO::analysisProperties(string &line, string &type, std::vector<string> &inexsarray, std::vector<string> &paramsarr) {
		//type:Layout /Uniform /in-out
		paramsarr[2] = paramsarr[2].substr(0, paramsarr[2].find(";"));
		string size = "1";
		int sx = paramsarr[2].find("[");
		if (sx > 0 && sx < paramsarr[2].length()) {
			string nu = paramsarr[2].substr(paramsarr[2].find("[") + 1);
			nu = nu.substr(0, nu.find("]"));
			size = nu;
			paramsarr[2] = paramsarr[2].substr(paramsarr[2].find("[") + 1, paramsarr[2].find("]"));
		}

		Property param = { paramsarr[0],paramsarr[1],paramsarr[2],size };

		if (type == "layout") {
			
			if (inexsarray[0] == "location") {
				Layout lyout = { stoi(inexsarray[2]) ,param };
				if (paramsarr[0] == "in") {
					std::cout << "loaction in:" << inexsarray[2] << "\n" << endl;
					//inLocation->add(stoi(inexsarray[2]), param);
					inlocation.push_back(lyout);
					std::cout << "paramsarr0:" << paramsarr[0] << "\t paramsarr1:" << paramsarr[1] << "\t paramsarr2:" << paramsarr[2] << endl;
				}
				else {
					std::cout << "loaction out:" << inexsarray[2] << "\n" << endl;
					//outLocation->add(stoi(inexsarray[2]), param);
					outlocation.push_back(lyout);
					std::cout << "paramsarr0:" << paramsarr[0] << "\t paramsarr1:" << paramsarr[1] << "\t paramsarr2:" << paramsarr[2] << endl;

				}
			}
			else if (inexsarray[0] == "binding") {
				Layout lyout = { stoi(inexsarray[2]) ,param };
				std::cout << "binding:" << inexsarray[2] << "\n" << endl;
				//binding->add(stoi(inexsarray[2]), param);
				binding.push_back(lyout);
				std::cout << "paramsarr0:" << paramsarr[0] << "\t paramsarr1:" << paramsarr[1] << "\t paramsarr2:" << paramsarr[2] << endl;

			}
			else if (inexsarray[0] == "push_constant") {
				std::cout << "push_constant:\n" << endl;
				std::cout << "paramsarr0:" << paramsarr[0] << "\t paramsarr1:" << paramsarr[1] << "\t paramsarr2:" << paramsarr[2] << endl;

				pushconstant.push_back(param);
			}

		}
		else if (type == "uniform") {
			std::cout << "uniform:\n" << endl;
			std::cout << "paramsarr0:" << paramsarr[0] << "\t paramsarr1:" << paramsarr[1] << "\t paramsarr2:" << paramsarr[2] << endl;

			uniforms.push_back(param);

		}

	}

void ShaderPSO::analysisAttribute(string& line, string& type, std::vector<string>& inexsarray, std::vector<string>& paramsarr, std::istringstream& iss) {
	//TO DO : resovle struct map
	int begins = line.find("{");
	while (begins < 0) {
		std::getline(iss, line);
		begins = line.find("{");
	}
	int ends = line.find("}");
	bool foundend = false;
	bool shouldend = false;
	while (ends < 0 && !shouldend) {
		int subdx = line.find(";");
		if (subdx < 0) {


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
					if (end != "") {
						Property param = { paramsarr[0],paramsarr[1],end,size };
						if (type == "layout") {
							if (inexsarray[0] == "location") {
								Layout lyout = { stoi(inexsarray[2]) ,param };
								if (paramsarr[0] == "in") {
									//inLocation->add(stoi(inexsarray[2]), param);
									inlocation.push_back(lyout);
								}
								else {
									//	outLocation->add(stoi(inexsarray[2]), param);
									outlocation.push_back(lyout);
								}
							}
							else if (inexsarray[0] == "binding") {
								Layout lyout = { stoi(inexsarray[2]) ,param };
								//binding->add(stoi(inexsarray[2]), param);
								binding.push_back(lyout);
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
		Attribute subp = { subparam[0],subparam[1] };
		std::cout << paramsarr[1] << ":\t" << subparam[0] << ":" << subparam[1] << endl;

		std::vector<Attribute> params = *attributeMap->get(paramsarr[1]);
		
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

				//sProperty param = { paramsarr[0],paramsarr[1],end,size };
				if (end != "") {
					Property param = { paramsarr[0],paramsarr[1],end,size };
					if (type == "layout") {
						if (inexsarray[0] == "location") {
							Layout lyout = { stoi(inexsarray[2]) ,param };
							if (paramsarr[0] == "in") {
								//inLocation->add(stoi(inexsarray[2]), param);
								inlocation.push_back(lyout);

							}
							else {
								//outLocation->add(stoi(inexsarray[2]), param);
								outlocation.push_back(lyout);
							}
						}
						else if (inexsarray[0] == "binding") {
							Layout lyout = { stoi(inexsarray[2]) ,param };
							//binding->add(stoi(inexsarray[2]), param);
							binding.push_back(lyout);
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
	
	



