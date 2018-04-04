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

#ifndef TASKCHECK_H
#define  TASKCHECK_H

#include <QObject>
// This class helps to check task command (e.g. if script exists).
class TaskCheck  : public QObject
{
    Q_OBJECT
    QString scriptFullName;
public:
    TaskCheck( const QString taskCommand, //retrieves script full name from 'taskCommand' using 'scriptExtentsion'
               const QString scriptExtension );

    QString getScriptFullName(); //returns script full name including path if present
    bool ifScript(); 		   //returns true if task contains a script name
    bool ifGeneratedScript();  //returns true if script was generated from module conf file
    bool ifScriptExists();     //returns true if script file is found
    void warningMessage( const QString text ); //QMessageBox with text
    bool ifScriptPathExists();  //returns true if script path is provided
    QString scriptPathCandidate(); //returns script path candidate
    QString scriptName(); //returns script name without path
    QStringList getScriptPathCandidateList(); //returns the list of script path candidates
    QString scriptPathCandidate( QStringList pathList ); // returns script path candidate
    QString taskScriptPathReplace( const QString taskCommand,
                                   const QString newScriptPath ); //returns task command with a new script path
};
#endif // TASKMONITOR_H

