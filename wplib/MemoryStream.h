#if !defined(MEMORYSTREAM_INCLUDED_)
#define MEMORYSTREAM_INCLUDED_

#pragma warning(disable: 4786)
#include <list>
#include <map>
#include <string>

namespace weipan
{


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

typedef dword StreamOffsetOrigin;
const StreamOffsetOrigin SOFROMBEGINNING    = 0;
const StreamOffsetOrigin SOFROMCURRENT      = 1;
const StreamOffsetOrigin SOFROMEND          = 2;
const dword MEMSTREAM_MINIMAL_EXPAND_SIZE = 65536;

class CMemoryStream
{

public:
    CMemoryStream();
    CMemoryStream(const dword size);
    CMemoryStream(const CMemoryStream& memorystream);
    ~CMemoryStream();

private:
    char*   m_pBuffer;          //point to the memory block header
    char*   m_pPos;             //working cursor
    dword   m_BufferSize;       //length of the buffer
    dword   m_AssignedSize;     //the accurate size of stream

    void encrypt();
    void decrypt();

public:
    bool __stdcall Eof() const;
    bool __stdcall Bof() const;
    dword __stdcall MovePos(const long delta, const StreamOffsetOrigin origin = SOFROMBEGINNING);
    dword __stdcall GetPos() const;
    dword Read(void* const p, const dword count);
    dword Write(const void* const p, const dword count);
    bool __stdcall SetSize(const dword size);
    dword __stdcall GetSize() const;
    bool __stdcall SaveToFile(const char* const filename);
    bool __stdcall LoadFromFile(const char* const filename);
    const void* _GetRawPos() const;

};
//================================================================================

}

#endif