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

#include "ShTaskPage.h"
#include "ui_ShTaskPage.h"
#include "jobs/ShTaskJob.h"
#include "TaskCheck.h"

#include "JobQueue.h"

#include "GlobalStorage.h"
#include "utils/Logger.h"
#include "utils/CalamaresUtilsGui.h"
#include "utils/Retranslator.h"

#include <QBoxLayout>
#include <QComboBox>
#include <QCheckBox>

#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QDir>

#include "jobs/TextWindow.h"

ShTaskPage::ShTaskPage( QWidget* parent )
    : QWidget( parent )
    , ui( new Ui::ShTaskPage )
{
    ui->setupUi( this );
}


ShTaskPage::~ShTaskPage()
{
    // destructor
    delete ui;
}

void
ShTaskPage::onActivate()
{
    ui->taskChoice_CBx->setFocus();
}

// enable/disable 'Pause' and 'Delay' widgets according to Window checkbox
// slot function, connect 'outputWindow' check box and time control widgets
void
ShTaskPage::enableDisableTimeControl( int state )
{
    if ( state == 0 ) // check box OutputWindow is unchecked
    {
        ui->outputPause_ChBx->setEnabled( false );
        enableDisablePause( 2 );	//false
    }
    else
    {
        ui->outputPause_ChBx->setEnabled( true );
        if ( ui->outputPause_ChBx->checkState() ) enableDisablePause( 2 );
        else 								   enableDisablePause( 0 );
    }
}

// enable/disable 'Delay' widgets according to 'Pause' checkbox
// slot function, connect 'outputPause check box and 'outputDelay' widget
void
ShTaskPage::enableDisablePause( int state )
{
    if ( state == 0 ) // checkbox 'outputPause' is unchecked
    {
        ui->outputDelay_SBx->setEnabled( true );
        ui->outputDelay1_Lbl->setEnabled( true );
        ui->outputDelay2_Lbl->setEnabled( true );
    }
    else
    {
        ui->outputDelay_SBx->setEnabled( false );
        ui->outputDelay1_Lbl->setEnabled( false );
        ui->outputDelay2_Lbl->setEnabled( false );
    }
}

// update 'taskCommand'; connected with 'taskEdit_LEd' QLineEdit
void
ShTaskPage::correctTaskCommand()
{
    int taskIndex = ui->taskChoice_CBx->currentIndex();	 // get current 'taskIndex'
    QString taskCommand = ui->taskEdit_LEd->text();      // get changed 'taskCommand'
    m_taskCommands[taskIndex] = taskCommand;             // set 'taskCommand' to corrected variant
}

// open file dialog; find script; insert script file name in task command
// connected to 'Find script' button
void
ShTaskPage::findScript( void )
{
    QString fileName = QFileDialog::getOpenFileName( this,
                       tr( "Find Script" ), QDir::currentPath(), tr( "Scrpt Files (*.sh)" ) );
    if ( fileName.count( ' ' ) != fileName.length() ) // if fileName is not empty
    {
        ui->taskEdit_LEd->setText( "bash " + fileName );
        ShTaskPage::correctTaskCommand();
    }
}

// check 'taskCommand' string content
// connected with 'taskChoice_CBx' QComboBox; checks 'taskCommand';
// for script existence 3 situations are considered:
// 	- script generated from module conf file
// 	- script without a path ( looking for a path candidate )
// 	- script with the path
void
ShTaskPage::checkTaskCommand()
{
    // get current 'taskCommand'
    int taskIndex = ui->taskChoice_CBx->currentIndex();
    QString taskCommand = m_taskCommands[taskIndex];

    // place current 'taskCommand' to edit line 'taskEdit_LEd'
    ui->taskEdit_LEd->setText( taskCommand );

    // check if 'taskCommand' contains a script
    TaskCheck* cs = new TaskCheck( taskCommand,".sh" ); // heap
    if ( !cs->ifScript() ) //no script
    {
        ui->findScript_Btn->setEnabled( false );
        return;
    }
    else
        ui->findScript_Btn->setEnabled( true );

    // if 'taskCommand' contains generated script
    QString wm = "";
    if ( cs->ifGeneratedScript() )
    {
        if ( !cs->ifScriptExists() ) //if scrpit was not found
        {
            wm = "Generated script was not found.\n";
            wm.append( "Edit script path/name or find it using 'Find script' button." );
            cs->warningMessage( wm );
        }
        return;
    }

    // check script existance, update path and 'taskCommand' string if necessary
    QString newTaskCommand = taskCommand;
    QStringList pathList = cs->getScriptPathCandidateList();

    if ( !cs->ifScriptPathExists() ) // if script path is not present try path candidates
    {
        QString pathCandidate = cs->scriptPathCandidate( pathList );
        if ( pathCandidate.contains( "Script path is not found" ) )
        {
            wm = "Script is not found in : \n" + pathList.join( "\n" ) + "\n";
            wm.append( "Edit script path/name or find it using 'Find script' button." );
            cs->warningMessage( wm );
        }
        else   	//place path corrected 'taskCommand' to edit line `taskEdit_LEd`
        {
            newTaskCommand = cs->taskScriptPathReplace( taskCommand,pathCandidate );
            ui->taskEdit_LEd->setText( newTaskCommand );
        }
    }
    else   // if script path is present
    {
        if ( !cs->ifScriptExists() )
        {
            wm = "Script is not found.\n";
            wm.append( "Edit script path/name or find it using 'Find script' button.\n" );
            wm.append( "Possible locations are : \n" + pathList.join( "\n" ) );
            cs->warningMessage( wm );
        }
    }
}

// converts a QStringList to QByteArray
const QByteArray
ShTaskPage::convertStringListToByteArray( const QStringList& slist )
{
    QStringList sl = slist;
    QByteArray ba;
    for ( QStringList::Iterator it = sl.begin(); it != sl.end(); ++it )
        ba.append( *it+"\n" );
    return ba;
}

// generates script file from module conf file
// returns script file full name or error message
const QString
ShTaskPage::generateScript( const QStringList& taskScript )
{
    QFile file( "generated.sh" );
    if ( file.open( QFile::WriteOnly | QFile::Text ) )
    {
        QTextStream s( &file );
        s << "#This calamares script is automatically generated from module conf file\n";
        s << convertStringListToByteArray( taskScript );
        file.close();
    }
    else
        return "Script was not generated";
    QFileInfo fi( "generated.sh" );
    return fi.absoluteFilePath();
}

// display help
void
ShTaskPage::displayHelp( void )
{
    TextWindow* hw = new TextWindow( this );
    hw->displayTextWindow( "Help", m_taskHelp );
}

// display about
void
ShTaskPage::displayAbout( void )
{
    TextWindow* aw = new TextWindow( this );
    aw->displayTextWindow( "About", m_taskAbout );
}

// init ui according module conf. file
// set signal - slot connections
void
ShTaskPage::initUI( const QString& moduleLabel,
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
                    const QStringList& taskAbout )
{

    ui->infoLine_Lbl->setText( moduleInfoLine );
    ui->taskChoice_Lbl->setText( taskChoiceLabel );
    ui->taskChoice_CBx->addItems( taskLabels );

    ui->choice_GBx->setTitle( moduleLabel );
    ui->output_GBx->setTitle( moduleLabel + " Output" );

    ui->taskEdit_Lbl->setText( taskEditLabel );
    if ( taskOutputBox ) ui->output_GBx->show();
    else 			   ui->output_GBx->hide();

    ui->taskEdit_LEd->setText( taskCommands[0] );

    ui->outputConsole_ChBx->setChecked( taskOutputConsole );

    ui->outputPause_ChBx->setChecked( outputPause );
    ui->outputWindow_ChBx->setChecked( taskOutputWindow );
    if ( taskOutputWindow ) ShTaskPage::enableDisableTimeControl( 2 );
    else  				  ShTaskPage::enableDisableTimeControl( 0 );

    ui->outputDelay_SBx->setRange( 0,10 );
    ui->outputDelay_SBx->setValue( outputDelaySec );

    m_taskHelp = convertStringListToByteArray( taskHelp );
    m_taskAbout = convertStringListToByteArray( taskAbout );
    m_taskCommands = taskCommands;   // initial, unchanged 'taskCommands'
    ShTaskPage::checkTaskCommand(); // initial check of unchanged 'taskCommand'

    // check selected 'taskCommand'
    connect( ui->taskChoice_CBx, static_cast< void ( QComboBox::* )( int ) >(
                 &QComboBox::currentIndexChanged ),
             this, [ & ]( int newTaskIndex )
    {
        ShTaskPage::checkTaskCommand();
    }
           );

    // CONNECTIONS
    // correct selected 'taskCommand'
    connect( ui->taskEdit_LEd,   static_cast< void ( QLineEdit::* )( const QString& ) >
             ( &QLineEdit::textEdited ),
             this, [ & ]( const QString& newTaskEdit )
    {
        ShTaskPage::correctTaskCommand();
    }
           );

    // enable/disable time control widgets 'Pause' and 'Delay'
    connect( ui->outputWindow_ChBx, SIGNAL( stateChanged( int ) ), this, SLOT( enableDisableTimeControl( int ) ) );

    // enable/disable time control widget 'Delay'
    connect( ui->outputPause_ChBx, SIGNAL( stateChanged( int ) ), this, SLOT( enableDisablePause( int ) ) );

    // display 'taskHelp'
    connect( ui->taskHelp_Btn, SIGNAL( pressed() ), this, SLOT( displayHelp() ) );

    // display 'taskAbout'
    connect( ui->taskAbout_Btn, SIGNAL( pressed() ), this, SLOT( displayAbout() ) );

    // find script file, open file open dialog
    connect( ui->findScript_Btn, SIGNAL( pressed() ), this, SLOT( findScript() ) );
}

QList< Calamares::job_ptr >
ShTaskPage::createJobs()
{
    QList< Calamares::job_ptr > list;

    QString tl = ui->taskChoice_CBx->currentText();  // tl - selected 'taskLabel'
    QString tc = ui->taskEdit_LEd->text();           // tc - corrected 'taskCommand'

    bool toc  = ui->outputConsole_ChBx->checkState();// toc - selected 'taskOutputConsole'
    bool tow  = ui->outputWindow_ChBx->checkState(); // tow - selected 'taskOutputWindow'
    bool op   = ui->outputPause_ChBx->checkState();  // op  - selected 'outputPause'

    int od    = ui->outputDelay_SBx->value();        // od  - selected OutputDelay

    Calamares::Job* j = new ShTaskJob(  tl, tc, toc, tow, op, od );
    list.append( Calamares::job_ptr( j ) );

    return list;
}
