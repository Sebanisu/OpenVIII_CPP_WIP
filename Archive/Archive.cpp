// Archive.cpp : Defines the entry point for the application.
//

#include "Archive.hpp"
#include "FI.hpp"
#include "FL.hpp"
#include "FS.hpp"
#include <filesystem>
#include <map>
#include <string_view>
#include <algorithm>
#include "FIFLFS.hpp"



/*void testFI(const OpenVIII::FI& fi)
{
	std::cout << fi << '\n';
}
void testFIPath(const std::filesystem::path&, const int&);*/
/*void testFL(const std::pair<int,std::string_view>& fl, const std::filesystem::path& fipath = "")
{
	const auto [id, path] = fl;
	std::cout << '{' << id << ", " << path << '}' << '\n';
	if (fipath != "")
	{
		testFIPath(fipath, id);
	}
}*/
/*
void testFIPath(const std::filesystem::path& path, const int & id = -1)
{
	//const auto count = std::filesystem::file_size(path) / OpenVIII::FI::Size;
	//std::cout << path << " " << count << '\n';
	
	if (id > -1)
	{
		const auto FI = OpenVIII::FI::GetEntry(path,id,0);
		testFI(FI);
	}
	else
	{
		const auto allFI = OpenVIII::FI::GetAllEntries(path);
		std::for_each(allFI.begin(), allFI.end(), &testFI);
	}
}
*/

/*void testFLPath(const std::filesystem::path& path, const std::filesystem::path& fipath)
{

	std::cout << path << " ";
	const auto allFL = OpenVIII::FL::GetAllEntries(path, 0);
	std::cout << allFL.size() << '\n';

	const auto lam = [fipath](const std::pair<int, std::string_view>& fl)
	{
		testFL(fl, fipath);
	};
	std::for_each(allFL.begin(), allFL.end(), lam);
}*/



int main() {

    std::string path = R"(C:\Program Files (x86)\Steam\steamapps\common\FINAL FANTASY VIII\Data\lang-en)";
    OpenVIII::Tools::replaceAll(path, '\\', std::filesystem::path::preferred_separator);

    if (!std::filesystem::exists(path)) {
        return 0;
    }
    const auto files = OpenVIII::Archive::FIFLFS::GetFiles(path);
    std::for_each(files.begin(), files.end(), &OpenVIII::Archive::FIFLFS::testPAIR);
    return 0;
}
