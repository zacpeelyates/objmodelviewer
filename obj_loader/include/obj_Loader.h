#include "OBJDatatypes.h"
#include <fstream>
class OBJLoader {
	public:

		static OBJModel* OBJProcess(const std::string& a_strFilePath,const bool a_bPrintComments);
	protected:
		static bool OBJLoadMaterials(const std::string& a_strFilePath, OBJModel& a_roLoadedData, const bool a_bPrintComments);
		static OBJFace ProcessFace(std::string a_faceData);
		static glm::vec4 OBJGetVectorFromValue(const std::string a_strValue);
		static bool OBJGetKeyValuePairFromLine(const std::string& a_rStrLine, std::string& a_rStrOutKey, std::string& a_rStrOutValue);
		
	private:
		OBJLoader();
		~OBJLoader();
		std::map<std::string, OBJModel*> m_modelMap;
	
};
