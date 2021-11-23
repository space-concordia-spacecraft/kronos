#pragma once

#include "ks_file.h"
#include "ks_vector.h"
#include "ks_string.h"

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
        //! \brief Default constructor with no parameters
        ApolloHeader() = default;

        //! \brief Constructor with header name and data type
        //!
        //! \param name the name of the header
        //! \param dataType the data type for the values under that header
        ApolloHeader(String name, uint8_t dataType) : name(name), dataType(dataType) {}

        //! Name of the header.
        String name = "";

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
        ApolloExporter(File* file, const Vector<ApolloHeader>& headers);

        //! \brief Destructor that closes the file
        ~ApolloExporter();

        //! \brief Writes the header list into a given file
        //!
        //! \param file File object used to write the headers into
        //! \param headers Vector of ApolloHeaders stored into the file
        KsResult Open(File* file, const Vector<ApolloHeader>& headers);

        //! \brief Writes a given header list into the file stored in the ApolloExporter object
        //!
        //! \param headers Vector of ApolloHeader objects that get decoded and then stored in a file
        //! \return KS_SUCCESS if the operation was successful
        KsResult WriteFileHeader(const Vector<ApolloHeader>& headers);

        //! \brief Writes a row of data into the file stored in the ApolloExporter
        //!
        //! \param data Vector of uint32_t data to store into the file
        //! \return KS_SUCCESS if the operation was successful
        KsResult WriteRow(const Vector<uint32_t>& data);

        //! \brief Closes the file object provided in the constructor of ApolloExporter
        void Close();

    private:
        //! File pointer to the file object used to store data and headers
        File* m_File;

        //! Status of the ApolloExporter
        KsResult m_Status;
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
        ApolloImporter(File* file);

        //! \brief Destructor that closes the file that was passed in through the constructor
        ~ApolloImporter();

        //! \brief Reads the headers from the file stored in the ApolloImporter
        //!
        //! \return KS_SUCCESS if the operation was successful
        KsResult ReadFileHeader();

        //! \brief Reads row of data from the file stored in the ApolloImporter
        //!
        //! \param data Vector of uint32_t used to store the data read from the file
        //! \return KS_SUCCESS if the operation was successful
        KsResult ReadRow(Vector<uint32_t>& data);

        //! \brief Closes the file stored in the ApolloImporter
        void Close();

        //! \brief Getter for the headers read from the file
        //!
        //! \return Vector of ApolloHeaders read from the file
        const Vector<ApolloHeader>& GetHeaders() { return m_Headers; }

    private:
        //! File object used to read the data and the headers
        File* m_File;

        //! Status of the ApolloImporter
        KsResult m_Status;

        //! Vector of ApolloHeaders used to decode the data from the file
        Vector<ApolloHeader> m_Headers;

        //! Version of the ApolloFormat
        uint32_t m_Version;
    };

}

