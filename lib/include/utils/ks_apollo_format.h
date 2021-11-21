#pragma once

#include "ks_file.h"
#include "ks_vector.h"
#include "ks_string.h"

#define KS_APOLLO_VERSION_1     1
#define KS_APOLLO_INT           0
#define KS_APOLLO_FLOAT         1

#define KS_APOLLO_MAGIC 0x00001919

namespace kronos {

    struct ApolloHeader {
        ApolloHeader() = default;
        ApolloHeader(String name, uint8_t dataType) : name(name), dataType(dataType) {}

        String name = "";
        uint8_t dataType = KS_APOLLO_INT;
    };

    class ApolloExporter {
    public:
        ApolloExporter(File* file, const Vector<ApolloHeader>& headers);
        ~ApolloExporter();

        KsResult WriteFileHeader(const Vector<ApolloHeader>& headers);
        KsResult WriteRow(const Vector<uint32_t>& data);
        void Close();

    private:
        File* m_File;
        KsResult m_Status;
    };

    class ApolloImporter {
    public:
        ApolloImporter(File* file);
        ~ApolloImporter();

        KsResult ReadFileHeader();
        KsResult ReadRow(Vector<uint32_t>& data);
        void Close();

        const Vector<ApolloHeader>& GetHeaders() { return m_Headers; }

    private:
        File* m_File;
        KsResult m_Status;
        Vector<ApolloHeader> m_Headers;
        uint32_t m_Version;
    };

}

