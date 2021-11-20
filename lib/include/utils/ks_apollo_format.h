#pragma once
#include "ks_file.h"
#include "ks_vector.h"
#include "ks_string.h"

#define KS_APOLLO_VERSION_1     1
#define KS_APOLLO_INT           0
#define KS_APOLLO_FLOAT         1

#define KS_APOLLO_MAGIC 0x00001919

namespace kronos{
    struct ApolloHeader {
        String name = "";
        uint8_t dataType = KS_APOLLO_INT;
    };

    class ApolloExporter {
    public:
        ApolloExporter(File* file,const Vector<ApolloHeader>& headers);
        void WriteHeader(const Vector<ApolloHeader>& headers);
        void WriteRow(const Vector<uint32_t>& data);
    private:
        File* m_File;
    };

    class ApolloImporter {
    public:
        ApolloImporter(File* file);
        KsResult ReadHeader();
        KsResult ReadRow(Vector<uint32_t>& data);

        const Vector<ApolloHeader> & GetHeaders(){ return m_Headers;}

    private:
        File* m_File;
        Vector<ApolloHeader> m_Headers;
        uint32_t m_Version;

    };

}

