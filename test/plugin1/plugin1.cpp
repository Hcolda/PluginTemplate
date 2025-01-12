#include "plugin1.h"
#include <print>
#include <iostream>
#include "plugin2/plugin2.h"
#include "extensionsystem/pluginmanager.h"
Plugin1::Plugin1() {}

bool Plugin1::initialize(const QStringList &arguments, QString &errorString)
{
    std::print("Plugin1::initialize\n");
    std::cout.flush();
    auto ptr = ExtensionSystem::PluginManager::instance().getObject<Service>();
    if(ptr)
       ptr->print();
    else
        std::print("Service not found\n");
    return true;
}

void Plugin1::extensionsInitialized()
{

}

bool Plugin1::delayedInitialize()
{
    return false;
}

ExtensionSystem::PluginShutdownFlag Plugin1::aboutToShutdown()
{
    std::print("Plugin1::aboutToShutdown\n");
    std::cout.flush();
    return ExtensionSystem::PluginShutdownFlag::SynchronousShutdown;
}

void Plugin1::initialize()
{

}
