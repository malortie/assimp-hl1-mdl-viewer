/**
* \file shader_parser.cpp
* \brief Implementation for the shader parser class.
*/

#include "pch.h"
#include "shader_parser.h"
#include <iostream>
#include <fstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

namespace hl_mdlviewer {

ShaderParser::ShaderParser(FileSystem* file_system) :
    file_system_(file_system)
{
}

void ShaderParser::parse(
    const std::string& file_path,
    std::string& parsed_shader_string)
{
    std::set<std::string> files_already_included;
    parse_recursively(file_path,
        files_already_included, 
        parsed_shader_string);
}

void ShaderParser::parse_recursively(
    const std::string& file_path,
    std::set<std::string>& files_already_included,
    std::string& parsed_shader_string)
{
    std::string file_path2;
    if (!file_system_->find_file(file_path.c_str(), file_path2, FileSystem::SearchFlags::Recursive))
        throw std::runtime_error(("Failed to load shader file " + file_path + ". No such file exists.").c_str());

    if (files_already_included.count(file_path))
        return;

    files_already_included.insert(file_path);

    std::ifstream ifs;
    try
    {
        ifs.open(fs::path(file_path2).string(), std::ios::in);

        std::string line;
        while (std::getline(ifs, line))
        {
            bool append_content = true;

            if (line.find_first_not_of(' ') != std::string::npos)
            {
                // Check for include files.
                const std::string INCLUDE_STRING = "#include";
                size_t pos = line.find(INCLUDE_STRING);
                if (pos != std::string::npos)
                {
                    append_content = false;

                    pos = line.find_first_not_of(' ', pos + INCLUDE_STRING.length());
                    if (pos != std::string::npos)
                    {
                        std::string token = line.substr(pos, line.length() - pos);
                        std::string file_name = "";
                        strip_quotes(token, file_name);

                        parse_recursively(
                            fs::path(file_path)
                                .parent_path()
                                .append(file_name).string(),
                            files_already_included,
                            parsed_shader_string);
                    }
                }
            }
            else
            {
                append_content = false;
            }

            if (append_content)
            {
                parsed_shader_string += line + '\n';
            }
        }

        ifs.close();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        ifs.close();
        throw e;
    }
}

void ShaderParser::strip_quotes(const std::string& token, std::string& result)
{
    bool finished = false;
    for (size_t i = 0; i < token.length() && !finished; ++i)
    {
        if (token[i] == '\"')
        {
            if (i != 0)
                finished = true;
        }
        else
        {
            result += token[i];
        }
    }
}

}
