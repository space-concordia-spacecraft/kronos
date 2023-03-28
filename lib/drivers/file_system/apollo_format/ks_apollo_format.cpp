#include "ks_apollo_format.h"

namespace kronos {

    KsResult ApolloExporter::Export(const String& path, const List <ApolloHeader>& headers) {
        if(!m_File.IsOpen())
            KS_TRY(ks_error, m_File.Open(path, KS_OPEN_MODE_WRITE_ONLY | KS_OPEN_MODE_CREATE));

        KS_TRY(ks_error, WriteFileHeader(headers));
        return ks_success;
    }

    KsResult ApolloExporter::WriteFileHeader(const List <ApolloHeader>& headers) {
        uint32_t magicNumber = KS_APOLLO_MAGIC;
        uint32_t version = KS_APOLLO_VERSION_1;
        uint32_t headerCount = headers.size();

        // Write magic number, version
        KS_TRY(ks_error_apollo_exporter_open, m_File.Write(&magicNumber, sizeof(magicNumber)));
        KS_TRY(ks_error_apollo_exporter_open, m_File.Write(&version, sizeof(version)));
        KS_TRY(ks_error_apollo_exporter_open, m_File.Write(&headerCount, sizeof(headerCount)));

        // Write all headers
        for (const auto& header: headers) {
            uint32_t sizeOfString = header.name.size();
            KS_TRY(ks_error_apollo_exporter_open, m_File.Write(&header.dataType, sizeof(header.dataType)));
            KS_TRY(ks_error_apollo_exporter_open, m_File.Write(&sizeOfString, sizeof(sizeOfString)));
            KS_TRY(ks_error_apollo_exporter_open, m_File.Write(header.name.data(), header.name.size()));
        }

        // Sync file
        KS_TRY(ks_error_apollo_exporter_open, m_File.Sync());

        return {};
    }

    KsResult ApolloExporter::WriteRow(const List <uint32_t>& data) {
        // Write entire row
        uint32_t size = data.size() * sizeof(uint32_t);
        KS_TRY(ks_error_apollo_exporter_open, m_File.Write(data.data(), size));
        KS_TRY(ks_error_apollo_exporter_open, m_File.Sync());

        return {};
    }

    KsResult ApolloImporter::Import(const String& path) {
        KS_TRY(ks_error, m_File.Open(path, KS_OPEN_MODE_READ_ONLY));
        KS_TRY(ks_error, ReadFileHeader());

        return {};
    }

    KsResult ApolloImporter::ReadFileHeader() {
        uint32_t magicNumber;
        uint32_t headerCount;

        // Read magic number
        KS_TRY(ks_error_apollo_exporter_open, m_File.Read(&magicNumber, sizeof(magicNumber)));
        if (magicNumber != KS_APOLLO_MAGIC)
            KS_THROW(ks_error_apolloformat_header);

        // Read version
        KS_TRY(ks_error_apollo_exporter_open, m_File.Read(&m_Version, sizeof(m_Version)));
        if (m_Version != KS_APOLLO_VERSION_1)
            KS_THROW(ks_error_apolloformat_version);

        // Read header count
        KS_TRY(ks_error_apollo_exporter_open, m_File.Read(&headerCount, sizeof(headerCount)));
        for (size_t i = 0; i < headerCount; i++) {
            ApolloHeader header;

            // Read data type
            KS_TRY(ks_error_apollo_exporter_open, m_File.Read(&header.dataType, sizeof(header.dataType)));

            // Read header name size
            uint32_t nameSize;
            KS_TRY(ks_error_apollo_exporter_open, m_File.Read(&nameSize, sizeof(nameSize)));

            // Read header name
            char* str = new char[nameSize + 1];
            KS_TRY(ks_error_apollo_exporter_open, m_File.Read(str, nameSize));
            str[nameSize] = '\0';
            header.name = str;
            delete[] str;

            // Add header
            m_Headers.push_back(header);
        }

        return {};
    }

    KsResult ApolloImporter::ReadRow(List <uint32_t>& data) {
        // Clear vector
        data.clear();
        data.resize(m_Headers.size());

        // Read entire row into vector
        uint32_t size = m_Headers.size() * sizeof(uint32_t);
        KS_TRY(ks_error_apollo_exporter_open, m_File.Read(data.data(), size));

        return {};
    }

}

