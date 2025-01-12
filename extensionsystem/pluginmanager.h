#pragma once

#include <QString>
#include <QObject>
#include <QReadWriteLock>
#include <QPointer>
#include <QSet>
#include <QEventLoop>
#include <QTimer>
#include <QQueue>
#include "pluginspecification.h"

namespace ExtensionSystem
{
class EXTENSIONSYSTEM_EXPORT PluginManager : public QObject
{
    Q_OBJECT
public:
    static PluginManager &instance();
    static QString platformName();
    QString pluginIID() const;
    void setPluginIID(const QString &newPluginIID);

    void addObject(QObject *obj);
    void removeObject(QObject *obj);
    QVector<QPointer<QObject>> allObjects();

    template <typename T> T *getObject()
    {
        const QVector<QPointer<QObject>> all = allObjects();
        for (QObject *obj : all) {
            if (T *result = qobject_cast<T *>(obj))
                return result;
        }
        return nullptr;
    }
    QReadWriteLock *listLock();

    void loadPlugins();
    const QVector<PluginSpecification *> loadQueue();
    void setPluginPaths(const QStringList &paths);
    void readPluginPaths();
    QStringList pluginFiles(const QStringList &pluginPaths);
    void shutdown();
    void stopAll();
    void deleteAll();
private:
    PluginManager();
    bool loadQueue(PluginSpecification *spec,
                   QVector<PluginSpecification *> &queue,
                   QVector<PluginSpecification *> &circularityCheckQueue);
    void loadPlugin(PluginSpecification *spec, PluginState destState);
    void startDelayedInitialize();
    QString m_pluginIID;
    mutable QReadWriteLock m_lock;
    QVector<QPointer<QObject>> m_allObjects;
    QSet<PluginSpecification *> m_asynchronousPlugins;
    QVector<PluginSpecification *> m_pluginSpecs;
    QEventLoop *m_shutdownEventLoop = nullptr;
    QHash<QString, QVector<PluginSpecification *>> m_pluginCategories;
    QQueue<PluginSpecification *> m_delayedInitializeQueue;
    QTimer m_delayedInitializeTimer;
    bool m_isInitializationDone = false;
    bool m_isShuttingDown = false;
    QStringList m_pluginPaths;
signals:
    void objectAdded(QObject *obj);
    void aboutToRemoveObject(QObject *obj);
    void pluginsChanged();
    void initializationDone();
};
} // namespace ExtensionSystem
