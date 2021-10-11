#include <string>
struct FileInfo
{
public:
	std::string path = 0;
	std::streamsize size = 0;
	long processTime = 0;
	char* pszData = 0;
};