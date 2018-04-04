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

#ifndef SHTASKPAGE_H
#define SHTASKPAGE_H

#include "Typedefs.h"
#include <QWidget>

namespace Ui
{
class ShTaskPage;
}

class ShTaskPage : public QWidget
{
    Q_OBJECT

public:
    explicit ShTaskPage( QWidget* parent = nullptr );
    virtual ~ShTaskPage();

    void initUI( const QString& moduleLabel,
                 const QString& moduleInfoLine,
                 const QString& taskChoiceLabel,
                 const QString& taskEditLabel,
                 const bool taskOutputBox,
                 const QStringList& taskLabels,
                 const QStringList& taskCommands,
                 const bool taskOutputConsole,
                 const bool taskOutputWindow,
                 const bool outputPause,
                 const int outputDelaySec,
                 const QStringList& taskHelp,
                 const QStringList& taskAbout );

    const QString generateScript( const QStringList& taskScript );
    const QByteArray convertStringListToByteArray( const QStringList& slist );
    void checkTaskCommand();
    void correctTaskCommand();

    QString prettyStatus() const;
    QList< Calamares::job_ptr > createJobs();

    void onActivate();

public slots:
    void enableDisableTimeControl( int state );
    void enableDisablePause( int state );
    void displayHelp( void );
    void displayAbout( void );
    void findScript( void );

private:
    Ui::ShTaskPage* ui;
    QStringList m_taskCommands;
    QByteArray m_taskHelp;
    QByteArray m_taskAbout;
};

#endif // SHTASKPAGE_H
