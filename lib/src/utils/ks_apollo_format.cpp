#include "ks_apollo_format.h"


namespace kronos{

    ApolloExporter::ApolloExporter(File *file, const Vector<ApolloHeader> &headers) {
        WriteHeader(headers);
    }

    void ApolloExporter::WriteHeader(const Vector<ApolloHeader>& headers) {
        uint32_t magicNumber = 0;
        uint32_t version = KS_APOLLO_VERSION_1;
        uint32_t numberOfHeaders = headers.Size();

        m_File->Write(&magicNumber,sizeof(magicNumber));
        m_File->Write(&version,sizeof(version));
        m_File->Write(&numberOfHeaders,sizeof(numberOfHeaders));

        for(const auto& header : headers){
            uint32_t sizeOfString = header.name.Size();
            m_File->Write(&header.dataType, sizeof(header.dataType));
            m_File->Write(&sizeOfString, sizeof(sizeOfString));
            m_File->Write(header.name.Ptr(), header.name.Size());
        }

    }

    void ApolloExporter::WriteRow(const Vector<uint32_t>& data) {
        for(const auto& dataVar : data ){
            m_File->Write(&dataVar, sizeof(dataVar));
        }
    }

    ApolloImporter::ApolloImporter(File* file):m_File(file) {
        ReadHeader();
    }

    KsResult ApolloImporter::ReadHeader() {
        uint32_t magicNumber;
        uint32_t numberOfHeaders;

        m_File->Read(&magicNumber, sizeof(magicNumber));

        if(magicNumber != KS_APOLLO_MAGIC)
            return KS_ERROR_APOLLO_HEADER;

        m_File->Read(&m_Version, sizeof(m_Version));

        if(m_Version != KS_APOLLO_VERSION_1)
            return KS_ERROR_APOLLO_VERSION;

        m_File->Read(&numberOfHeaders, sizeof(numberOfHeaders));

        for(size_t i = 0; i < numberOfHeaders; i ++) {
            ApolloHeader newHeader;

            uint32_t stringSize;

            m_File->Read(&newHeader.dataType, sizeof(newHeader.dataType));
            m_File->Read(&stringSize, sizeof(stringSize));

            char* str = new char [stringSize + 1];

            m_File->Read(str, stringSize);
            str[stringSize] = '\0';

            newHeader.name = str;

            m_Headers.Add(newHeader);
        }
        return KS_SUCCESS;
    }

    KsResult ApolloImporter::ReadRow(Vector<uint32_t>& data) {
        data.Clear();
        data.Reserve(m_Headers.Size());

        m_File->Read(data.Ptr(), m_Headers.Size() * sizeof(uint32_t));

        return KS_SUCCESS;
    }
}

