#ifndef OBJECTS_H
#define OBJECTS_H

#include "ObjectFactory.h"
#include "UniversalString.h"

#include "task/TRTThread.h"
#include "sync/TRTMutex.h"
#include "sync/TRTBinSemaphore.h"
#include "data/TRTQue.h"
#include "timer/TRTSysTimer.h"
#include "sync/TRTEvent.h"
#include "sync/TRTCondvar.h"
#include "data/TRTSharedMemory.h"

#define DELETER(T) [](void* ptr){ delete static_cast<T>(ptr); }
#define CALL(T, func) static_cast<T>(_object.get())->func


void checkArgsCount(const std::vector<UniversalString>& args, size_t size);


class TRTMutexObject : public Object
{
public:
    explicit TRTMutexObject(const std::vector<UniversalString>& args);
    void call(const UniversalString& funcName, const std::vector<UniversalString>& args) override;
    static const UniversalString& staticTypeName();

private:
    static const UniversalString _typeName;   // TRTMutex
};


class TRTBinSemaphoreObject : public Object
{
public:
    explicit TRTBinSemaphoreObject(const std::vector<UniversalString>& args);
    void call(const UniversalString& funcName, const std::vector<UniversalString>& args) override;
    static const UniversalString& staticTypeName();

private:
    static const UniversalString _typeName;   // TRTBinSemaphore
};


class TRTQueObject : public Object
{
public:
    explicit TRTQueObject(const std::vector<UniversalString>& args);
    void call(const UniversalString& funcName, const std::vector<UniversalString>& args) override;
    static const UniversalString& staticTypeName();

private:
    static const UniversalString _typeName;   // TRTQue
};


class TRTSysTimerObject : public Object
{
public:
    explicit TRTSysTimerObject(const std::vector<UniversalString>& args);
    void call(const UniversalString& funcName, const std::vector<UniversalString>& args) override;
    static const UniversalString& staticTypeName();

private:
    static const UniversalString _typeName;   // TRTSysTimer
};


class TRTEventObject : public Object
{
public:
    explicit TRTEventObject(const std::vector<UniversalString>& args);
    void call(const UniversalString& funcName, const std::vector<UniversalString>& args) override;
    static const UniversalString& staticTypeName();

private:
    static const UniversalString _typeName;   // TRTEvent
};


class TRTCondVarObject : public Object
{
public:
    explicit TRTCondVarObject(const std::vector<UniversalString>& args);
    void call(const UniversalString& funcName, const std::vector<UniversalString>& args) override;
    static const UniversalString& staticTypeName();

private:
    static const UniversalString _typeName;   // TRTCondVar
};


class TRTSharedMemoryObject : public Object
{
public:
    explicit TRTSharedMemoryObject(const std::vector<UniversalString>& args);
    void call(const UniversalString& funcName, const std::vector<UniversalString>& args) override;
    static const UniversalString& staticTypeName();

private:
    static const UniversalString _typeName;   // TRTSharedMemory
};


#endif //OBJECTS_H
