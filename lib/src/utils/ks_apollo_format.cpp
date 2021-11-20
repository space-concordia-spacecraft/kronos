#include "ks_apollo_format.h"


namespace kronos{

    kronos::ApolloExporter::ApolloExporter(kronos::File *file, const kronos::Vector<kronos::ApolloHeader> &headers) {
        WriteHeader(headers);
    }

    void kronos::ApolloExporter::WriteHeader(const Vector<ApolloHeader>& headers) {
        uint32_t version = KS_APOLLO_VERSION_1;
        uint32_t magicNumber = 0x00690420;
        uint32_t numberOfHeaders = headers.Size();

        //
        m_File->Write(&magicNumber,sizeof(magicNumber));
        m_File->Write(&version,sizeof(version));
        m_File->Write(&numberOfHeaders,sizeof(numberOfHeaders));

        for(const auto& header : headers){
            uint32_t sizeOfString = header.name.Size();
            m_File->Write(&header.dataType,sizeof(header.dataType));
            m_File->Write(&sizeOfString,sizeof(sizeOfString));
            m_File->Write(header.name.Ptr(),header.name.Size());
        }

    }

    void kronos::ApolloExporter::WriteRow(const Vector<uint32_t>& data) {
        for(const auto& dataVar : data ){
            m_File->Write(&dataVar, sizeof(dataVar));
        }
    }
}

