// Header
#include "String.hpp"
#include "Math.hpp"

// Usings
using namespace ark;

bool 
String::StartsWith(String const &needle) const
{
    if(IsEmpty()) {
        return false;
    }

    size_t const curr_len   = Length();
    size_t const needle_len = needle.Length();

    if(needle.Length() > curr_len) {
        return false;
    }

    for(size_t i = 0; i < needle_len; ++i) {
        if(__Container::operator[](i) != needle[i]) {
            return false;
        }
    }

    return true;
}


bool 
String::StartsWith(char const needle) const
{
    if(IsEmpty() || needle) {
        return false;
    }
    
    return __Container::operator[](0) == needle;
}

bool 
String::EndsWith(String const &needle) const
{
    if(IsEmpty() || needle.IsEmpty()) {
        return false;
    }

    size_t const curr_len   = Length();
    size_t const needle_len = needle.Length();

    if(needle.Length() > curr_len) {
        return false;
    }

    for(size_t i = 1; i < needle_len; ++i) {
        if(__Container::operator[](curr_len- i) != needle[needle_len - i]) {
            return false;
        }
    }

    return true;
}

bool 
String::EndsWith(char const needle) const
{
    if(IsEmpty()) {
        return false;
    }
    return __Container::back() == needle;
}


size_t 
String::FindIndexOf(char const needle, size_t start_index /* = 0 */) const
{
    size_t const curr_len = Length();
    if(IsEmpty() || start_index >= curr_len) {
        return INVALID_STRING_INDEX;
    }

    for(size_t i = start_index; i < curr_len; ++i) {
        if(__Container::operator[](i) == needle) {
            return i;
        }
    }

    return INVALID_STRING_INDEX;
}


size_t 
String::FindFirstNotOf(char const needle, size_t start_index /* = 0 */) const
{
    size_t const curr_len = Length();
    if(IsEmpty() || start_index >= curr_len) {
        return INVALID_STRING_INDEX;
    }

    for(size_t i = start_index; i < curr_len; ++i) {
        if(__Container::operator[](i) != needle) {
            return i;
        }
    }

    return INVALID_STRING_INDEX;
}

size_t 
String::FindLastIndexOf(char const needle, size_t start_index /* = 0 */) const
{
    size_t const curr_len = Length();
    if(IsEmpty() || start_index >= curr_len) {
        return INVALID_STRING_INDEX;
    }

    for(size_t i = curr_len -start_index -1; i >= 0; --i) {
        if(__Container::operator[](i) == needle) {
            return i;
        }
    }

    return INVALID_STRING_INDEX;
}

size_t 
String::FindLastIndexNotOf(char const needle, size_t start_index /* = 0 */) const
{
    size_t const curr_len = Length();
    if(IsEmpty() || start_index >= curr_len) {
        return INVALID_STRING_INDEX;
    }

    for(size_t i = curr_len -start_index -1; i >= 0; --i) {
        if(__Container::operator[](i) != needle) {
            return i;
        }
    }

    return INVALID_STRING_INDEX;
}

Array<String>
String::Split(char const separator) const
{
    Array<String> splits;
    if(IsEmpty()) {
        return splits;
    }
    
    size_t index = 0;
    while(true) {
        size_t const new_index  = FindIndexOf(separator, index);
        String const new_string = SubString(index, new_index);
        splits.PushBack(new_string);

        if(new_index == INVALID_STRING_INDEX) {
            return splits;
        }

        index = (new_index + 1);
    }
}

Array<String>
String::Split(Array<char> const &separators) const
{
    Array<String> splits;

    size_t index                  = 0;
    size_t const separators_count = separators.Count();
    size_t const this_len         = this->Length();

    while(true) {
        bool has_separator = false;
        size_t new_index   = INVALID_STRING_INDEX;
        for(size_t sep_i = 0; sep_i < separators_count; ++sep_i) {
            char const curr_separator = separators[sep_i];
            new_index = Math::Min(new_index, FindIndexOf(curr_separator, index));
            if(new_index != INVALID_STRING_INDEX) {
                has_separator = true;
            }
        }

        String const new_string = SubString(index, new_index);
        splits.PushBack(new_string);

        if(!has_separator) {
            return splits;
        }

        index = (new_index + 1);
    }

    return {};
}
