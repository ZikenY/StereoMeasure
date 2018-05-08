#if !defined(STRINGFUNCS_INCLUDE_)
#define STRINGFUNCS_INCLUDE_

#if !defined(NULL)
    #define NULL 0
#endif

#if !defined(byte)
    typedef unsigned char byte;
#endif

#if !defined(word)
    typedef unsigned short word;
#endif

#if !defined(dword)
    typedef unsigned long dword;
#endif

//C++ Standand Libs
#pragma warning(disable: 4786)
#include <vector>
#include <string>

namespace weipan
{

class Strings
{
public:
    Strings();
    Strings(const std::string& text);
    ~Strings();

private:
    std::vector<std::string> m_lines;

public:
    void SetText(const std::string& text);
    std::string GetText() const;
    dword GetLineCount() const;
    bool GetLine(const dword index, std::string& line) const;
    void AppendLine(const std::string& line);
    bool SetLine(const dword index, const std::string& line);
    bool DeleteLine(const dword index);
};


//  去掉左边的空格
std::string TrimLeft(const std::string& s);


//  去掉右边的空格
std::string TrimRight(const std::string& s);


//  去掉两端的空格
std::string Trim(const std::string& s);


//  变成大写
std::string UpperString(const std::string& s);


//  变成小写
std::string LowerString(const std::string& s);


//  整形    ->  字符串
std::string IntToStr(const long i);


//  字符串  ->  整形
long StrToInt(const std::string& s);


//  浮点型  ->  字符串
std::string FloatToStr(const double f);


//  字符串  ->  浮点型
double StrToFloat(const std::string& s);


//  找到最前一个'find'字符
long FindFirstChar(const char* pc, const char find);


//  找到最后一个'find'字符
long FindLastChar(const char* pc, const char find);


//  取出路径部分（去掉最后的文件名）
std::string GetDirectoryPart(const std::string& pathfilename);


//  取出最后的文件名（去掉路径部分）
std::string RemoveDirectoryPart(const std::string& pathfilename);


//  取出扩展名
std::string GetExtNamePart(const std::string& filename);


//  去掉扩展名
std::string RemoveExtNamePart(const std::string& filename);


//  读出文本文件
bool File2String(const std::string& filename, std::string& text);


//  写入文本文件
bool String2File(const std::string& text, const std::string& filename);

}

#endif
