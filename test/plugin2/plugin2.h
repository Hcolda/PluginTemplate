#pragma once
#include <extensionsystem/iplugin.h>
class Plugin2 : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "MyProject.Plugin" FILE "plugin2.json")
public:
    Plugin2();

    // IPlugin interface
public:
    bool initialize(const QStringList &arguments, QString &errorString);
    void extensionsInitialized();
    bool delayedInitialize();
    ExtensionSystem::PluginShutdownFlag aboutToShutdown();

protected:
    void initialize();
};

class Service
{
public:
    virtual ~Service() = default;
    virtual void print() = 0;
};
#define Service_iid "MyProject.Service"
QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(Service, Service_iid)
QT_END_NAMESPACE
class Service2 : public QObject, public Service
{
    Q_OBJECT
    Q_INTERFACES(Service)
    public:
    void print() override;
};
