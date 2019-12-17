/**
* \file file_system.cpp
* \brief Implementation for the file system class.
*/

#include "pch.h"
#include "file_system.h"
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

namespace hl_mdlviewer {

FileSystem::FileSystem() :
    search_paths_()
{
}

void FileSystem::add_search_path(const char* path)
{
    search_paths_.push_back(path);
}

bool FileSystem::find_file(const char* file_name, 
    std::string& full_file_path,
    SearchFlags search_flags)
{
    // Start by checking for the file at the root of the current directory.
    fs::path path(fs::current_path());
    path.append(file_name);

    if (fs::exists(path))
    {
        full_file_path = path.string();
        return true;
    }

    // Look for the file using all search paths.
    for (auto& search_path : search_paths_)
    {
        path.clear();
        path.append(search_path);
        path.append(file_name);
        if (fs::exists(path))
        {
            full_file_path = path.string();
            return true;
        }

        // If allowed to search recursively in all sub folders, do so.
        if (search_flags & SearchFlags::Recursive)
        {
            for (auto& sub_directory : fs::recursive_directory_iterator(search_path))
            {
                path.clear();
                path.append(sub_directory);
                path.append(file_name);
                if (fs::exists(path))
                {
                    full_file_path = path.string();
                    return true;
                }
            }
        }
    }

    return false;
}

}
