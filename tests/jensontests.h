/****************************************************************************

 Copyright (c) 2014, Hans Robeers
 All rights reserved.

 BSD 2-Clause License

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

****************************************************************************/

#ifndef JENSONTESTS_H
#define JENSONTESTS_H

#include <QObject>
#include <QUuid>
#include "src/jenson.h"

class JensonTests : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testSerialization();
    void testCustomSerialization();
    void testSerializationFailures();
    void testOnDeserialized();
};


//
// Memory leak testing (testing sptr's deleter)
//

struct cntr
{
    QList<QObject*> objList;
    bool enabled = false;
    void inc(QObject* obj) { if (enabled) objList.append(obj); }
    void dec(QObject* obj) { objList.removeAll(obj); }
    ~cntr();
};

static cntr OBJ_CNT;


//
// Classes for testing
//

class CustomSerializable : public QObject
{
    Q_OBJECT

public:
    qreal x;

    CustomSerializable() : x(5) { OBJ_CNT.inc(this); }

    virtual ~CustomSerializable() { OBJ_CNT.dec(this); }
};

class CustomSerializableSerializer : public jenson::JenSON::CustomSerializer<CustomSerializable>
{
protected:
    virtual QJsonValue serializeImpl(const CustomSerializable *object) const override
    {
        QJsonObject retVal;
        QJsonValue val(object->x / 2);
        retVal.insert("custom", val);
        return retVal;
    }
    virtual sptr<CustomSerializable> deserializeImpl(const QJsonValue *jsonValue, QString* /*unused*/) const override
    {
        sptr<CustomSerializable> retVal(new CustomSerializable());
        qreal x = jsonValue->toObject().value("custom").toDouble() + 5;
        retVal->x = x;
        return retVal;
    }
};
CUSTOMSERIALIZABLE(CustomSerializable, CustomSerializableSerializer, cserial)

class CustomContainer : public QObject
{
    Q_OBJECT

    Q_PROPERTY(CustomSerializable* nested READ nested WRITE setNested)

private:
    sptr<CustomSerializable> _nested;

public:
    Q_INVOKABLE CustomContainer() { _nested.reset(new CustomSerializable()); OBJ_CNT.inc(this); }
    CustomSerializable* nested() { return _nested.get(); }
    void setNested(CustomSerializable* nested) { _nested.reset(nested); }

    virtual ~CustomContainer() { OBJ_CNT.dec(this); }
};
SERIALIZABLE(CustomContainer, cContainer)

class Nestedobject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString someString READ someString WRITE setSomeString)
    Q_PROPERTY(int random READ random)

private:
    QString _someString;
    int _random;

public:
    Q_INVOKABLE Nestedobject() { _random = (int)this; OBJ_CNT.inc(this); }

    virtual ~Nestedobject() { OBJ_CNT.dec(this); }

    QString someString() const { return _someString; }
    int random() const { return _random; }

    void setSomeString(const QString &someString) { _someString = someString; }
};
SERIALIZABLE(Nestedobject, nObj)

class SingleProperty : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUuid someUuid READ someUuid WRITE setSomeUuid)

private:
    QUuid _someUuid;

public:
    Q_INVOKABLE SingleProperty() { _someUuid = QUuid::createUuid(); OBJ_CNT.inc(this); }

    virtual ~SingleProperty() { OBJ_CNT.dec(this); }

    QUuid someUuid() const { return _someUuid; }

    void setSomeUuid(const QUuid &someUuid) { _someUuid = someUuid; }
};
SERIALIZABLE(SingleProperty, sProp)

class DerivedSingleProperty : public SingleProperty
{
    Q_OBJECT

public:
    Q_INVOKABLE DerivedSingleProperty() : SingleProperty() { OBJ_CNT.inc(this); }

    virtual ~DerivedSingleProperty() { OBJ_CNT.dec(this); }
};
SERIALIZABLE(DerivedSingleProperty, dProp)

class OnDeserialized : public SingleProperty
{
    Q_OBJECT

public:
    Q_INVOKABLE OnDeserialized() : SingleProperty() { OBJ_CNT.inc(this); }

    bool _onDeserializedCalled = false;
    Q_INVOKABLE void onDeserialized() { _onDeserializedCalled = true; }

    virtual ~OnDeserialized() { OBJ_CNT.dec(this); }
};
SERIALIZABLE(OnDeserialized, onDeserial)

class Testobject : public QObject
{
    Q_OBJECT

    // Convenience JENSON_PROPERTY_GETSET macros
    JENSON_PROPERTY_GETSET(qreal, x)
    JENSON_PROPERTY_GETSET(Nestedobject*, nestedObj)

    // Standard Q_PROBERTY macros
    Q_PROPERTY(qreal y READ y WRITE setY)
    Q_PROPERTY(QString optionalStr READ optionalStr WRITE setOptionalStr RESET initOptionalStr)
    Q_PROPERTY(SingleProperty* singleProp READ singleProp WRITE setSingleProp)
    Q_PROPERTY(QVariantList list READ list WRITE setList)
    Q_PROPERTY(QVariantList intList READ intList WRITE setIntList)

private:
    qreal _y;
    QString _optionalStr;
    SingleProperty *_sProp;
    QList<std::shared_ptr<SingleProperty>> _list;
    QList<int> _intList;

    void init()
    {
        _nestedObj = new Nestedobject();
        _nestedObj->setParent(this);

        _sProp = new SingleProperty();
        _sProp->setParent(this);

        _list.append(std::shared_ptr<SingleProperty>(new SingleProperty()));
        _list.append(std::shared_ptr<SingleProperty>(new DerivedSingleProperty()));
        _list.append(std::shared_ptr<SingleProperty>(new SingleProperty()));

        _intList.append((int)this);
        _intList.append((int)&_intList);
    }

public:
    Q_INVOKABLE Testobject() : _x(0), _y(0), _optionalStr("") { init(); OBJ_CNT.inc(this); }
    Testobject(qreal x, qreal y) : _x(x), _y(y), _optionalStr("") { init(); OBJ_CNT.inc(this); }

    virtual ~Testobject() { OBJ_CNT.dec(this); }

    QList<std::shared_ptr<SingleProperty>> *internalList() { return &_list; }

    // Q_PROPERTY getters
    qreal y() const { return _y; }
    QString optionalStr() const { return _optionalStr; }
    SingleProperty* singleProp() { return _sProp; }
    QVariantList list()
    {
        QVariantList retVal;
        foreach (const std::shared_ptr<SingleProperty> &item, _list)
        {
            QVariant var = QVariant::fromValue(item.get());
            retVal.append(var);
        }
        return retVal;
    }
    QVariantList intList()
    {
        QVariantList retVal;
        foreach (int item, _intList) { retVal.append(item); }
        return retVal;
    }

    // Q_PROPERTY setters
    void setY(const qreal y) { _y = y; }
    void setOptionalStr(const QString &str) { _optionalStr = str; }
    void setSingleProp(SingleProperty *sProp) { _sProp = sProp; }
    void setList(QVariantList list)
    {
        _list.clear();
        foreach (const QVariant &item, list)
        {
            SingleProperty *obj = qvariant_cast<SingleProperty*>(item);
            _list.append(std::shared_ptr<SingleProperty>(obj));
        }
    }
    void setIntList(QVariantList list)
    {
        _intList.clear();
        foreach (const QVariant &item, list) { _intList.append(item.toInt()); }
    }

    // Q_PROPERTY resetters
    void initOptionalStr() { _optionalStr = "initialized"; }
};
SERIALIZABLE(Testobject, tObj)

#endif // JENSONTESTS_H
