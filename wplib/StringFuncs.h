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


//  ȥ����ߵĿո�
std::string TrimLeft(const std::string& s);


//  ȥ���ұߵĿո�
std::string TrimRight(const std::string& s);


//  ȥ�����˵Ŀո�
std::string Trim(const std::string& s);


//  ��ɴ�д
std::string UpperString(const std::string& s);


//  ���Сд
std::string LowerString(const std::string& s);


//  ����    ->  �ַ���
std::string IntToStr(const long i);


//  �ַ���  ->  ����
long StrToInt(const std::string& s);


//  ������  ->  �ַ���
std::string FloatToStr(const double f);


//  �ַ���  ->  ������
double StrToFloat(const std::string& s);


//  �ҵ���ǰһ��'find'�ַ�
long FindFirstChar(const char* pc, const char find);


//  �ҵ����һ��'find'�ַ�
long FindLastChar(const char* pc, const char find);


//  ȡ��·�����֣�ȥ�������ļ�����
std::string GetDirectoryPart(const std::string& pathfilename);


//  ȡ�������ļ�����ȥ��·�����֣�
std::string RemoveDirectoryPart(const std::string& pathfilename);


//  ȡ����չ��
std::string GetExtNamePart(const std::string& filename);


//  ȥ����չ��
std::string RemoveExtNamePart(const std::string& filename);


//  �����ı��ļ�
bool File2String(const std::string& filename, std::string& text);


//  д���ı��ļ�
bool String2File(const std::string& text, const std::string& filename);

}

#endif
