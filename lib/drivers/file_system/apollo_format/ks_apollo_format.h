#pragma once

#include "ks_file.h"

//! \def KS_APOLLO_VERSION_1
//! Version of the Apollo format
#define KS_APOLLO_VERSION_1     1

//! \def KS_APOLLO_INT
//! Type used in the ApolloHeader to store integers
#define KS_APOLLO_INT           0

//! \def KS_APOLLO_FLOAT
//! Type used in the ApolloHeader to store doubles
#define KS_APOLLO_FLOAT         1

//! \def KS_APOLLO_MAGIC
//! Magic number used to know if the file is using the Apollo format
#define KS_APOLLO_MAGIC 0x00001919

namespace kronos {

    //! \struct ApolloHeader
    //! \brief Header template used for the apollo format
    struct ApolloHeader {
        //! Name of the header.
        String name;

        //! Data type for the header. Used to decode the data stored in files.
        uint8_t dataType = KS_APOLLO_INT;
    };

    //! \class ApolloExporter
    //! \brief A class the implements the exporter for the apollo format
    //!
    //! This class uses a list of ApolloHeader objects to encode the data and then write into a File.
    class ApolloExporter {
    public:
        ApolloExporter() = default;
        ~ApolloExporter() = default;

        KsResult Export(const String& path, const List <ApolloHeader>& headers);

        //! \brief Writes a given header list into the file stored in the ApolloExporter object
        //!
        //! \param headers Vector of ApolloHeader objects that get decoded and then stored in a file
        //! \return KS_SUCCESS if the operation was successful
        KsResult WriteFileHeader(const List <ApolloHeader>& headers);

        //! \brief Writes a row of data into the file stored in the ApolloExporter
        //!
        //! \param data Vector of uint32_t data to store into the file
        //! \return KS_SUCCESS if the operation was successful
        KsResult WriteRow(const List <uint32_t>& data);

    private:
        //! File pointer to the file object used to store data and headers
        File m_File;

        String m_FilePath;

        //! Status of the ApolloExporter
        KsResultType m_Status = ks_error_apolloformat_status_uninitianalized;
    };

    //! \class ApolloImporter
    //! \brief Class that implements the importing of data from a file into a Vector
    //!
    //! This class reads the file headers into a Vector of ApolloHeader objects and uses the information to read data from a file.
    class ApolloImporter {
    public:
        //! \brief Constructor that uses a file to read the headers
        //!
        //! \param file File object that contains data in the Apollo format
        ApolloImporter() = default;
        ~ApolloImporter() = default;

        KsResult Import(const String& path);

        //! \brief Reads the headers from the file stored in the ApolloImporter
        //!
        //! \return KS_SUCCESS if the operation was successful
        KsResult ReadFileHeader();

        //! \brief Reads row of data from the file stored in the ApolloImporter
        //!
        //! \param data Vector of uint32_t used to store the data read from the file
        //! \return KS_SUCCESS if the operation was successful
        KsResult ReadRow(List <uint32_t>& data);

        //! \brief Getter for the headers read from the file
        //!
        //! \return Vector of ApolloHeaders read from the file
        List <ApolloHeader> GetHeaders() { return m_Headers; }

    private:
        //! File object used to read the data and the headers
        File m_File;

        String m_FilePath;

        //! Status of the ApolloImporter
        KsResultType m_Status = ks_error_apolloformat_status_uninitianalized;

        //! Vector of ApolloHeaders used to decode the data from the file
        List <ApolloHeader> m_Headers;

        //! Version of the ApolloFormat
        uint32_t m_Version = ks_error_apolloformat_version_uninitianalized;
    };

}

