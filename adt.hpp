/** adt.h
 * ASCII delimited text
 *
 * Uses ASCII 0x1e (unit separator) for separating fields in a record and
 * ASCII 0x1f (record separator) to separater records.
 * 
**/

#ifndef _ADT_H_
#define _ADT_H_

#include <iostream>
#include <deque>
#include <string>
#include <stdexcept>
#include <sstream>
#include <fstream>

namespace adt
{

// Forward declaration.
class Record;

// The field separator character.
const char FIELDSEP = static_cast<char>(0x1e);

// The record separator character.
const char RECORDSEP = static_cast<char>(0x1f);

class Record
{
public:
    // Insert a field
    Record& operator<<(const std::string& s)
    {
        m_fields.push_back(s);
        return *this;
    }

    // Extract a field
    Record& operator>>(std::string& s)
    {
        if (m_fields.empty())
        {
            throw std::runtime_error("Read from an empty record");
        }
        s = m_fields.front();
        m_fields.pop_front();
        return *this;
    }
    
    template<class T>
    Record& operator<<(const T& d)
    {
        std::stringstream ss;
        ss << d;
        *this << ss.str();
        return *this;
    }

    template<class T>
    Record& operator>>(T& d)
    {
        std::string s;
        *this >> s;
        std::stringstream ss { s };
        ss >> d;
        return *this;
    }
    
    // Read in a record
    friend std::istream& operator>>(std::istream& in, Record& r);

    // Write out a record
    friend std::ostream& operator<<(std::ostream& out, Record& r);
    
    // Clear all fields
    void clear()
    {
        m_fields.clear();
    }
    
    inline bool empty() const
    {
        return m_fields.empty();
    }

private:
    std::deque<std::string> m_fields;
};

std::ostream& operator<<(std::ostream& out, Record& r)
{
    auto end_idx = r.m_fields.size() - 1;
    decltype(end_idx) idx = 0;
    for (auto& field : r.m_fields)
    {
        out << field;
        // Have to do this as there is no "standard" way to achieve end-clean
        // implodes in C++.
        if (idx != end_idx)
        {
            out << FIELDSEP;
        }
        ++idx;
    }
    out << RECORDSEP;
    return out;
}

std::istream& operator>>(std::istream& in, Record& r)
{
    while (true)
    {
        std::stringstream ss;
        char c { static_cast<char>(0) };
        
        // Read character by character till we hit end of stream,
        // end of field or end of record.
        bool eof = true;
        while (in >> c)
        {
            if (c == FIELDSEP || c == RECORDSEP)
            {
                eof = false;
                break;
            }
            ss << c;
        }

        r.m_fields.push_back(ss.str());
        
        // We are done if we've read in a full record.
        if (eof || c == RECORDSEP)
        {
            break;
        }
    }
    return in;
}

} // namespace adt

#endif // _ADT_H_
