#pragma once

// Standard Library
#include <cstdint>
#include <climits>
#include <cstdarg>
#include <cstring>

typedef int32_t KsResultType;
typedef uint32_t KsTickType;
typedef uint32_t KsIdType;

#include <memory>
#include <algorithm>
#include <variant>
#include <type_traits>
#include <numeric>
#include <typeinfo>

#include <string>
#include <sstream>
#include <fstream>
#include <array>
#include <vector>
#include <set>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <regex>
#include <any>
#include <span>
#include <sstream>

#include "nameof.hpp"

namespace kronos {
    // STD Pointers
    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T>
    using Scope = std::unique_ptr<T>;

    // STD Containers
    using String = std::string;
    using StringView = std::string_view;
    template<typename T>
    using List = std::vector<T>;
    template<typename T>
    using Set = std::set<T>;
    template<typename T>
    using Stack = std::stack<T>;
    template<typename K, typename V>
    using Map = std::unordered_map<K, V>;
    template<typename T>
    using PriorityQueue = std::priority_queue<T>;

    template<typename T, typename ... Args>
    Ref<T> CreateRef(Args&& ... args) {
        return std::make_shared<T, Args...>(std::forward<Args>(args)...);
    }

    template<typename T, typename ... Args>
    Scope<T> CreateScope(Args&& ... args) {
        return std::make_unique<T, Args...>(std::forward<Args>(args)...);
    }

    struct TypeInfo {
        KsIdType id;
        StringView name;
    };

    template<typename T>
    KsIdType ClassID() {
        return typeid(T).hash_code();
    }

    template<typename T>
    TypeInfo GetTypeInfo() {
        return {
            .id = ClassID<T>(),
            .name = NAMEOF_SHORT_TYPE(T)
        };
    }
}