#ifndef _FIFLFS_H
#define _FIFLFS_H

#include "FI.hpp"
#include "FL.hpp"
#include "FS.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <utility>
#include <string>
#include <algorithm>
#include <set>
#include <iterator>

namespace OpenVIII::Archive {
    struct FIFLFS {
    private:
        std::filesystem::path fi_{""};
        std::filesystem::path fs_{""};
        std::filesystem::path fl_{""};
        size_t fiOffset_{0U};
        size_t fsOffset_{0U};
        size_t flOffset_{0U};
        size_t count_{0U};
    public:
        [[maybe_unused]] [[nodiscard]] auto FI() const {
            return fi_;
        }

        [[maybe_unused]] [[nodiscard]] auto FS() const {
            return fs_;
        }

        [[maybe_unused]] [[nodiscard]] auto FL() const {
            return fl_;
        }

        void FI(const std::filesystem::path &fi, const size_t &offset = 0U, size_t size = 0U) {
            fi_ = fi;
            fiOffset_ = offset;
            if (size == 0U) {
                if (std::filesystem::exists(fi_)) {
                    size = std::filesystem::file_size(fi_);
                }
            }
            count_ = FI::GetCount(size);
        }

        void FS(const std::filesystem::path &fs, const size_t &offset = 0U) {
            fs_ = fs;
            fsOffset_ = offset;
        }

        void FL(const std::filesystem::path &fl, const size_t &offset = 0U) {
            fl_ = fl;
            flOffset_ = offset;
        }

        [[nodiscard]] bool AllSet() const {
            return (fi_ != "") && (fs_ != "") && (fl_ != "");
        }

        FIFLFS() = default;

        FIFLFS(const std::filesystem::path &fi, const std::filesystem::path &fs, const std::filesystem::path &fl,
               const size_t &fiOffset = 0U, const size_t &fsOffset = 0U, const size_t &flOffset = 0U) {
            fi_ = fi;
            fs_ = fs;
            fl_ = fl;
            fiOffset_ = fiOffset;
            flOffset_ = flOffset;
            fsOffset_ = fsOffset;
        }

        [[nodiscard]] friend std::ostream &operator<<(std::ostream &os, const FIFLFS &data) {
            os << '{' << data.fi_ << ", " << data.fl_ << ", " << data.fs_ << '}';
            return os;
        }

//
//		[[nodiscard]] constexpr auto Count()
//		{
//			return count_;
//		}
        [[nodiscard]] auto GetEntry(const unsigned int &id) const {
            auto fi = Archive::FI();
            if (id < count_ || count_ == 0) {
                fi = FI::GetEntry(fi_, id, fiOffset_);
            }
            return FS::GetEntry(fs_, fi, fsOffset_);
        }

        [[maybe_unused]] [[nodiscard]] auto GetEntry(const std::string_view &needle) const {
            auto found = FL::GetEntry(fl_, needle, fiOffset_);
            const auto&[id, path] = found;
            return GetEntry(id);
        }

        void Test() const {
            const auto allFL = Archive::FL::GetAllEntries(fl_, flOffset_);


            std::filesystem::path path;
            std::filesystem::path dir;
            std::filesystem::path filename;
            std::basic_ofstream<char> fp;
            for (const auto &item : allFL) {
                const auto &[id, strPath] = item;
                //strPath.replace(strPath.begin(),strPath.end(),'\\','/');

                auto buffer = GetEntry(id);

                const auto writeBuffer = [&fp, &buffer]() {
                    auto tmp = std::vector<char>(buffer.size());
                    memcpy(tmp.data(), buffer.data(), buffer.size());
                    fp.write(tmp.data(), buffer.size());
                    //todo in cpp20 use bitcast instead. or find another way to write data.
                };
                const auto &size{buffer.size()};
                std::cout << '{' << id << ", " << size << ", " << strPath << "}\n";
                if (size == 0) {
                    continue;
                }
                if (buffer.empty()) {
                    continue;
                }
                path = strPath;
                dir = std::filesystem::path("tmp");
                filename = dir / path;
                std::filesystem::create_directories(filename.parent_path());
                fp = std::ofstream(filename, std::ios::out | std::ios::binary | std::ios::trunc);
                if (fp.is_open()) {
                    writeBuffer();
                }
                fp.close();
            }
        }

        //order must match switch below
        static constexpr const std::array<std::string_view, 3> extensions = {OpenVIII::Archive::FL::Ext,
                                                                             OpenVIII::Archive::FS::Ext,
                                                                             OpenVIII::Archive::FI::Ext};

        constexpr static bool
        AddFileToFIFLFSByExtension(FIFLFS &archive, const std::filesystem::directory_entry &fileEntry) {

            if (fileEntry.path().has_extension()) {
                unsigned char i = 0;
                for (const auto &ext : extensions) {
                    if (OpenVIII::Tools::iEquals(fileEntry.path().extension().string(), ext)) {
                        switch (i) {
                            case 0:
                                archive.FL(fileEntry);
                                return true;
                            case 1:
                                archive.FS(fileEntry);
                                return true;
                            case 2:
                                archive.FI(fileEntry);
                                return true;
                            default:
                                break;
                        }

                        break;
                    }
                    i++;
                }
            }
            return false;
        }

//todo move get files to here
        using FIFLFSmap = std::map<std::string, OpenVIII::Archive::FIFLFS>;

        static FIFLFSmap GetFilesFromPath(const std::string_view path) {
            const std::filesystem::directory_options options = std::filesystem::directory_options::skip_permission_denied;
            return GetFilesFromIterator(std::filesystem::directory_iterator(path, options)); //todo may need sorted.
        }

        template<class iter_t>
        static FIFLFSmap GetFilesFromIterator(iter_t iter) {
//            static_assert(!std::is_same<typename std::iterator_traits<iter_t>::value_type,
//                    void>::value, "not an iterator!");
            auto tmp = FIFLFSmap();
            auto archive = OpenVIII::Archive::FIFLFS();
            for (const auto &fileEntry : iter) {
                if (AddFileToFIFLFSByExtension(archive, fileEntry)) {
                    if (archive.AllSet()) { //todo confirm basename matches right now i'm assuming the 3 files are together.
                        //todo check for language codes to choose correct files
                        auto name = fileEntry.path().filename().stem().string();
                        std::transform(name.begin(), name.end(), name.begin(), ::toupper);
                        tmp.insert(std::make_pair(name, archive));
                        archive = OpenVIII::Archive::FIFLFS();
                    }
                }
            }
            return tmp;
        }

        static void testPAIR(const std::pair<std::string_view, OpenVIII::Archive::FIFLFS> &pair) {
            const auto&[name, paths] = pair;
            std::cout << name << " " << paths << '\n';
            paths.Test();
            //testFLPath(paths.FL(),paths.FI());
        }
        //todo make getfiles allow recursive archives
    };
}
#endif // !_FIFLFS_H
