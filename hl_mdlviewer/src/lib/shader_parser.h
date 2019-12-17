/**
* \file shader_parser.h
* \brief Declaration for the shader parser class.
*/

#ifndef HLMDLVIEWER_SHADER_PARSER_H_
#define HLMDLVIEWER_SHADER_PARSER_H_

#include "file_system.h"
#include <string>
#include <set>

namespace hl_mdlviewer {

class ShaderParser
{
public:
    ShaderParser(FileSystem* file_system);

    void parse(
        const std::string& file_path,
        std::string& parsed_shader_string);

protected:
    void parse_recursively(
        const std::string& file_path,
        std::set<std::string>& files_already_included,
        std::string& parsed_shader_string);

    void strip_quotes(const std::string& token, std::string& result);

private:
    FileSystem* file_system_;
};

}

#endif // HLMDLVIEWER_SHADER_PARSER_H_
