#include "ks_apollo_format.h"

#define FILE_OPERATION(op) if ((m_Status = op) != KS_SUCCESS) \
                                return m_Status

namespace kronos {

    ApolloExporter::ApolloExporter(File* file, const Vector<ApolloHeader>& headers) {
        Open(file, headers);
    }


    ApolloExporter::~ApolloExporter() {
        Close();
    }

    KsResult ApolloExporter::Open(File* file, const Vector<ApolloHeader>& headers) {
        m_File = file;
        return WriteFileHeader(headers);
    }

    KsResult ApolloExporter::WriteFileHeader(const Vector<ApolloHeader>& headers) {
        uint32_t magicNumber = KS_APOLLO_MAGIC;
        uint32_t version = KS_APOLLO_VERSION_1;
        uint32_t headerCount = headers.Size();

        // Write magic number, version
        FILE_OPERATION(m_File->Write(&magicNumber, sizeof(magicNumber)));
        FILE_OPERATION(m_File->Write(&version, sizeof(version)));
        FILE_OPERATION(m_File->Write(&headerCount, sizeof(headerCount)));

        // Write all headers
        for (const auto& header: headers) {
            uint32_t sizeOfString = header.name.Size();
            FILE_OPERATION(m_File->Write(&header.dataType, sizeof(header.dataType)));
            FILE_OPERATION(m_File->Write(&sizeOfString, sizeof(sizeOfString)));
            FILE_OPERATION(m_File->Write(header.name.Ptr(), header.name.Size()));
        }

        // Sync file
        FILE_OPERATION(m_File->Sync());

        return KS_SUCCESS;
    }

    KsResult ApolloExporter::WriteRow(const Vector<uint32_t>& data) {
        // Write entire row
        FILE_OPERATION(m_File->Write(data.Ptr(), data.Size() * sizeof(uint32_t)));

        // Sync file
        FILE_OPERATION(m_File->Sync());

        return KS_SUCCESS;
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

    ApolloImporter::~ApolloImporter() {
        Close();
    }

    KsResult ApolloImporter::ReadFileHeader() {
        uint32_t magicNumber;
        uint32_t headerCount;

        // Read magic number
        FILE_OPERATION(m_File->Read(&magicNumber, sizeof(magicNumber)));
        if (magicNumber != KS_APOLLO_MAGIC)
            return KS_ERROR_APOLLO_HEADER;

        // Read version
        FILE_OPERATION(m_File->Read(&m_Version, sizeof(m_Version)));
        if (m_Version != KS_APOLLO_VERSION_1)
            return KS_ERROR_APOLLO_VERSION;

        // Read header count
        FILE_OPERATION(m_File->Read(&headerCount, sizeof(headerCount)));
        for (size_t i = 0; i < headerCount; i++) {
            ApolloHeader header;

            // Read data type
            FILE_OPERATION(m_File->Read(&header.dataType, sizeof(header.dataType)));

            // Read header name size
            uint32_t nameSize;
            FILE_OPERATION(m_File->Read(&nameSize, sizeof(nameSize)));

            // Read header name
            char* str = new char[nameSize + 1];
            FILE_OPERATION(m_File->Read(str, nameSize));
            str[nameSize] = '\0';
            header.name = str;
            delete[] str;

            // Add header
            m_Headers.Add(header);
        }

        return KS_SUCCESS;
    }

    KsResult ApolloImporter::ReadRow(Vector<uint32_t>& data) {
        // Clear vector
        data.Clear();
        data.Reserve(m_Headers.Size());

        // Read entire row into vector
        FILE_OPERATION(m_File->Read(data.Ptr(), m_Headers.Size() * sizeof(uint32_t)));

        return KS_SUCCESS;
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

