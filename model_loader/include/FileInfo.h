#include <string>
struct FileInfo
{
public:
<<<<<<< Updated upstream
	std::string path = 0;
=======
	std::string path = "";
>>>>>>> Stashed changes
	std::streamsize size = 0;
	long processTime = 0;
	char* pszData = 0;
};