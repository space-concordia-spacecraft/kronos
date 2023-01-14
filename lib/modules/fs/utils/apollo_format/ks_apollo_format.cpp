#include "ks_apollo_format.h"

namespace kronos {

    ApolloExporter::ApolloExporter(const String& path, const List <ApolloHeader>& headers) :
        m_File(path, KS_OPEN_MODE_WRITE_ONLY | KS_OPEN_MODE_CREATE) {
        WriteFileHeader(headers);
    }

    ApolloExporter::~ApolloExporter() {
        Close();
    }

    KsResultType ApolloExporter::WriteFileHeader(const List <ApolloHeader>& headers) {
        uint32_t magicNumber = KS_APOLLO_MAGIC;
        uint32_t version = KS_APOLLO_VERSION_1;
        uint32_t headerCount = headers.size();

        // Write magic number, version
        m_File.Write(&magicNumber, sizeof(magicNumber));
        m_File.Write(&version, sizeof(version));
        m_File.Write(&headerCount, sizeof(headerCount));

        // Write all headers
        for (const auto& header: headers) {
            uint32_t sizeOfString = header.name.size();
            m_File.Write(&header.dataType, sizeof(header.dataType));
            m_File.Write(&sizeOfString, sizeof(sizeOfString));
            m_File.Write(header.name.data(), header.name.size());
        }

        // Sync file
        auto ret = m_File.Sync();
        if (ret != ks_success) {
            return ret;
        }

        return ks_success;
    }

    KsResultType ApolloExporter::WriteRow(const List <uint32_t>& data) {
        // Write entire row
        uint32_t size = data.size() * sizeof(uint32_t);
        m_File.Write(data.data(), size);

        // Sync file
        auto ret = m_File.Sync();
        if (ret != ks_success) {
            return ret;
        }

        return ks_success;
    }

    void ApolloExporter::Close() {
        m_File.Close();
    }

    ApolloImporter::ApolloImporter(const String& path) :
        m_File(path, KS_OPEN_MODE_READ_ONLY) {
        ReadFileHeader();
    }

    ApolloImporter::~ApolloImporter() {
        Close();
    }

    KsResultType ApolloImporter::ReadFileHeader() {
        uint32_t magicNumber;
        uint32_t headerCount;

        // Read magic number
        m_File.Read(&magicNumber, sizeof(magicNumber));
        if (magicNumber != KS_APOLLO_MAGIC)
            return ks_error_apolloformat_header;

        // Read version
        m_File.Read(&m_Version, sizeof(m_Version));
        if (m_Version != KS_APOLLO_VERSION_1)
            return ks_error_apolloformat_version;

        // Read header count
        m_File.Read(&headerCount, sizeof(headerCount));
        for (size_t i = 0; i < headerCount; i++) {
            ApolloHeader header;

            // Read data type
            m_File.Read(&header.dataType, sizeof(header.dataType));

            // Read header name size
            uint32_t nameSize;
            m_File.Read(&nameSize, sizeof(nameSize));

            // Read header name
            char* str = new char[nameSize + 1];
            m_File.Read(str, nameSize);
            str[nameSize] = '\0';
            header.name = str;
            delete[] str;

            // Add header
            m_Headers.push_back(header);
        }

        return ks_success;
    }

    KsResultType ApolloImporter::ReadRow(List <uint32_t>& data) {
        // Clear vector
        data.clear();
        data.resize(m_Headers.size());

        // Read entire row into vector
        uint32_t size = m_Headers.size() * sizeof(uint32_t);
        m_File.Read(data.data(), size);

        return ks_success;
    }

    void ApolloImporter::Close() {
        // Make sure to close file
        m_File.Close();
    }

}

