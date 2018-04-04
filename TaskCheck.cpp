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

#include "TaskCheck.h"
#include "utils/Logger.h" // cDebug
#include <QList>
#include <QString>
#include <QRegularExpression>
#include <QFileInfo>
#include <QMessageBox>
#include "utils/CalamaresUtilsGui.h"
#include <QDir>

//retrieves script full name from 'taskCommand' using 'scriptExtentsion'
TaskCheck::TaskCheck( const QString taskCommand,
                      const QString scriptExtension )
{
    scriptFullName = "Scrpt full name not found";
    QList<QString> sl;
    sl = taskCommand.split( QRegularExpression( "\\s+" ), QString::KeepEmptyParts );
    for ( int i = 0; i < sl.size(); ++i )
    {
        if ( sl.at( i ).contains( scriptExtension ) )
            scriptFullName = sl.at( i );
    }
}

// returns script full name including path if present
QString
TaskCheck::getScriptFullName()
{
    return scriptFullName;
}

// returns true if task contains a script name
bool
TaskCheck::ifScript()
{
    if ( scriptFullName.contains( ".sh" ) ) return true;
    else return false;
}

// returns true if script was generated from module conf file
bool
TaskCheck::ifGeneratedScript()
{
    if ( scriptFullName.contains( "generated.sh" ) ) return true;
    else return false;
}
//returns true if script file is found
bool
TaskCheck::ifScriptExists()
{
    return QFileInfo::exists( scriptFullName );
}

// generates warning QMessageBox
void
TaskCheck::warningMessage( const QString text )
{
    QMessageBox msgBox;
    msgBox.setWindowTitle( "Warning" );
    msgBox.setText( "Check task command: " + text );
    msgBox.exec();
}

// returns true if script path is provided
bool
TaskCheck::ifScriptPathExists()
{
    QFileInfo fi( scriptFullName );
    QString scriptPathName = fi.filePath();
    QString scriptName = fi.fileName();
    int c = scriptPathName.compare( scriptName );
    if ( c != 0 ) return true;
    else       return false;
}

// returns the list of script path candidates
QStringList
TaskCheck::getScriptPathCandidateList()
{
    QStringList list;

    // scripts placed in .../calamares/src/branding/default/shtask.sh
    // before calamares build
    list << CalamaresUtils::appDataDir().absolutePath() + "/branding/default";

    // current path
    list << QDir::currentPath();

    return list;
}

// returns candidate for script path
// try to find script using path candidates from 'pathList'
QString
TaskCheck::scriptPathCandidate( QStringList pathList )
{
    QString s;
    for ( int i = 0; i < pathList.size(); ++i )
    {
        s = pathList.at( i ) + "/" + TaskCheck::scriptName();
        if ( QFileInfo::exists( s ) ) return pathList.at( i );
    }
    return "Script path is not found";
}

// returns script name without path
QString
TaskCheck::scriptName()
{
    QFileInfo fi( scriptFullName );
    return fi.fileName();
}

// returns task command with a new script path
QString
TaskCheck::taskScriptPathReplace( const QString taskCommand,
                                  const QString newScriptPath )
{
    QString newTaskCommand = taskCommand;
    newTaskCommand.replace( scriptFullName,"" );
    newTaskCommand.append( newScriptPath + "/" + TaskCheck::scriptName() );
    return newTaskCommand;
}



