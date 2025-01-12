#include <QApplication>
#include <QWidget>
#include <extensionsystem/pluginmanager.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ExtensionSystem::PluginManager::instance().setPluginIID("MyProject.Plugin");
    ExtensionSystem::PluginManager::instance().setPluginPaths(QStringList() << "");
    ExtensionSystem::PluginManager::instance().readPluginPaths();
    ExtensionSystem::PluginManager::instance().loadPlugins();
    QWidget w;
    w.show();
    QObject::connect(&a, &QCoreApplication::aboutToQuit,
                     &ExtensionSystem::PluginManager::instance(), &ExtensionSystem::PluginManager::shutdown);
    return a.exec();
}
