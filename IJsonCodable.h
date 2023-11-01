//
// IJsonCodable.h
// 2023-10-21
// Ivan Konishchev
//

/*
 *  Объекты которые иплементируют данный интерфес должны будут переопределить
 *  методы для кодирования и декодирования себя в QByteArray;
 */

#pragma once
#include <QJsonDocument>
#include <QJsonObject>

class IJsonCodable
{
public:
    virtual QJsonObject encodeToJson() = 0;
    virtual void decodeFromJson(const QJsonObject &data) = 0;
};
