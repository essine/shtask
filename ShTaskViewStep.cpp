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

#include "ShTaskViewStep.h"
#include "ShTaskPage.h"

#include "utils/Logger.h"
#include "JobQueue.h"
#include "GlobalStorage.h"

CALAMARES_PLUGIN_FACTORY_DEFINITION( ShTaskViewStepFactory, registerPlugin<ShTaskViewStep>(); )

ShTaskViewStep::ShTaskViewStep( QObject* parent )
    : Calamares::ViewStep( parent )
    , m_widget( new ShTaskPage() )
{
    emit nextStatusChanged( true );
}


ShTaskViewStep::~ShTaskViewStep()
{
    if ( m_widget && m_widget->parent() == nullptr )
        m_widget->deleteLater();
}


QString
ShTaskViewStep::prettyName() const
{
    return m_moduleLabel ; //Will appear in the main menu list on the left
}


QWidget*
ShTaskViewStep::widget()
{
    return m_widget;
}


void
ShTaskViewStep::next()
{
    emit done();
}


void
ShTaskViewStep::back()
{}


bool
ShTaskViewStep::isNextEnabled() const
{
    return true;
}


bool
ShTaskViewStep::isBackEnabled() const
{
    return true;
}


bool
ShTaskViewStep::isAtBeginning() const
{
    return true;
}


bool
ShTaskViewStep::isAtEnd() const
{
    return true;
}

QList< Calamares::job_ptr >
ShTaskViewStep::jobs() const
{
    return m_jobs;
}

void
ShTaskViewStep::onActivate()
{
    m_widget->onActivate();
}


void
ShTaskViewStep::onLeave()
{
    m_jobs.clear();
    m_jobs.append( m_widget->createJobs() );
}

// retrieves parameters from shtask.conf
void
ShTaskViewStep::setConfigurationMap( const QVariantMap& configurationMap )
{
    // set moduleLabel from shtask.conf or default
    if ( configurationMap.contains( "moduleLabel" ) &&
            configurationMap.value( "moduleLabel" ).type() == QVariant::String )
        m_moduleLabel = configurationMap.value( "moduleLabel" ).toString();
    else
    {
        m_moduleLabel = QString{ "moduleLabel" };
    }

    // set moduleInfoLine from shtask.conf or default
    if ( configurationMap.contains( "moduleInfoLine" ) &&
            configurationMap.value( "moduleInfoLine" ).type() == QVariant::String )
        m_moduleInfoLine = configurationMap.value( "moduleInfoLine" ).toString();
    else
    {
        m_moduleInfoLine = QString{ "ShTask module perform a shell task of your choice" };
    }

    // set taskChoiceLabel from shtask.conf or default
    if ( configurationMap.contains( "taskChoiceLabel" ) &&
            configurationMap.value( "taskChoiceLabel" ).type() == QVariant::String )
        m_taskChoiceLabel = configurationMap.value( "taskChoiceLabel" ).toString();
    else
    {
        m_taskChoiceLabel = QString{ "Select a task to perform" };
    }

    // set taskEditLabel from shtask.conf or default
    if ( configurationMap.contains( "taskEditLabel" ) &&
            configurationMap.value( "taskEditLabel" ).type() == QVariant::String )
        m_taskEditLabel = configurationMap.value( "taskEditLabel" ).toString();
    else
    {
        m_taskEditLabel = QString{ "Selected task command (editable)" };
    }

    // set taskOutputBox from shtask.conf or default
    if ( configurationMap.contains( "taskOutputBox" ) &&
            configurationMap.value( "taskOutputBox" ).type() == QVariant::Bool )
        m_taskOutputBox = configurationMap.value( "taskOutputBox" ).toBool();
    else
        m_taskOutputBox = true;

    // set taskLabels from shtask.conf or default
    if ( configurationMap.contains( "taskLabels" ) &&
            configurationMap.value( "taskLabels" ).type() == QVariant::List )
        m_taskLabels = configurationMap.value( "taskLabels" ).toStringList();
    else
    {
        cWarning() << "No taskLabels  in shtask.conf.";
        m_taskLabels = QStringList{ "Systeminformation", "Effective user and group IDs", "Current directory" };
    }

    // set taskCommands from shtask.conf or default
    if ( configurationMap.contains( "taskCommands" ) &&
            configurationMap.value( "taskCommands" ).type() == QVariant::List )
        m_taskCommands = configurationMap.value( "taskCommands" ).toStringList();
    else
    {
        cWarning() << "No taskCommands  in shtask.conf.";
        m_taskCommands = QStringList{ "uname -a", "id", "pwd" };
    }

    // set taskOutputConsole from shtask.conf or default
    if ( configurationMap.contains( "taskOutputConsole" ) &&
            configurationMap.value( "taskOutputConsole" ).type() == QVariant::Bool )
        m_taskOutputConsole = configurationMap.value( "taskOutputConsole" ).toBool();
    else
        m_taskOutputConsole = true;

    // set taskOutputWindow from shtask.conf or default
    if ( configurationMap.contains( "taskOutputWindow" ) &&
            configurationMap.value( "taskOutputWindow" ).type() == QVariant::Bool )
        m_taskOutputWindow = configurationMap.value( "taskOutputWindow" ).toBool();
    else
        m_taskOutputWindow = true;

    // set outputPause from shtask.conf or default
    if ( configurationMap.contains( "outputPause" ) &&
            configurationMap.value( "outputPause" ).type() == QVariant::Bool )
        m_outputPause = configurationMap.value( "outputPause" ).toBool();
    else
        m_outputPause = true;

    // set outputDelaySec from shtask.conf or default
    if ( configurationMap.contains( "outputDelaySec" ) &&
            configurationMap.value( "outputDelaySec" ).type() == QVariant::Int )
        m_outputDelaySec = configurationMap.value( "outputDelaySec" ).toInt();
    else
        m_outputDelaySec = 0;

    // set taskHelp from shtask.conf or default
    if ( configurationMap.contains( "taskHelp" ) &&
            configurationMap.value( "taskHelp" ).type() == QVariant::List )
        m_taskHelp = configurationMap.value( "taskHelp" ).toStringList();
    else
    {
        m_taskHelp = QStringList
        {
            "Select a shell command or script using 'Select a task to perform' combobox.",
            "The selected command can be corrected in 'Selected task command' line edit.",
            "Check 'Console' checkbox to send command output to Calamares console (calamares -d).",
            "Check 'Window' checkbox to send command output to a new graphical window.",
            "If checkbox 'Pause' is checked, the output window stay visible until close button click.",
            "If checkbox 'Pause' is unchecked, the output window will be shown for selected 'Delay' seconds.",
            "The parameters that control module behavior are defined in 'shtask.conf' configuration file.",
            "For more help please see README.html.",
            "\n",
            "This text can be replaced in 'shtask.conf', the module configuration file."};
    }

    // set taskAbout from shtask.conf or default
    if ( configurationMap.contains( "taskAbout" ) &&
            configurationMap.value( "taskAbout" ).type() == QVariant::List )
        m_taskAbout = configurationMap.value( "taskAbout" ).toStringList();
    else
    {
        m_taskAbout = QStringList
        {
            "'shtask' is a Calamares module.",
            "It allows to select and perform a shell command or script.",
            "\n",
            "This text can be replaced in 'shtask.conf', the module configuration file."};
    }

    // generate taskScript from shtask.conf
    if ( configurationMap.contains( "taskScript" ) &&
            configurationMap.value( "taskScript" ).type() == QVariant::List )
    {
        m_taskScript = configurationMap.value( "taskScript" ).toStringList();
        QString gsc = "bash " + m_widget->generateScript( m_taskScript );
        m_taskLabels << "generated script";
        m_taskCommands << gsc;
    }

    // arrange UI according to shtask.conf or default
    m_widget->initUI( m_moduleLabel,
                      m_moduleInfoLine, m_taskChoiceLabel,
                      m_taskEditLabel, m_taskOutputBox,
                      m_taskLabels, m_taskCommands,
                      m_taskOutputConsole, m_taskOutputWindow,
                      m_outputPause, m_outputDelaySec,
                      m_taskHelp, m_taskAbout );
}
