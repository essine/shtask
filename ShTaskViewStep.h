/* === This file is part of Calamares - <https://github.com/calamares> ===
 *
 *   Copyright 2018-2019, Kirill Esin <essine_kv@yahoo.com>
 *
 *   Calamares is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Calamares is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Calamares. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SHTASKPAGEPLUGIN_H
#define SHTASKPAGEPLUGIN_H

#include <QObject>

#include <utils/PluginFactory.h>
#include <viewpages/ViewStep.h>

#include <PluginDllMacro.h>

#include <QVariant>

class ShTaskPage;

class PLUGINDLLEXPORT ShTaskViewStep : public Calamares::ViewStep
{
    Q_OBJECT

public:
    explicit ShTaskViewStep( QObject* parent = nullptr );
    virtual ~ShTaskViewStep() override;

    QString prettyName() const override;

    QWidget* widget() override;

    void next() override;
    void back() override;

    bool isNextEnabled() const override;
    bool isBackEnabled() const override;

    bool isAtBeginning() const override;
    bool isAtEnd() const override;

    QList< Calamares::job_ptr > jobs() const override;

    void onActivate() override;
    void onLeave() override;

    void setConfigurationMap( const QVariantMap& configurationMap ) override;

private:
    ShTaskPage* m_widget;
    // variables from file shtask.conf
    QString m_moduleLabel;
    QString m_moduleInfoLine;
    QString m_taskChoiceLabel;
    QString m_taskEditLabel;
    bool m_taskOutputBox;
    QStringList m_taskLabels;
    QStringList m_taskCommands;
    QStringList m_taskScript;
    bool m_taskOutputConsole;
    bool m_taskOutputWindow;
    bool m_outputPause;
    int m_outputDelaySec;
    QStringList m_taskHelp;
    QStringList m_taskAbout;

    QList< Calamares::job_ptr > m_jobs;
};

CALAMARES_PLUGIN_FACTORY_DECLARATION( ShTaskViewStepFactory )

#endif // ANYTASKPAGEPLUGIN_H
