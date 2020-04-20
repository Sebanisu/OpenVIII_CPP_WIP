// Archive.cpp : Defines the entry point for the application.
//

#include "Archive.hpp"
#include "FI.hpp"
#include "FL.hpp"
#include "FS.hpp"
#include <filesystem>
#include <array>
#include <map>
#include <string_view>
#include <algorithm>
#include "FIFLFS.hpp"


auto GetFiles(const std::string_view path) {
    //order must match switch below
    static constexpr const std::array<std::string_view, 3> extensions = {OpenVIII::Archive::FL::Ext,
                                                                         OpenVIII::Archive::FS::Ext,
                                                                         OpenVIII::Archive::FI::Ext};
    const std::filesystem::directory_options options = std::filesystem::directory_options::skip_permission_denied;
    auto tmp = std::map<std::string, OpenVIII::Archive::FIFLFS>();
    auto archive = OpenVIII::Archive::FIFLFS();
    for (const auto &fileEntry : std::filesystem::directory_iterator(path, options)) {
        if (fileEntry.is_regular_file()) {
            unsigned char i = 0;
            for (const auto &ext : extensions) {
                if (OpenVIII::Tools::iEquals(fileEntry.path().extension().string(), ext)) {
                    switch (i) {
                        case 0:
                            archive.FL(fileEntry);
                            break;
                        case 1:
                            archive.FS(fileEntry);
                            break;
                        case 2:
                            archive.FI(fileEntry);
                            break;
                        default:
                            break;
                    }

                    if (archive.AllSet()) {
                        auto name = fileEntry.path().filename().stem().string();
                        std::transform(name.begin(), name.end(), name.begin(), ::toupper);
                        tmp.insert(std::make_pair(name, archive));
                        archive = OpenVIII::Archive::FIFLFS();
                    }
                    break;
                }
                i++;
            }
        }
    }
    return tmp;
}
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


void testPAIR(const std::pair<std::string_view, OpenVIII::Archive::FIFLFS> &pair) {
    const auto&[name, paths] = pair;
    std::cout << name << " " << paths << '\n';
    paths.Test();
    //testFLPath(paths.FL(),paths.FI());
}

int main() {

    std::string path = R"(C:\Program Files (x86)\Steam\steamapps\common\FINAL FANTASY VIII\Data\lang-en)";
    OpenVIII::Tools::replaceAll(path, '\\', static_cast<char>(std::filesystem::path::preferred_separator)); //windows uses a wchar_t instead of char.

    if (!std::filesystem::exists(path)) {
        return 0;
    }
    const auto files = GetFiles(path);
    std::for_each(files.begin(), files.end(), &testPAIR);
    return 0;
}
