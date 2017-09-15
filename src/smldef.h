#ifndef SML_SMLDEF_H
#define SML_SMLDEF_H

#include <exception>
#include <string>

namespace sml
{
    /// Will be throwed on a .sml file is not support to the format.
    class ParseException : public std::runtime_error
    {
    public:
        explicit ParseException(const std::string& msg = "parse exception")
            : std::runtime_error(msg) {}
    };

    /// Will be throwed on a key not found.
    class KeyNotFound : public std::runtime_error
    {
    public:
        explicit KeyNotFound(const std::string& msg = "key not found")
            : std::runtime_error(msg) {}
    };

    /// Will be throwed on type mismatched.
    class MismatchType : public std::runtime_error
    {
    public:
        explicit MismatchType(const std::string& msg = "mismatch type")
            : std::runtime_error(msg) {}
    };

    /// Integer type
    using integer_t = int;

    /// Real type
#ifdef SML_DOUBLE
    using real_t = double;
#else
    using real_t = float;
#endif

    /// String type
    using string_t = std::string;

    /// Array type
    class array_t;

    /// table type
    class table_t;

    /// Null type
    struct Null {};

    class Integer;
    class Real;
    class String;

    template <class T>
    struct ObjectType
    {
        using type = T;
    };

    template <>
    struct ObjectType<integer_t>
    {
        using type = Integer;
    };

    template <>
    struct ObjectType<real_t>
    {
        using type = Real;
    };

    template <>
    struct ObjectType<string_t>
    {
        using type = String;
    };

    template <class T>
    using ObjectType_t = typename ObjectType<T>::type;

    template <class T>
    struct TypeTag {};

    /// Type visitor
    struct Visitor
    {
        ~Visitor() = default;
        virtual void visit(const integer_t&) {}
        virtual void visit(const real_t&) {}
        virtual void visit(const string_t&) {}
        virtual void visit(const array_t&) {}
        virtual void visit(const table_t&) {}
        virtual void visit(Null) {}
    };
}

#endif
