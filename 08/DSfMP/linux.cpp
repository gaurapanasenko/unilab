#include "linux.h"

Linux::Linux(const QString &name, const QString &family)
    : m_name(name), m_family(family)
{

}

QString Linux::name() const
{
    return m_name;
}

void Linux::setName(const QString &name)
{
    m_name = name;
    //emit nameChanged();
}

QString Linux::family() const
{
    return m_family;
}

void Linux::setFamily(const QString &family)
{
    m_family = family;
    //emit familyChanged();
}
