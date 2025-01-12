#pragma once
#include <QString>
#include <QVector>
#include <QStringList>
#include <QHash>
#include <optional>
#include <QPluginLoader>
#include <QJsonObject>
#include "extensionsystemglobal.h"
#include "iplugin.h"


namespace ExtensionSystem {
enum PluginState
{
    Invalid,
    Read,
    Resolved,
    Loaded,
    Initialized,
    Running,
    Stopped,
    Deleted
};

struct PluginDependency
{
    enum class Type {
        Required,
        Optional,
        Test
    };

    PluginDependency() : type(Type::Required) {}

    friend size_t qHash(const PluginDependency &value);

    QString name;
    QString version;
    Type type;
    bool operator==(const PluginDependency &other) const;
    QString toString() const;
};

struct EXTENSIONSYSTEM_EXPORT PluginArgumentDescription
{
    QString name;
    QString parameter;
    QString description;
};

class IPlugin;
class EXTENSIONSYSTEM_EXPORT PluginSpecification
{
public:
    QString name() const;
    QString version() const;
    QString compatVersion() const;
    QString description() const;
    QString license() const;
    IPlugin *plugin() const;
    bool isRequired() const;
    bool isExperimental() const;
    bool delayedInitialize();
    PluginState state() const;
    QVector<PluginDependency> dependencies() const;
    QHash<PluginDependency, PluginSpecification *> dependencySpecifications() const;
    QStringList arguments() const;
    QVector<PluginArgumentDescription> argumentDescriptions() const;

    bool initializeExtensions();

    void addArguments(const QStringList &arguments);
    void setArguments(const QStringList &arguments);
    bool read(const QString &filePath);
    bool read(const QStaticPlugin &plugin);
    void reset();
    bool loadLibrary();

    std::optional<QString> errorString() const;
    bool hasError() const;

    void kill();
    bool initializePlugin();
    PluginShutdownFlag stop();

    bool provides(const QString &pluginName, const QString &version) const;
    static PluginSpecification *readPlugin(const QString &filePath);
    static PluginSpecification *readPlugin(const QStaticPlugin &plugin);
    static int versionCompare(const QString &version1, const QString &version2);
private:
    friend class PluginManager;
    bool readMetaData(const QJsonObject &pluginMetaData);
    bool reportError(const QString &errorString);
    bool resolveDependencies(const QVector<PluginSpecification *> &specs);
    QString m_name;
    QString m_version;
    QString m_compatVersion;
    QString m_description;
    QString m_license;
    QString m_filePath;
    IPlugin *m_plugin;
    bool m_required = false;
    bool m_experimental = false;
    PluginState m_state;
    QVector<PluginDependency> m_dependencies;
    QHash<PluginDependency, PluginSpecification *> m_dependencySpecifications;
    QStringList m_arguments;
    QVector<PluginArgumentDescription> m_argumentDescriptions;
    std::optional<QPluginLoader> m_loader;
    std::optional<QString> m_errorString;

    std::optional<QStaticPlugin> m_staticPlugin;
};

} // namespace ExtensionSystem
