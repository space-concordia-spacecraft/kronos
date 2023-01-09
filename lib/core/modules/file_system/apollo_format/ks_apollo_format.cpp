#include "ks_apollo_format.h"

#define FILE_READ_WRITE(op, nBytes) if (op != (int32_t)nBytes) \
                                return ks_error_apolloformat_readwrite_nbytes

#define FILE_OPERATION(op) if ((m_Status = op) != ks_success) \
                                return m_Status

namespace kronos {

    ApolloExporter::ApolloExporter(File* file, const std::vector<ApolloHeader>& headers) {
        Open(file, headers);
    }

    KsResult ApolloExporter::Open(File* file, const std::vector<ApolloHeader>& headers) {
        m_File = file;
        return WriteFileHeader(headers);
    }

    KsResult ApolloExporter::WriteFileHeader(const std::vector<ApolloHeader>& headers) {
        uint32_t magicNumber = KS_APOLLO_MAGIC;
        uint32_t version = KS_APOLLO_VERSION_1;
        uint32_t headerCount = headers.size();

        // Write magic number, version
        FILE_READ_WRITE(m_File->Write(&magicNumber, sizeof(magicNumber)), sizeof(magicNumber));
        FILE_READ_WRITE(m_File->Write(&version, sizeof(version)), sizeof(version));
        FILE_READ_WRITE(m_File->Write(&headerCount, sizeof(headerCount)), sizeof(headerCount));

        // Write all headers
        for (const auto& header: headers) {
            uint32_t sizeOfString = header.name.size();
            FILE_READ_WRITE(m_File->Write(&header.dataType, sizeof(header.dataType)), sizeof(header.dataType));
            FILE_READ_WRITE(m_File->Write(&sizeOfString, sizeof(sizeOfString)), sizeof(sizeOfString));
            FILE_READ_WRITE(m_File->Write(header.name.data(), header.name.size()), header.name.size());
        }

        // Sync file
        FILE_OPERATION(m_File->Sync());

        return ks_success;
    }

    KsResult ApolloExporter::WriteRow(const std::vector<uint32_t>& data) {
        // Write entire row
        uint32_t size = data.size() * sizeof(uint32_t);
        FILE_READ_WRITE(m_File->Write(data.data(), size), size);

        // Sync file
        FILE_OPERATION(m_File->Sync());

        return ks_success;
    }

    void ApolloExporter::Close() {
        // Already deleted
        if (m_File == nullptr)
            return;

        // Delete and close the file object
        delete m_File;
        m_File = nullptr;
    }

    ApolloImporter::ApolloImporter(File* file) : m_File(file) {
        ReadFileHeader();
    }

    KsResult ApolloImporter::ReadFileHeader() {
        uint32_t magicNumber;
        uint32_t headerCount;

        // Read magic number
        FILE_READ_WRITE(m_File->Read(&magicNumber, sizeof(magicNumber)), sizeof(magicNumber));
        if (magicNumber != KS_APOLLO_MAGIC)
            return ks_error_apolloformat_header;

        // Read version
        FILE_READ_WRITE(m_File->Read(&m_Version, sizeof(m_Version)), sizeof(m_Version));
        if (m_Version != KS_APOLLO_VERSION_1)
            return ks_error_apolloformat_version;

        // Read header count
        FILE_READ_WRITE(m_File->Read(&headerCount, sizeof(headerCount)), sizeof(headerCount));
        for (size_t i = 0; i < headerCount; i++) {
            ApolloHeader header;

            // Read data type
            FILE_READ_WRITE(m_File->Read(&header.dataType, sizeof(header.dataType)), sizeof(header.dataType));

            // Read header name size
            uint32_t nameSize;
            FILE_READ_WRITE(m_File->Read(&nameSize, sizeof(nameSize)), sizeof(nameSize));

            // Read header name
            char* str = new char[nameSize + 1];
            FILE_READ_WRITE(m_File->Read(str, nameSize), nameSize);
            str[nameSize] = '\0';
            header.name = str;
            delete[] str;

            // Add header
            m_Headers.push_back(header);
        }

        return ks_success;
    }

    KsResult ApolloImporter::ReadRow(std::vector<uint32_t>& data) {
        // Clear vector
        data.clear();
        data.resize(m_Headers.size());

        // Read entire row into vector
        uint32_t size = m_Headers.size() * sizeof(uint32_t);
        FILE_READ_WRITE(m_File->Read(data.data(), size), size);

        return ks_success;
    }

    void ApolloImporter::Close() {
        // Already deleted
        if (m_File == nullptr)
            return;

        // Delete and close the file object
        delete m_File;
        m_File = nullptr;
    }

}

