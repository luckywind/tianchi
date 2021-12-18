// **************************************************************************
// Tianchi C++ library for Qt (open source)
// ��ع���Դ���
// ��Ȩ���� (C) ��ع���Դ��⿪����
// ��ȨЭ�飺���Ķ���ع���Դ��⸽������ȨЭ��
// **************************************************************************
// �ĵ�˵�����ַ���������ǿ����չ
// ==========================================================================
// ������־��
// ����         ��Ա        ˵��
// --------------------------------------------------------------------------
// 2013.04.16   ʥ������    ����
// 2021.12.18   XChinux     add Qt6 support
// ==========================================================================
/// @file String.cpp �ַ���������ǿ����չ

#include <tianchi/core/tcstring.h>

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
#else
#include <QTextCodec>
#endif

#include <QFile>
#include <QTextStream>

TcString::TcString()
{
}

QByteArray TcString::first(QByteArray& str, const QByteArray& split)
{
    QByteArray ret;
    int endOf = str.indexOf(split);
    if ( endOf == 0 )
    {
        str.remove(0, 1);
    }
    else
    if ( endOf > 0 )
    {
        ret = str.left(endOf);
        str.remove(0, endOf +1);
    }
    else
    {
        ret = str;
    }

    return ret;
}

int TcString::find(const QStringList& ss, const QString& s)
{
    return ss.indexOf(s);
}

int TcString::findOf(const QStringList& list, const QString& key)
{
    int ret = -1;
    for( int i=0;i<list.count();i++ )
    {
        if ( list.at(i).trimmed().compare(key.trimmed(), Qt::CaseInsensitive)==0 )
        {
            ret = i;
            break;
        }
    }
    return ret;
}

bool TcString::filter(const QString& text, const QStringList& filters)
{
    bool ret = filters.count() <= 0;
    foreach(QString s, filters)
    {
        s = s.trimmed();
        if ( ! s.isEmpty() )
        {
            if ( text.indexOf(s, Qt::CaseInsensitive) >= 0 )
            {
                ret = true;
                break;
            }
        }
    }
    return ret;
}

/// �ֽ��������������ա�Ӣ�ģ�
int TcString::splitHumanName(QString full, QString& sur, QString& real, QString& english)
{
    const char *str = "ŷ��\n̫ʷ\n��ľ\n�Ϲ�\n˾��\n����\n����\n�Ϲ�\n��ٹ\n"
                      "����\n�ĺ�\n���\nξ��\n����\n����\n�̨\n�ʸ�\n����\n"
                      "���\n��ұ\n̫��\n����\n����\nĽ��\n����\n����\n����\n"
                      "����\n˾ͽ\n����\n˾��\n����\n�ӳ�\n����\n˾��\n����\n"
                      "����\n���\n����\n����\n���\n����\n�׸�\n����\n�ذ�\n"
                      "�й�\n��ԯ\n���\n�θ�\n����\n����\n����\n����\n����\n"
                      "΢��\n����\n����\n����\n����\n����\n����\n����\n��ɽ\n"
                      "����\n����\n����\n����\n����\n����\n����\n����\n���\n"
                      "����\n����\n����\n�ٳ�\n����\n��ɣ\n��ī\n����\n��ʦ\n";
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    QString surs = QStringDecoder(QStringConverter::System).decode(QByteArray(str));
#else
    QString surs = QTextCodec::codecForLocale()->toUnicode(str);
#endif

    QStringList doubleSurnames = surs.split("\n");

    full = full.trimmed();

    int ret = 0;
    if ( ! full.isEmpty() )
    {
        if ( full.length() != full.toLocal8Bit().length() )
        {// ����
            foreach(QString s, doubleSurnames)
            {
                if ( ! s.isEmpty() && full.startsWith(s) )
                {
                    sur = s;
                    ret = 2;
                    break;
                }
            }
            if ( ret != 2 )
            {
                sur = full.mid(0, 1);
                ret = 1;
            }
            real = full.mid(sur.length());
        }else
        {// Ӣ����
            QStringList ss = full.split(" ", 
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
                    Qt::SkipEmptyParts
#else
                    QString::SkipEmptyParts
#endif
                    );
            english = "";
            for( int i=0;i<ss.count();i++ )
            {
                QString t = ss.at(i);
                if ( i == ss.count()-1 )
                {
                    sur = t;
                }else
                {
                    english += t + " ";
                }
            }
            english = english.trimmed();

            ret = 3;
        }
    }
    return ret;
}

QString TcString::getTextByIndex(const char* strings, int index)
{
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    QString str = QStringDecoder(QStringConverter::System).decode(QByteArray(strings));
    QStringList ss = str.split("\n", Qt::SkipEmptyParts);
#else
    QStringList ss = QTextCodec::codecForLocale()->toUnicode(
            strings).split("\n", QString::SkipEmptyParts);
#endif

    return ss.at(index);
}

// class StringList
TcStringList::TcStringList()
{
    #if defined(Q_OS_WIN)
        m_lineBreak = "\r\n";
    #elif defined(Q_OS_LINUX)
        m_lineBreak = "\n";
    #elif defined(Q_OS_MAC)
        m_lineBreak = "\r";
    #else
        m_lineBreak = "\n";
    #endif
}

bool TcStringList::loadFrom(const QString& fileName)
{
    bool ret = false;
    QFile f(fileName);
    if ( f.open(QFile::Text | QFile::ReadOnly) )
    {
        clear();
        QTextStream in(&f);
        while(!in.atEnd())
        {
            this->append(in.readLine());
        }
        f.close();
        ret = false;
    }
    return ret;
}

bool TcStringList::saveTo(const QString& fileName)
{
    bool ret = false;
    QFile f(fileName);
    if ( f.open(QFile::Text | QFile::WriteOnly) )
    {
        QTextStream out(&f);
        foreach(QString s, *this)
        {
            out<<s<<lineBreak();
        }
        f.close();
    }
    return ret;
}
