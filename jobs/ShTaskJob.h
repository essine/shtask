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

#ifndef SHTASKJOB_H
#define SHTASKJOB_H

#include <Job.h>

class ShTaskJob : public Calamares::Job
{
    Q_OBJECT
public:
    ShTaskJob( const QString& taskLabel,
               const QString& taskCommand,
               const bool taskOutputConsole,
               const bool taskOutputWindow,
               const bool outputPause,
               const int  outputDelaySec );

    QString prettyName() const override;
    QString prettyStatusMessage() const override;

    Calamares::JobResult exec() override;

private:
    void cleanup( void );

    QString m_taskLabel;
    QString m_taskCommand;
    bool m_taskOutputConsole;
    bool m_taskOutputWindow;
    bool m_outputPause;
    int  m_outputDelaySec;
};

#endif /* SHTASKJOB_H */
