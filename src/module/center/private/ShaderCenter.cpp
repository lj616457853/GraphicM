#include <ShaderCenter.h>

ShaderCenter* ShaderCenter::shaderCenter = nullptr;



ShaderCenter::ShaderCenter() {
	shaderById = new HashTable<int, ShaderPSO*>(100);
	shaderByPath = new HashTable<string, ShaderPSO*>(100);
	idByPath = new 	HashTable<string, int>(100);

	filesToPso();

}


void ShaderCenter::filesToPso() {
	ShaderFileManager* manager =  ShaderFileManager::getShaderManger();

	std::vector<string> files = manager->getShaderFiles();
	vector<string>::iterator  path;

	for (path = files.begin(); path < files.end(); path++) {
		string fpath = *path;
		int spvidx = fpath.find(".spv");
		if (spvidx>0&&spvidx<fpath.length()) {
			fpath = fpath.substr(0,spvidx);
		}
		ShaderPSO* pso = *shaderByPath->get(fpath);
		if (pso != nullptr) {
			continue;
		}
		pso = new ShaderPSO(asid,fpath);
		pso->analysiscode(manager->getShaderFile(fpath));
		asid++;
		shaderById->add(asid,pso);
		idByPath->add(fpath,pso->ID);
		psos.push_back(pso);
	}
}
string ShaderCenter::hadlepath(string pathbefore) {
	int spvidx = pathbefore.find(".spv");
	if (spvidx > 0 && spvidx < pathbefore.length()) {
		pathbefore = pathbefore.substr(0, spvidx);
	}
	return pathbefore;

}

int ShaderCenter::getShaderId(string fpath) {
	return idByPath->get(hadlepath(fpath))==nullptr? -1:*idByPath->get(hadlepath(fpath));
}

ShaderPSO* ShaderCenter::getShaderPsoById(int sid) {
	return *shaderById->get(sid);
}

ShaderPSO* ShaderCenter::getShaderPsoByPath(string path) {
	return *shaderByPath->get(hadlepath(path));
}