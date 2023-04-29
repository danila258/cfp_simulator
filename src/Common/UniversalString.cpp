#include "UniversalString.h"

UniversalString::UniversalString(UniversalString&& other) noexcept : _str( std::move(other._str) )
{}

UniversalString& UniversalString::operator=(const UniversalString& other)
{
    if (this == &other)
    {
        return *this;
    }

    UniversalString copy(other);
    *this = std::move(copy);

    return *this;
}

UniversalString& UniversalString::operator=(UniversalString&& other) noexcept
{
    _str = std::move(other._str);
    return *this;
}

UniversalString::UniversalString(const std::string& str) : _str( str.data() )
{}

UniversalString::UniversalString(const QString& str) : _str(str)
{}

UniversalString::UniversalString(QString&& str) : _str( std::move(str) )
{}

UniversalString::UniversalString(const char* str) : _str(str)
{}

UniversalString& UniversalString::operator=(const std::string& str)
{
    _str = QString::fromStdString(str);
    return *this;
}

UniversalString& UniversalString::operator=(const QString& str)
{
    _str = str;
    return *this;
}

UniversalString& UniversalString::operator=(QString&& str)
{
    _str = std::move(str);
    return *this;
}

UniversalString& UniversalString::operator=(const char* str)
{
    _str = str;
    return *this;
}

UniversalString::operator QString() const
{
    return _str;
}

UniversalString::operator std::string() const
{
    return _str.toStdString();
}

QString* UniversalString::operator->() const
{
    return const_cast<QString*>(&_str);
}

bool UniversalString::operator==(const std::string& str) const
{
    return _str.toStdString() == str;
}

bool UniversalString::operator==(const char* str) const
{
    return _str == str;
}