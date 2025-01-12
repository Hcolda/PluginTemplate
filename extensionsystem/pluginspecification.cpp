#include "pluginspecification.h"
#include <QFileInfo>
#include <QHashFunctions>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDir>
#include <QLoggingCategory>
#include <utils/hostinfo.h>
#include "pluginmanager.h"
#include "extensionsystemtr.h"
#include "iplugin.h"
#include <nlohmann/json.hpp>
#include <print>
#include <iostream>


Q_LOGGING_CATEGORY(pluginLog, "extensionsystem", QtWarningMsg)

namespace ExtensionSystem {

namespace Constants
{
const char kIID[] = "IID";
const char kPluginMetadata[] = "MetaData";
const char kPluginName[] = "Name";
const char kPluginVersion[] = "Version";
const char kPluginCompatversion[] = "CompatVersion";
const char kPluginRequired[] = "Required";
const char kPluginExperimental[] = "Experimental";
const char kDescription[] = "Description";
const char kLicense[] = "License";
const char kDependencies[] = "Dependencies";
const char kDependencyName[] = "Name";
const char kDependencyVersion[] = "Version";
const char kDependencyType[] = "Type";
const char kDependencyTypeSoft[] = "optional";
const char kDependencyTypeHard[] = "required";
const char kDependencyTypeTest[] = "test";
const char kArguments[] = "Arguments";
const char kArgumentName[] = "Name";
const char kArgumentParameter[] = "Parameter";
const char kArgumentDescription[] = "Description";
}

QString PluginSpecification::name() const
{
    return m_name;
}

QString PluginSpecification::version() const
{
    return m_version;
}

QString PluginSpecification::compatVersion() const
{
    return m_compatVersion;
}

QString PluginSpecification::description() const
{
    return m_description;
}

QString PluginSpecification::license() const
{
    return m_license;
}

IPlugin *PluginSpecification::plugin() const
{
    return m_plugin;
}

bool PluginSpecification::isRequired() const
{
    return m_required;
}

bool PluginSpecification::isExperimental() const
{
    return m_experimental;
}

PluginState PluginSpecification::state() const
{
    return m_state;
}

QVector<PluginDependency> PluginSpecification::dependencies() const
{
    return m_dependencies;
}

QHash<PluginDependency, PluginSpecification *> PluginSpecification::dependencySpecifications() const
{
    return m_dependencySpecifications;
}

QStringList PluginSpecification::arguments() const
{
    return m_arguments;
}

QVector<PluginArgumentDescription> PluginSpecification::argumentDescriptions() const
{
    return m_argumentDescriptions;
}

bool PluginSpecification::initializeExtensions()
{
    if (m_errorString)
        return false;
    if (m_state != PluginState::Initialized) {
        if (m_state == PluginState::Running)
            return true;
        m_errorString = ::ExtensionSystem::Tr::tr(
            "Cannot perform extensionsInitialized because state != Initialized");
        return false;
    }
    if (!m_plugin) {
        m_errorString = ::ExtensionSystem::Tr::tr(
            "Internal error: have no plugin instance to perform extensionsInitialized");
        return false;
    }
    m_plugin->extensionsInitialized();
    m_state = PluginState::Running;
    return true;
}

void PluginSpecification::addArguments(const QStringList &arguments)
{
    m_arguments.append(arguments);
}

void PluginSpecification::setArguments(const QStringList &arguments)
{
    m_arguments = arguments;
}

bool PluginSpecification::read(const QString &filePath)
{
    reset();
    QFileInfo fileInfo(filePath);
    m_filePath = filePath;
    m_loader.emplace();
    if (Utils::HostInfo::isMacHost())
        m_loader->setLoadHints(QLibrary::ExportExternalSymbolsHint);
    m_loader->setFileName(filePath);
    if (m_loader->fileName().isEmpty()) {
        return false;
    }

    if (!readMetaData(m_loader->metaData()))
        return false;

    m_state = PluginState::Read;
    return true;
}

bool PluginSpecification::read(const QStaticPlugin &plugin)
{
    reset();
    m_staticPlugin = plugin;
    if (!readMetaData(plugin.metaData()))
        return false;

    m_state = PluginState::Read;
    return true;
}

void PluginSpecification::reset()
{
    m_name.clear();
    m_version.clear();
    m_compatVersion.clear();
    m_description.clear();
    m_license.clear();
    m_plugin = nullptr;
    m_required = false;
    m_experimental = false;
    m_state = PluginState::Invalid;
    m_dependencies.clear();
    m_dependencySpecifications.clear();
    m_arguments.clear();
    m_argumentDescriptions.clear();
    m_loader.reset();
    m_errorString.reset();
    m_staticPlugin.reset();
}

bool PluginSpecification::loadLibrary()
{
    if (m_errorString.has_value())
        return false;
    if (m_state != PluginState::Resolved) {
        if (m_state == PluginState::Loaded)
            return true;
        m_errorString =
            ::ExtensionSystem::Tr::tr("Loading the library failed because state != Resolved");
        return false;
    }
    if (m_loader && !m_loader->load()) {
        m_errorString = QDir::toNativeSeparators(m_filePath) + QString::fromLatin1(": ")
                      + m_loader->errorString();
        return false;
    }
    auto *pluginObject = m_loader ? qobject_cast<IPlugin *>(m_loader->instance())
                                : qobject_cast<IPlugin *>(m_staticPlugin->instance());
    if (!pluginObject) {
        m_errorString =
            ::ExtensionSystem::Tr::tr("Plugin is not valid (does not derive from IPlugin)");
        if (m_loader)
            m_loader->unload();
        return false;
    }
    m_state = PluginState::Loaded;
    m_plugin = pluginObject;
    return true;
}

bool PluginSpecification::readMetaData(const QJsonObject &pluginMetaData)
try
{

    QString qJson = QString(QJsonDocument(pluginMetaData).toJson());
    nlohmann::json j = nlohmann::json::parse(qJson.toStdString());
    auto iid = QString::fromStdString(j[Constants::kIID].get<std::string>());
    if (iid != PluginManager::instance().pluginIID()) {
        m_errorString = ::ExtensionSystem::Tr::tr("Plugin does not have the correct IID");
        return false;
    }
    auto json = j[Constants::kPluginMetadata];
    std::print("Plugin1::toStdString\n");
    std::print("{0}1231231\n", json.dump(4));
    std::cout .flush();
    m_name = QString::fromStdString(json[Constants::kPluginName].get<std::string>());
    m_version = QString::fromStdString(json[Constants::kPluginVersion].get<std::string>());
    m_compatVersion = QString::fromStdString(json[Constants::kPluginCompatversion].get<std::string>());
    m_description = QString::fromStdString(json[Constants::kDescription].get<std::string>());
    m_license = QString::fromStdString(json[Constants::kPluginName].get<std::string>());
    m_required = json[Constants::kPluginRequired].get<bool>();
    m_experimental = json[Constants::kPluginExperimental].get<bool>();
    // Dependencies
    auto dependencies = json[Constants::kDependencies];
    if (!dependencies.is_null()) {
        if (!dependencies.is_array()) {
            m_errorString = ::ExtensionSystem::Tr::tr("Dependencies must be an array");
            return false;
        }
        for (const auto &dep : dependencies) {
            if (!dep.is_object()) {
                m_errorString = ::ExtensionSystem::Tr::tr("Dependency must be an object");
                return false;
            }
            auto name = QString::fromStdString(dep[Constants::kDependencyName].get<std::string>());
            auto version = QString::fromStdString(dep[Constants::kDependencyVersion].get<std::string>());
            auto type = QString::fromStdString(dep[Constants::kDependencyType].get<std::string>());
            PluginDependency::Type depType = PluginDependency::Type::Required;;
            if (type == Constants::kDependencyTypeSoft)
                depType = PluginDependency::Type::Optional;
            else if (type == Constants::kDependencyTypeTest)
                depType = PluginDependency::Type::Test;
            PluginDependency dependency;
            dependency.name = name;
            dependency.version = version;
            dependency.type = depType;
            m_dependencies.append(dependency);
        }
    }
    // Arguments
    auto arguments = json[Constants::kArguments];
    if (!arguments.is_null()) {
        if (!arguments.is_array()) {
            m_errorString = ::ExtensionSystem::Tr::tr("Arguments must be an array");
            return false;
        }
        for (const auto &arg : arguments) {
            if (!arg.is_object()) {
                m_errorString = ::ExtensionSystem::Tr::tr("Argument must be an object");
                return false;
            }
            auto name = QString::fromStdString(arg[Constants::kArgumentName].get<std::string>());
            auto parameter = QString::fromStdString(arg[Constants::kArgumentParameter].get<std::string>());
            auto description = QString::fromStdString(arg[Constants::kArgumentDescription].get<std::string>());
            PluginArgumentDescription argument;
            argument.name = name;
            argument.parameter = parameter;
            argument.description = description;
            m_argumentDescriptions.append(argument);
        }
    }
    return true;
}
catch (const nlohmann::json::exception &e)
{
    m_errorString = ::ExtensionSystem::Tr::tr("Error parsing plugin metadata: %1").arg(e.what());
    return false;
}

bool PluginSpecification::reportError(const QString &errorString)
{
    m_errorString = errorString;
    return true;
}

bool PluginSpecification::resolveDependencies(const QVector<PluginSpecification *> &specs)
{
    if (m_errorString.has_value())
        return false;
    if (m_state == PluginState::Resolved)
        m_state = PluginState::Read; // Go back, so we just re-resolve the dependencies.
    if (m_state != PluginState::Read) {
        m_errorString = ::ExtensionSystem::Tr::tr("Resolving dependencies failed because state != Read");
        return false;
    }
    QHash<PluginDependency, PluginSpecification *> resolvedDependencies;
    for (const PluginDependency &dependency : std::as_const(m_dependencies)) {
        // PluginSpecification * const found = Utils::findOrDefault(specs, [&dependency](PluginSpecification *spec) {
        //     return spec->provides(dependency.name, dependency.version);
        // });
        // use std::ranges
        auto it = std::ranges::find_if(specs, [&dependency](PluginSpecification *spec) {
            return spec->provides(dependency.name, dependency.version);
        }) ;
        PluginSpecification * const found = it != specs.end() ? *it : nullptr;
        if (!found) {
            if (dependency.type == PluginDependency::Type::Required) {
                if (m_errorString.has_value())
                    m_errorString.value().append(QLatin1Char('\n'));
                m_errorString.value().append(::ExtensionSystem::Tr::tr("Could not resolve dependency '%1(%2)'")
                                       .arg(dependency.name, dependency.version));
            }
            continue;
        }
        resolvedDependencies.insert(dependency, found);
    }
    if (m_errorString)
        return false;

    m_dependencySpecifications = resolvedDependencies;

    m_state = PluginState::Resolved;

    return true;
}

std::optional<QString> PluginSpecification::errorString() const
{
    return m_errorString;
}

bool PluginSpecification::hasError() const
{
    return m_errorString.has_value();
}


void PluginSpecification::kill()
{
    if (!m_plugin)
        return;
    delete m_plugin;
    m_plugin = nullptr;
    m_state = PluginState::Deleted;
}

bool PluginSpecification::initializePlugin()
{
    if (m_errorString.has_value())
        return false;
    if (m_state != PluginState::Loaded) {
        if (m_state == PluginState::Initialized)
            return true;
        m_errorString = ::ExtensionSystem::Tr::tr(
            "Initializing the plugin failed because state != Loaded");
        return false;
    }
    if (!m_plugin) {
        m_errorString = ::ExtensionSystem::Tr::tr(
            "Internal error: have no plugin instance to initialize");
        return false;
    }
    QString err;
    if (!m_plugin->initialize(m_arguments, err)) {
        m_errorString = ::ExtensionSystem::Tr::tr("Plugin initialization failed: %1").arg(err);
        return false;
    }
    m_state = PluginState::Initialized;
    return true;
}

PluginShutdownFlag PluginSpecification::stop()
{
    if (!m_plugin)
        return PluginShutdownFlag::SynchronousShutdown;
    m_state = PluginState::Stopped;
    return m_plugin->aboutToShutdown();
}

bool PluginSpecification::provides(const QString &pluginName, const QString &version) const
{
    if (QString::compare(pluginName, m_name, Qt::CaseInsensitive) != 0)
        return false;
    return (versionCompare(m_version, version) >= 0) && (versionCompare(m_compatVersion, version) <= 0);

}

bool PluginSpecification::delayedInitialize()
{
    if (m_errorString.has_value())
        return false;
    if (m_state != PluginState::Running)
        return false;
    if (!m_plugin) {
        m_errorString = ::ExtensionSystem::Tr::tr(
            "Internal error: have no plugin instance to perform delayedInitialize");
        return false;
    }
    const bool res = m_plugin->delayedInitialize();
    return res;
}

bool PluginDependency::operator==(const PluginDependency &other) const
{
    return name == other.name && version == other.version && type == other.type;
}

size_t qHash(const PluginDependency &value)
{
    return qHash(value.name);
}

PluginSpecification *PluginSpecification::readPlugin(const QString &filePath)
{
    auto spec = new PluginSpecification;
    if (!spec->read(filePath)) { // not a Qt Creator plugin
        delete spec;
        return nullptr;
    }
    return spec;
}

PluginSpecification *PluginSpecification::readPlugin(const QStaticPlugin &plugin)
{
    auto spec = new PluginSpecification;
    if (!spec->read(plugin)) { // not a Qt Creator plugin
        delete spec;
        return nullptr;
    }
    return spec;
}

int PluginSpecification::versionCompare(const QString &version1, const QString &version2)
{
    static const QRegularExpression reg("^([0-9]+)(?:[.]([0-9]+))?(?:[.]([0-9]+))?(?:_([0-9]+))?$");
    const QRegularExpressionMatch match1 = reg.match(version1);
    const QRegularExpressionMatch match2 = reg.match(version2);
    if (!match1.hasMatch())
        return 0;
    if (!match2.hasMatch())
        return 0;
    for (int i = 0; i < 4; ++i) {
        const int number1 = match1.captured(i + 1).toInt();
        const int number2 = match2.captured(i + 1).toInt();
        if (number1 < number2)
            return -1;
        if (number1 > number2)
            return 1;
    }
    return 0;
}
} // namespace ExtensionSystem
