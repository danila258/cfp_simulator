#ifndef UNIVERSALSTRING_H
#define UNIVERSALSTRING_H

#include <string>
#include <QString>

// implicit convert to std::string and QString
// inside contains QString
// for call QString methods use operator -> not (.)
struct UniversalString
{
    UniversalString() = default;

    UniversalString(const UniversalString& other) = default;
    UniversalString(UniversalString&& other) noexcept;

    UniversalString& operator=(const UniversalString& other);
    UniversalString& operator=(UniversalString&& other) noexcept;

    UniversalString(const std::string& str);
    UniversalString(const QString& str);
    UniversalString(QString&& str);
    UniversalString(const char* str);

    UniversalString& operator=(const std::string& str);
    UniversalString& operator=(const QString& str);
    UniversalString& operator=(QString&& str);
    UniversalString& operator=(const char* str);

    operator QString() const;
    operator std::string() const;

    QString* operator->() const;

    bool operator==(const std::string& str) const;
    bool operator==(const char* str) const;

private:
    QString _str;
};


#endif //UNIVERSALSTRING_H
