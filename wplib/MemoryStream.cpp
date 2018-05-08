#include "MemoryStream.h"

#include <stdio.h>
#include <memory.h>
#include <malloc.h>

namespace weipan
{

inline bool _invalid(const void* const p)
{
    return p ? false : true;
};

CMemoryStream::CMemoryStream()
{
    m_pBuffer       = NULL;
    m_BufferSize    = 0;
    m_AssignedSize  = 0;
    m_pPos          = m_pBuffer;
}

CMemoryStream::CMemoryStream(const dword size)
{
    if (0 < size)
    {
        m_pBuffer = (char*)::malloc(size);
        if (!m_pBuffer)
        {
            //�ڴ����
            m_BufferSize = m_AssignedSize = 0;
            m_pPos = m_pBuffer;
            return;
        }
        ::memset(m_pBuffer, 0, size);
    }
    else
    {
        m_pBuffer = NULL;
    }
    m_BufferSize = m_AssignedSize = size;
    m_pPos = m_pBuffer;
}

CMemoryStream::CMemoryStream(const CMemoryStream& memorystream)
{
    m_BufferSize = m_AssignedSize = memorystream.m_AssignedSize;
    if (0 < m_BufferSize)
    {
        m_pBuffer = (char*)::malloc(m_BufferSize);
        if (!m_pBuffer)
        {
            //�ڴ����
            m_BufferSize = m_AssignedSize = 0;
            m_pPos = m_pBuffer;
            return;
        }
        ::memcpy(m_pBuffer, memorystream.m_pBuffer,
            m_AssignedSize);
    }
    else
    {
        m_pBuffer = NULL;
    }
    this->MovePos(memorystream.GetPos());
}

CMemoryStream::~CMemoryStream()
{
    ::free(m_pBuffer);
}

bool __stdcall CMemoryStream::Eof() const
{
    return (this->GetPos() == this->GetSize());
}

bool __stdcall CMemoryStream::Bof() const
{
    return (0 == this->GetPos());
}

dword __stdcall CMemoryStream::MovePos(const long delta,
    const StreamOffsetOrigin origin)
{
    dword posdelta;
    switch(origin)
    {
    case SOFROMBEGINNING:
        posdelta = (dword)delta;
        break;
    case SOFROMCURRENT:
        posdelta = (dword)(m_pPos - m_pBuffer + delta);
        break;
    case SOFROMEND:
        posdelta = m_AssignedSize - (dword)delta;
        break;
    default: return 0;
    }
    if ((0 > posdelta) || (posdelta > m_AssignedSize))
        return 0;

    m_pPos = m_pBuffer + posdelta;

    return m_pPos - m_pBuffer;
}

dword __stdcall CMemoryStream::GetPos() const
{
    return m_pPos - m_pBuffer;
}

dword CMemoryStream::Read(void* const p, const dword count)
{
    if (_invalid(p)) throw;
    if (0 >= m_AssignedSize) return 0;

    dword realcount;
    dword remain = m_AssignedSize - (m_pPos - m_pBuffer);
    if (remain >= count)
    {
        realcount = count;
    }
    else
    {
        realcount = remain;
    }
    ::memcpy(p, m_pPos, realcount);
    m_pPos += realcount;
    return realcount;
}

dword CMemoryStream::Write(const void* const p, const dword count)
{
    if (_invalid(p)) throw;
    if (0 >= count) return 0;

    //������ڴ������Բ��Ǻܺã����ڴ��������¶����Ը�Ч�Ĺ���

    dword oldpos = m_pPos - m_pBuffer;
    //����ڴ���buffer�ĳ��ȣ������������չ
    dword remain = m_BufferSize - oldpos;
    if (remain < count)
    {
        //buffer���Ȳ�����������չ֮
        //ע��������ڴ��С��3�ֿ��ܣ�
        //    1. count
        //    2. m_BufferSize
        //    3. m_MinRequestSize
        dword requestsize;
        if (count > m_BufferSize)
        {
            requestsize = count;
        }
        else
        {
            requestsize = m_BufferSize;
        }

        if (requestsize < MEMSTREAM_MINIMAL_EXPAND_SIZE)
        {
            requestsize = MEMSTREAM_MINIMAL_EXPAND_SIZE;
        }

        dword buffersize = oldpos + requestsize;
        if (m_pBuffer)
        {
            char* pTmp = (char*)::realloc(m_pBuffer,
                buffersize);
            if (!pTmp)
            {
                //�ڴ����
                return false;
            }
            m_pBuffer = pTmp;   //�п����ڴ����׵�ַ�Ѿ������˸ı�
        }
        else
        {
            m_pBuffer = (char*)::malloc(buffersize);
            if (!m_pBuffer)
            {
                //�ڴ����
                return false;
            }
        }

        m_BufferSize = buffersize;
        m_pPos = m_pBuffer + oldpos;//�����˽��α�ָ����ȷ��λ��
    }

    //���m_AssignedSize��ֵ��ȷ����ȷ����Ч����
    if (m_AssignedSize - oldpos < count)
    {
        m_AssignedSize = oldpos + count;
    }

    ::memcpy(m_pPos, p, count);
    m_pPos += count;
    return count;
}

bool __stdcall CMemoryStream::SetSize(const dword size)
{
    if (0 > size) return false;

    //�����и����ȱ��
    //buffer���Ⱥ���Ч���Ƚ�����Ϊͬ����ֵ��m_AssignedSize = m_BufferSize = size
    //�����Ҫ��LoadFormFile()������һ�������ȷ����

    dword oldpos = m_pPos - m_pBuffer;
    if (m_pBuffer)
    {
        char* pTmp = (char*)::realloc(m_pBuffer, size);
        if (0 == size)
        {
            m_pBuffer = NULL;
        }
        else if (!pTmp)
        {
            //�ڴ����
            return false;
        }
        //�п����ڴ����׵�ַ�Ѿ������˸ı�
        m_pBuffer = pTmp;
    }
    else if (0 < size)
    {
        m_pBuffer = (char*)::malloc(size);
        if (!m_pBuffer)
        {
            //�ڴ����
            return false;
        }
    }

    if (size > m_AssignedSize)
    {
//        CharType* p = m_pBuffer + m_AssignedSize;
//        ::memset(p, 0, (size - m_AssignedSize));
        m_pPos = m_pBuffer + oldpos;
    }
    else
    {
        if (size > oldpos)
        {
            m_pPos = m_pBuffer + oldpos;
        }
        else
        {
            m_pPos = m_pBuffer + size;
        }
    }
    m_BufferSize = m_AssignedSize = size;
    return true;
}

dword __stdcall CMemoryStream::GetSize() const
{
    //���ﷵ�ص�����Ч����
    return m_AssignedSize;
}

void CMemoryStream::encrypt()
{
    long size = this->GetSize();
    for (long i=0; i< size; i++)
    {
        *(m_pBuffer+i) = *(m_pBuffer+i) + 100;
    }
}

void CMemoryStream::decrypt()
{
    long size = this->GetSize();
    for (long i=0; i< size; i++)
    {
        *(m_pBuffer+i) = *(m_pBuffer+i) - 100;
    }
}

bool __stdcall CMemoryStream::SaveToFile(const char* const filename)
{
    FILE* pFile = ::fopen(filename, "wb");
    if (!pFile)
    {
        return false;
    }

    //��д��4bytes�ĳ�����Ϣ
    ::fwrite(&m_AssignedSize, sizeof(dword), 1, pFile);

    this->encrypt();

    dword numwritten = 0;
    if (0 < m_AssignedSize)
    {
        numwritten = ::fwrite(m_pBuffer, sizeof(char),
            m_AssignedSize, pFile);
    }
    ::fclose(pFile);

    this->decrypt();

    if (numwritten != m_AssignedSize)
    {
        //���ܴ��̿ռ䲻��
        throw;
    }
    return true;
}

bool __stdcall CMemoryStream::LoadFromFile(const char* const filename)
{
    this->SetSize(0);//��������

    FILE* pFile = ::fopen(filename, "rb");
    if (!pFile)
    {
        return false;
    }

    //�ȶ���4bytes�ĳ�����Ϣ
    dword numreaded = ::fread(&m_AssignedSize, sizeof(dword), 1, pFile);
    if (numreaded < 1)
    {
        return false;
    }

    m_BufferSize = m_AssignedSize;
    numreaded = 0;
    if (0 < m_AssignedSize)
    {
        m_pBuffer = (char*)::malloc(m_AssignedSize);
        if (!m_pBuffer)
        {
            //�ڴ����
            ::fclose(pFile);
            m_pPos = m_pBuffer;
            m_BufferSize = m_AssignedSize = 0;
            return false;
        }
        numreaded = ::fread(m_pBuffer, sizeof(char),
            m_AssignedSize, pFile);
    }
    else
    {
        //������SetSize(0)���Ѿ�����
    }
    ::fclose(pFile);
    m_pPos = m_pBuffer;

    this->decrypt();

    if (numreaded != m_AssignedSize)
    {
        throw;
    }

    return true;
}

const void* CMemoryStream::_GetRawPos() const
{
    return m_pPos;
}

}