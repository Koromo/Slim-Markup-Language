#ifndef SML_SMLOBJ_H
#define SML_SMLOBJ_H

#include "smldef.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace sml
{
    class Value
    {
    public:
        virtual bool is(TypeTag<integer_t>) const { return false; }
        virtual bool is(TypeTag<real_t>) const { return false; }
        virtual bool is(TypeTag<string_t>) const { return false; }
        virtual bool is(TypeTag<array_t>) const { return false; }
        virtual bool is(TypeTag<table_t>) const { return false; }
        virtual void accept(Visitor& v) const {}
    };

    /// Apply visitor for type safe processes.
    void applyVisitor(Visitor& v, const Value& val)
    {
        val.accept(v);
    }

    /// ditto
    void applyVisitor(Visitor&& v, const Value& val)
    {
        val.accept(v);
    }

    class Integer : public Value
    {
        integer_t i_;

    public:
        Integer(integer_t i)
            : i_(i)
        {
        }

        bool is(TypeTag<integer_t>) const
        {
            return true;
        }

        void accept(Visitor& v) const
        {
            v.visit(ref());
        }

        integer_t& ref()
        {
            return i_;
        }

        const integer_t& ref() const
        {
            return i_;
        }
    };

    class Real : public Value
    {
    private:
        real_t r_;

    public:
        Real(real_t r)
            : r_(r)
        {
        }

        bool is(TypeTag<real_t>) const
        {
            return true;
        }

        void accept(Visitor& v) const
        {
            v.visit(ref());
        }

        real_t& ref()
        {
            return r_;
        }

        const real_t& ref() const
        {
            return r_;
        }
    };

    class String : public Value
    {
    private:
        string_t s_;

    public:
        String(const std::string& s)
            : s_(s)
        {
        }

        bool is(TypeTag<string_t>) const
        {
            return true;
        }

        void accept(Visitor& v) const
        {
            v.visit(ref());
        }

        string_t& ref()
        {
            return s_;
        }

        const string_t& ref() const
        {
            return s_;
        }
    };

    /// Array type
    class array_t : public Value
    {
    private:
        std::vector<std::shared_ptr<Value>> arr_;

    public:
        bool is(TypeTag<array_t>) const
        {
            return true;
        }

        void accept(Visitor& v) const
        {
            v.visit(ref());
        }

        void acceptAt(Visitor& v, size_t i) const
        {
            if (i < arr_.size())
            {
                arr_[i]->accept(v);
            }
            else
            {
                v.visit(Null());
            }
        }

        array_t& ref()
        {
            return *this;
        }

        const array_t& ref() const
        {
            return *this;
        }

        /// Return the size of this array
        size_t length() const
        {
            return arr_.size();
        }

        /// Return the indexed value as a 'T' type. 
        template <class T>
        const T& valueAs(size_t i) const
        {
            using Obj = ObjectType_t<T>;
            if (!arrayIs<T>())
            {
                throw MismatchType();
            }
            const auto val = std::dynamic_pointer_cast<Obj>(arr_.at(i));
            return val->ref();
        }

        template <class T>
        T& valueAs(size_t i)
        {
            return const_cast<T&>(const_cast<const array_t&>(*this).template valueAs<T>(i));
        }

        /// Return true if the array type is 'T'.
        template <class T>
        bool arrayIs() const
        {
            return arr_.at(0)->is(TypeTag<T>());
        }

        void insertBack(const std::shared_ptr<Value>& val)
        {
            arr_.emplace_back(val);
        }
    };

    /// Return the indexed value as a 'T' type. 
    template <class T>
    const T& valueAs(size_t i, const array_t& a)
    {
        return a.template valueAs<T>(i);
    }

    /// Return true if the array type is 'T'.
    template <class T>
    bool arrayIs(const array_t& a)
    {
        return a.template arrayIs<T>();
    }

    /// Apply visitor for type safe processes.
    void applyVisitorAt(Visitor& v, size_t i, const array_t& val)
    {
        val.acceptAt(v, i);
    }

    /// ditto
    void applyVisitor(Visitor&& v, size_t i, const array_t& val)
    {
        val.acceptAt(v, i);
    }

    /// Table type
    class table_t : public Value
    {
    private:
        std::unordered_map<std::string, std::shared_ptr<Value>> table_;

    public:
        bool is(TypeTag<table_t>) const
        {
            return true;
        }

        void accept(Visitor& v) const
        {
            v.visit(ref());
        }

        void acceptAt(Visitor& v, const std::string& key) const
        {
            const auto val = get(key);
            if (val)
            {
                val->accept(v);
            }
            else
            {
                v.visit(Null());
            }
        }

        table_t& ref()
        {
            return *this;
        }

        const table_t& ref() const
        {
            return *this;
        }

        /// Whether this table contains the key.
        bool contains(const std::string& key) const
        {
            return !!get(key);
        }

        /// Count of keys.
        size_t length() const
        {
            return table_.size();
        }

        /// Return all keys
        /// TODO: Range
        std::vector<std::string> keys() const
        {
            std::vector<std::string> keys;
            keys.reserve(table_.size());
            for (const auto& e : table_)
            {
                keys.emplace_back(e.first);
            }
            return keys;
        }

        /// From the key inside the table, return a mapped value as a 'T' type. 
        template <class T>
        const T& valueAs(const std::string& key) const
        {
            using Obj = ObjectType_t<T>;

            if (!contains(key))
            {
                throw KeyNotFound();
            }
            if (!valueIs<T>(key))
            {
                throw MismatchType();
            }
            const auto val = std::dynamic_pointer_cast<const Obj>(get(key));
            return val->ref();
        }

        template <class T>
        T& valueAs(const std::string& key)
        {
            return const_cast<T&>(const_cast<const table_t&>(*this).template valueAs<T>(key));
        }

        /// Return true if the type of a value mapped by the key inside the table is 'T'.
        /// The case of type mismatch or the key is not exists, return false.
        template <class T>
        bool valueIs(const std::string& key) const
        {
            const auto val = get(key);
            return val && val->is(TypeTag<T>());
        }

        void addValue(const std::string& key, const std::shared_ptr<Value>& val)
        {
            table_.emplace(key, val);
        }

    private:
        std::shared_ptr<const Value> get(const std::string& key) const
        {
            const auto found = table_.find(key);
            if (found != std::cend(table_))
            {
                return found->second;
            }
            return nullptr;
         }

        std::shared_ptr<Value> get(const std::string& key)
        {
            return std::const_pointer_cast<Value>(const_cast<const table_t&>(*this).get(key));
        }
    };

    /// From the key inside the table, return a mapped value as a 'T' type. 
    template <class T>
    const T& valueAs(const std::string& key, const table_t& t)
    {
        return t.template valueAs<T>(key);
    }

    /// ditto
    template <class T>
    const T& valueAs(const std::string& key, const std::shared_ptr<const table_t>& t)
    {
        return valueAs<T>(key, *t);
    }

    /// Return true if the type of a value mapped by the key inside the table is 'T'.
    /// The case of type mismatch or the key is not exists, return false.
    template <class T>
    bool valueIs(const std::string& key, const table_t& t)
    {
        return t.template valueIs<T>(key);
    }

    /// ditto
    template <class T>
    bool valueIs(const std::string& key, const std::shared_ptr<const table_t>& t)
    {
        return valueIs<T>(key, *t);
    }

    /// Apply visitor for type safe processes.
    void applyVisitorAt(Visitor& v, const std::string& key, const table_t& val)
    {
        val.acceptAt(v, key);
    }

    /// ditto
    void applyVisitor(Visitor&& v, const std::string& key, const table_t& val)
    {
        val.acceptAt(v, key);
    }

    /// ditto
    void applyVisitorAt(Visitor& v, const std::string& key, const std::shared_ptr<const table_t>& val)
    {
        val->acceptAt(v, key);
    }

    /// ditto
    void applyVisitor(Visitor&& v, const std::string& key, const std::shared_ptr<const table_t>& val)
    {
        val->acceptAt(v, key);
    }
}

#endif