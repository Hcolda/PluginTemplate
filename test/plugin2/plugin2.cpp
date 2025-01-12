#include "plugin2.h"
#include <extensionsystem/pluginmanager.h>
#include <print>
#include <iostream>
Plugin2::Plugin2() {}

bool Plugin2::initialize(const QStringList &arguments, QString &errorString)
{
    std::print("Plugin2::initialize\n");
    std::cout.flush();
    ExtensionSystem::PluginManager::instance().addObject(new Service2);
    std::print("Service2 added\n");
    return true;
}

void Plugin2::extensionsInitialized()
{

}

bool Plugin2::delayedInitialize()
{
    return false;
}

ExtensionSystem::PluginShutdownFlag Plugin2::aboutToShutdown()
{
    std::print("Plugin2::aboutToShutdown\n");
    std::cout.flush();
    return ExtensionSystem::PluginShutdownFlag::SynchronousShutdown;
}

void Plugin2::initialize()
{

}

void Service2::print()
{
    std::print("Service2::print\n");
    std::cout.flush();
}
