#pragma once

// std
#include <string.h>
#include <ctype.h>
#include <string>
// 3rd-party
#include "fmt/format.h"
// Arkadia
#include "Array.hpp"
#include "CodeUtils.hpp"
#include "Macros.hpp"

namespace ark {

//
// Constants
//
constexpr char   NULL_CHAR = '\0';
constexpr size_t INVALID_STRING_INDEX = std::string::npos;


//
// C - String
//
ARK_FORCE_INLINE size_t
CStrLen(const char * const str)
{
    if(!str) {
        return 0;
    }
    return strlen(str);
}

ARK_FORCE_INLINE bool
CStrEquals(const char * const lhs, char const * const rhs)
{
    return strcmp(lhs, rhs) == 0;
}

ARK_FORCE_INLINE bool
CStrIsNullEmptyOrWhitespace(char const * const str)
{
    if(!str) {
        return true;
    }
    if(CStrLen(str) == 0) {
        return true;
    }
    for(char const *c = str; c != nullptr; ++c) {
        if(*c != ' ') {
            return false;
        }
    }
    return true;
}

ARK_FORCE_INLINE void
CStrMemCopy(
    char         const *dst,
    size_t       const offset,
    char const * const src,
    size_t       const src_size)
{
    memcpy(Cast<void*>(dst + offset), src, src_size);
}



//
// String
//
class String :
    public std::string
{
private:
    typedef std::string __Container;

    //
    // Static Methods
    //
public:
    static String const&
    Empty()
    {
        ark_local_persist String s;
        return s;
    }

    static String
    Repeat(String const &what, size_t const times)
    {
        if(times == 0) {
            return String::Empty();
        }
        size_t const repeat_len = what.Length();
        size_t const result_len = times * repeat_len;
        String       result     = String::CreateWithLength(result_len);

        for(size_t i = 0; i < result_len; i += repeat_len) {
            result.MemCopy(i, what.CStr(), repeat_len);
        }

        return result;
    }

    static String
    CreateWithLength(size_t const len)
    {
        String s;
        s.resize(len);
        return s;
    }

    static String
    CreateWithCapacity(size_t capacity) 
    {
        String s;
        s.Reserve(capacity);
        return s;
    }

    template <typename Type>
    static String
    Join(
        typename Type const &beg,
        typename Type const &end,
        char const separator = ' ')
    {
        Type curr = beg;

        size_t size_to_reserve = 0;
        while(curr != end) {
            size_to_reserve += curr->Length() + 1; // Separator Size.
            ++curr;
        }

        String s = String::CreateWithCapacity(size_to_reserve);
        size_t offset = 0;

        curr = beg;
        while(curr != end) {
            String const &component     = *curr;
            size_t const  component_len = component.Length();

            s.MemCopy(offset, component.CStr(), component_len);
            offset += component_len;

            s[offset] = separator;
            offset += 1;

            ++curr;
        }

        return s;
    }

    static String
    Join(Array<String> const &components, char const separator = ' ')
    {
        return Join(components.Begin(), components.End(), separator);
    }

    bool operator==(String const &rhs) const
    {
        return this->StdString() == rhs.StdString();
    }


    //
    // CTOR / DTOR
    //
public:
    String(i32 i) : __Container(std::to_string(i)) {}
    String(u32 i) : __Container(std::to_string(i)) {}

    String(std::string const &str)
        : std::string(str)
    {
        // Empty...
    }

    String(char const * const str = "")
        : std::string(str)
    {
        // Empty...
    }

    explicit String(char const c)
    {
        __Container::push_back(c);
    }

    //
    //
    //
public:
    char        const * CStr     () const { return this->c_str(); }
    std::string const & StdString() const { return *this;         }

    //
    //
    //
public:
    template <typename ...Args>
    static String
    Format(String const &fmt, Args... args)
    {
        return fmt::format(fmt.CStr(), args...);
    }

    //
    //
    //
public:
    bool   IsEmpty() const { return __Container::empty(); }
    size_t Length () const { return __Container::size (); }

    bool 
    IsEmptyOrWhitespace() const
    {
        if(IsEmpty()) {
            return true;
        }

        size_t const index = FindFirstNotOf(' ');
        return index == INVALID_STRING_INDEX;
    }

    //
    //
    //
public:
    void Reserve(size_t const capacity) { __Container::reserve(capacity); }

    void
    MemCopy(
        size_t       const offset,
        char const * const mem_to_copy,
        size_t       const size_to_copy)
    {
        CStrMemCopy(CStr(), offset, mem_to_copy, size_to_copy);
    }


    //
    //
    //
public:
    bool StartsWith(String const &needle) const;
    bool StartsWith(char   const  needle) const;

    bool EndsWith(String const &needle) const;
    bool EndsWith(char   const  needle) const;

    //
    //
    //
public:
    size_t FindIndexOf   (char const needle, size_t start_index = 0) const;
    size_t FindFirstNotOf(char const needle, size_t start_index = 0) const;

    size_t FindLastIndexOf   (char const needle, size_t start_index = 0) const;
    size_t FindLastIndexNotOf(char const needle, size_t start_index = 0) const;

    //
    //
    //
public:
    Array<String> Split(char        const  separator ) const;
    Array<String> Split(Array<char> const &separators) const;

    void PushBack(String const &rhs) { *this += rhs; }
    void PushBack(char   const  rhs) { *this += rhs; }

    void PopBack() { __Container::pop_back(); }


    __Container::reference       Back()       { return __Container::back(); }
    __Container::const_reference Back() const { return __Container::back(); }

    __Container::reference       Front()       { return __Container::front(); }
    __Container::const_reference Front() const { return __Container::front(); }

    __Container::iterator       begin() noexcept       { return __Container::begin(); }
    __Container::const_iterator begin() const noexcept { return __Container::begin(); }
    __Container::iterator       end  () noexcept       { return __Container::end  (); }
    __Container::const_iterator end  () const noexcept { return __Container::end  (); }

    //
    //
    //
public:
    String 
    SubString(size_t const left_index, size_t const right_index) const
    {
        return __Container::substr(left_index, right_index - left_index);
    }


    //
    //
    //
public:
    void
    TrimLeft(char const char_to_trim = ' ')
    {
        size_t const left_index = FindFirstNotOf(char_to_trim);
        if (left_index == INVALID_STRING_INDEX) {
            return;
        }
        // @todo(stdmatt): We are creating too much copies... Dec 20, 2020 
        __Container::operator=(SubString(left_index, Length()));
    }

    void
    TrimRight(char const char_to_trim = ' ')
    {
        size_t const right_index = FindLastIndexNotOf(char_to_trim);
        if (right_index == INVALID_STRING_INDEX) {
            return;
        }
        
        // @todo(stdmatt): We are creating too much copies... Dec 20, 2020 
        __Container::operator=(SubString(0, right_index + 1));
    }

    void
    Trim(char const char_to_trim = ' ')
    {
        TrimLeft (char_to_trim); 
        TrimRight(char_to_trim);
    }

    //
    //
    //
public:
    void 
    ToLower() 
    {
        size_t const len = Length();
        for (size_t i = 0; i < len; ++i) {
            __Container::operator[](i) = tolower(__Container::operator[](i));
        }
    }

    void 
    ToUpper() 
    {
        size_t const len = Length();
        for (size_t i = 0; i < len; ++i) {
            __Container::operator[](i) = toupper(__Container::operator[](i));
        }
    }

}; // class String


//
//
//
template <typename T>
ARK_FORCE_INLINE String ToString(T t) { return std::to_string(t); }

ARK_FORCE_INLINE String ToString(char const * const t) { return String(t); }
ARK_FORCE_INLINE String ToString(String const &t)      { return t; }

} // namespace ark
