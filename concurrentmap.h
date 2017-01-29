#ifndef CONCURRENTMAP_H
#define CONCURRENTMAP_H

#include <QMap>
#include <QMutex>
#include <functional>
#include <unordered_map>

namespace chatterino {

template <typename TKey, typename TValue>
class ConcurrentMap
{
public:
    ConcurrentMap()
        : map()
    {
        this->mutex = new QMutex();
    }

    bool
    tryGet(const TKey &name, TValue &value) const
    {
        this->mutex->lock();
        auto a = map.find(name);
        if (a == map.end()) {
            this->mutex->unlock();
            return false;
        }
        value = a.value();
        this->mutex->unlock();
        return true;
    }

    void
    getOrAdd(const TKey &name, TValue &value, std::function<TValue()> addLambda)
    {
        this->mutex->lock();
        auto a = map.find(name);

        if (a == map.end()) {
            value = addLambda();
            map.insert(name, value);
            this->mutex->unlock();
        }

        value = a.value();

        this->mutex->unlock();
    }

    void
    clear()
    {
        this->mutex->lock();
        map.clear();
        this->mutex->unlock();
    }

    void
    insert(const TKey &name, const TValue &value)
    {
        this->mutex->lock();
        map.insert(name, value);
        this->mutex->unlock();
    }

private:
    QMutex *mutex;
    QMap<TKey, TValue> map;
};
}

#endif  // CONCURRENTMAP_H
