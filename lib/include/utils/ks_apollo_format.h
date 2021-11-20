#pragma once
#include "ks_file.h"
#include "ks_vector.h"
#include "ks_string.h"

#define KS_APOLLO_VERSION_1     1
#define KS_APOLLO_INT           0
#define KS_APOLLO_FLOAT         1

namespace kronos{
    struct ApolloHeader{
        String name="";
        uint8_t dataType = KS_APOLLO_INT;
    };

    class ApolloExporter{
    public:
        ApolloExporter(File* file,const Vector<ApolloHeader>& headers);
        void WriteHeader(const Vector<ApolloHeader>& headers);
        void WriteRow(const Vector<uint32_t>& data);
    private:
        File* m_File;
    };

}

