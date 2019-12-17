#ifndef HLMDLVIEWERTESTS_TOSTRING_CONVERSIONS_H_
#define HLMDLVIEWERTESTS_TOSTRING_CONVERSIONS_H_

#include <string>
#include <vector>

namespace Microsoft
{
    namespace VisualStudio
    {
        namespace CppUnitTestFramework
        {
            template<typename T>
            inline static std::wstring ToString(const std::vector<T>& q)
            {
                std::wstringstream ss;
                ss << '[';
                for (size_t i = 0; i < q.size(); ++i)
                {
                    ss << std::to_wstring(q[i]);
                    if (i < q.size() - 1)
                        ss << ' ';
                }
                ss << ']';
                return ss.str();
            }
        }
    }
}

#endif // HLMDLVIEWERTESTS_TOSTRING_CONVERSIONS_H_
