/** \file shader_parser.cpp
* \brief Includes tests for shader parser class.
*/

#include "pch.h"
#include "CppUnitTest.h"
#include "file_system.h"
#include "shader_parser.h"

#if defined (_WIN32) && defined (_DEBUG)
#include "enable_memory_leak_detection.h"
static hl_mdlviewer::EnableMemoryLeakDetection memory_leak_detection;
#endif

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
    TEST_CLASS(TestShaderParser)
    {
        static hl_mdlviewer::FileSystem file_system_;
        static hl_mdlviewer::ShaderParser shader_parser_;

    public:

        TEST_CLASS_INITIALIZE(TestInit)
        {
            file_system_.add_search_path(HLMDLVIEWER_SHADERS_SEARCH_PATH);
        }

        TEST_METHOD(IncludesAreOnlyIncludedOnce)
        {
            std::string parsed_shader_string;

            // Test includes with quotes.
            shader_parser_.parse("includes_are_only_included_once/includes_with_quotes.shader", parsed_shader_string);

            Assert::AreEqual(1, count_occurences_of_string_in_string(
                parsed_shader_string, "my_matrix"));

            parsed_shader_string.clear();

            // Test includes with no quotes.
            shader_parser_.parse("includes_are_only_included_once/includes_with_no_quotes.shader", parsed_shader_string);

            Assert::AreEqual(1, count_occurences_of_string_in_string(
                parsed_shader_string, "my_matrix"));
        };

    private:
        int count_occurences_of_string_in_string(const std::string& searched_string, const std::string& string_to_match)
        {
            int num_occurences = 0;
            size_t pos = 0;

            do
            {
                pos = searched_string.find(string_to_match, pos);
                if (pos != std::string::npos)
                {
                    pos += string_to_match.length();
                    ++num_occurences;
                }
            } while (pos != std::string::npos);

            return num_occurences;
        }
    };

    hl_mdlviewer::FileSystem TestShaderParser::file_system_;
    hl_mdlviewer::ShaderParser TestShaderParser::shader_parser_(&file_system_);
}
