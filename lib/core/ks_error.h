#pragma once

#include "ks_error_codes.h"
#include "ks_types.h"

#define KS_THROW(error, T) return ErrorOr<T>(error, LOCATION)
#define KS_THROW_PROPAGATE(error, stackTrace, T) return ErrorOr<T>(error, LOCATION, stackTrace)
#define TRY(error, expression, T)                                \
    ({                                                 \
        auto&& _temporary_result = (expression);         \
        if (_temporary_result.HasError()) [[unlikely]] \
            KS_THROW_PROPAGATE(error, _temporary_result.GetStackTrace(), T);  \
        _temporary_result.Value();             \
    })

namespace kronos {
    template<typename T>
    class ErrorOr {
    public:
        ErrorOr(T value) : m_ValueOrError(value) {}

        ErrorOr(KsResult error, const String& location, List<String> stackTrace = {}) : m_ValueOrError(error) {
            m_StackTrace.emplace_back(location + std::to_string(error));
            m_StackTrace.assign(stackTrace.begin(), stackTrace.end());
        }

        [[nodiscard]] bool HasError() const { return std::holds_alternative<KsResult>(m_ValueOrError); }

        T& Value() { return std::get<T>(m_ValueOrError); }

        const T& Value() const { return std::get<T>(m_ValueOrError); }

        KsResult& Error() { return std::get<KsResult>(m_ValueOrError); }

        [[nodiscard]] const KsResult& Error() const { return std::get<KsResult>(m_ValueOrError); }

        List<String> GetStackTrace() { return m_StackTrace; }

    private:
        Variant<T, KsResult> m_ValueOrError;
        List<String> m_StackTrace{};
    };

    template<>
    class ErrorOr<void> {
    public:
        ErrorOr() = default;

        ErrorOr(KsResult error, const String& location, List<String> stackTrace = {}) : m_ValueOrError(error) {
            m_StackTrace.emplace_back(location + std::to_string(error));
            m_StackTrace.assign(stackTrace.begin(), stackTrace.end());
        }

        [[nodiscard]] bool HasError() const { return std::holds_alternative<KsResult>(m_ValueOrError); }

        void Value() const {}

        KsResult& Error() { return std::get<KsResult>(m_ValueOrError); }

        [[nodiscard]] const KsResult& Error() const { return std::get<KsResult>(m_ValueOrError); }

        List<String> GetStackTrace() { return m_StackTrace; }

    private:
        Variant<std::monostate, KsResult> m_ValueOrError;
        List<String> m_StackTrace{};
    };
}
