/**
* \file file_system.h
* \brief Declaration for the file system class.
*/

#ifndef HLMDLVIEWER_FILE_SYSTEM_H_
#define HLMDLVIEWER_FILE_SYSTEM_H_

#include <list>
#include <fstream>

namespace hl_mdlviewer {

/** \brief A class that can perform various IO file operations. */
class FileSystem
{
public:
    FileSystem();

    enum SearchFlags
    {
        None = 0,
        Recursive = 1, // Allow search in sub folders.
    };

    void add_search_path(const char* path);

    bool find_file(const char* file_name,
        std::string& full_file_path,
        SearchFlags search_flags = SearchFlags::None);

private:
    std::list<std::string> search_paths_;
};

}

#endif // HLMDLVIEWER_FILE_SYSTEM_H_
