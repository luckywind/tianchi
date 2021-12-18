// **************************************************************************
// Tianchi C++ library for Qt (open source)
// ��ع���Դ���
// ��Ȩ���� (C) ��ع���Դ��⿪����
// ��ȨЭ�飺���Ķ���ع���Դ��⸽������ȨЭ��
// **************************************************************************
// �ĵ�˵����Oracle ���Ӧ��
// ==========================================================================
// ������־��
// ����         ��Ա        ˵��
// --------------------------------------------------------------------------
// 2013.06.04   XChinux     ����
// 2013.06.04   XChinux     ����ODBCDSN()����
// 2021.12.18   XChinux     add Qt6 support
// ==========================================================================
/// @file tcoracle.cpp SQL Server ���Ӧ��

#include <tianchi/sql/tcoracle.h>
#include <tianchi/file/tcfile.h>
#include <QSettings>
#include <algorithm>
/**
 * get Oracle ODBC drivers name and version, every item such as:
 * -------------------------------------------------------------
 *     10.00/SQL Server Native Client 10.0
 *     09.00/SQL Native Client
 *     03.50/SQL Server
 * -------------------------------------------------------------
 * @author  XChinux<XChinux@163.com>
 * @final   2013-04-18
 * @return  QStringList         desc sorted ver/drivername lists
 */
QStringList TcOracle::availableODBCDrivers()
{
    QStringList slDrivers;
#ifdef Q_OS_WIN
    QSettings  sts("HKEY_LOCAL_MACHINE\\SOFTWARE\\ODBC\\ODBCINST.INI"
            "\\ODBC Drivers", QSettings::NativeFormat);
    QStringList slKeys = sts.allKeys();

    QStringList slKeys2;
    for (int i = slKeys.size() - 1; i > -1; i--)
    {
        if (slKeys[i].left(7) == "Oracle ")
        {
            slKeys2 << slKeys[i];
        }
    }
    slKeys2.removeDuplicates();
    QListIterator<QString> it(slKeys2);

    while (it.hasNext())
    {
        QString strV = it.next();
        if (sts.value(strV).toString() == "Installed")
        {
            QSettings sts2("HKEY_LOCAL_MACHINE\\SOFTWARE\\ODBC\\ODBCINST.INI\\"
                + strV, QSettings::NativeFormat);
            strV.prepend(TcFile::fileVersion(
                        sts2.value("Driver").toString()) + "/");
            slDrivers << strV;
        }
    }
    std::sort(slDrivers.begin(), slDrivers.end(), std::greater<QString>());
#endif
    return slDrivers;
}

/// @brief  ����ODBC DSN�ַ���
/// @author XChinux<XChinux@163.com>
/// @final  2013-06-04
/// @param  driver  ����ODBC��������
/// @param  tns     TNS Service Name
/// @param  user    �����û���
/// @param  pass    ��������
/// @return ����DSN�ַ���
QString TcOracle::ODBCDSN(const QString &driver,
            const QString &tns,
            const QString &user /*= QString() */,
            const QString &pass /*= QString()*/)
{
    QString strDSN = QString("DRIVER={%1};DBQ=%2;SERVER=%2")
        .arg(driver).arg(tns);
    if (!user.isEmpty())
    {
        strDSN += ";UID=" + user;
    }
    if (!pass.isEmpty())
    {
        strDSN += ";PWD=" + pass;
    }
    return strDSN;
}
