#pragma once
#include <extensionsystem/iplugin.h>
class Plugin1 : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "MyProject.Plugin" FILE "plugin1.json")
public:
    Plugin1();

    // IPlugin interface
public:
    bool initialize(const QStringList &arguments, QString &errorString);
    void extensionsInitialized();
    bool delayedInitialize();
    ExtensionSystem::PluginShutdownFlag aboutToShutdown();

protected:
    void initialize();
};
