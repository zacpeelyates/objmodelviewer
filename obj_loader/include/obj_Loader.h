#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H
#include "OBJDatatypes.h"
class OBJLoader {
	public:

		static OBJModel* OBJProcess(const std::string& a_strFilePath, const bool a_bPrintComments);
	protected:
		static bool OBJLoadMaterials(const std::string& a_strFilePath, OBJModel& a_roLoadedData, const bool a_bPrintComments);
		static OBJFace ProcessFace(std::string a_faceData);
		
		
	private:
		static std::map<std::string, OBJModel*> m_modelMap;
	
};
#endif // OBJ_LOADER_H
