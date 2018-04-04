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

#include "ShTaskJob.h"
#include "jobs/TextWindow.h" // display task output 

#include "JobQueue.h"
#include "GlobalStorage.h"
#include "utils/Logger.h"
#include "utils/CalamaresUtilsSystem.h"

#include <QProcess>
#include <QFile>

ShTaskJob::ShTaskJob( const QString& taskLabel,
                      const QString& taskCommand,
                      const bool taskOutputConsole,
                      const bool taskOutputWindow,
                      const bool outputPause,
                      const int  outputDelaySec )
    : Calamares::Job()
    , m_taskLabel( taskLabel )
    , m_taskCommand( taskCommand )
    , m_taskOutputConsole( taskOutputConsole )
    , m_taskOutputWindow( taskOutputWindow )
    , m_outputPause( outputPause )
    , m_outputDelaySec( outputDelaySec )
{
}

QString
ShTaskJob::prettyName() const
{
    // will be shown in console output, "calamares -d"
    return tr( "AnyTask module. Perform task: %1." ).arg( m_taskLabel );
}

QString
ShTaskJob::prettyStatusMessage() const
{
    return tr( "Performing task: %1." ).arg( m_taskCommand );
}

Calamares::JobResult
ShTaskJob::exec()
{
    QProcess taskProcess;

    taskProcess.start( m_taskCommand );
    if ( !taskProcess.waitForStarted() )
        return Calamares::JobResult::error( tr( "Failed to start job %1" ).arg( m_taskLabel ) );
    if ( !taskProcess.waitForFinished() )
        return Calamares::JobResult::error( tr( "Failed to finish job %1" ).arg( m_taskLabel ) );

    QByteArray taskOutput = taskProcess.readAll();

    // display output (QByteArray taskOutput)
    QString taskTitle =  m_taskLabel + " : " + m_taskCommand;

    TextWindow* ro = new TextWindow( 0 ); // pass 0 as parent
    ro->displayOutputWindow( taskTitle, taskOutput,
                             m_taskOutputConsole, m_taskOutputWindow,
                             m_outputPause, m_outputDelaySec );

    ShTaskJob::cleanup();
    return Calamares::JobResult::ok();
}

// clean up temporary files
void
ShTaskJob::cleanup( void )
{
    if ( QFile::exists( "generated.sh" ) ) QFile::remove( "generated.sh" );
}
