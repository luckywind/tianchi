// **************************************************************************
// Tianchi C++ library for Qt (open source)
// ��ع���Դ���
// ��Ȩ���� (C) ��ع���Դ��⿪����
// ��ȨЭ�飺���Ķ���ع���Դ��⸽������ȨЭ��
// **************************************************************************
// �ĵ�˵����������Ԫ���˵�Ԫ��ʹ�� namespace Tianchi
// ==========================================================================
// ������־��
// ����         ��Ա        ˵��
// --------------------------------------------------------------------------
// 2013.04.15   ʥ������    ����
// 2021.12.18   XChinux     add Qt6 support
// ==========================================================================
/// @file Common.cpp ������Ԫ

#include <tianchi/core/tccommon.h>

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
#else
QTextCodec* initQS(const QByteArray& name)
{
    QTextCodec* textCodec=QTextCodec::codecForName(name);
    QTextCodec::setCodecForLocale(textCodec);
    return textCodec;
}
#endif

QString YYYY_MM_DD_HH_MM_SS_ZZZ()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
}

QString YYYY_MM_DD_HH_MM_SS()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
}

