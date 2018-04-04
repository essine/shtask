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

#include "jobs/TextWindow.h"
#include "ui_TextWindow.h"
#include "utils/Logger.h" // cDebug

#include <QThread> // Delay

TextWindow::TextWindow( QWidget* parent ) :
    QDialog( parent ),
    ui( new Ui::TextWindow )
{
    ui->setupUi( this );
    connect( ui->close_Btn, SIGNAL( pressed() ), this, SLOT( accept() ) );
}

TextWindow::~TextWindow()
{
    delete ui;
}

// delay text window for 'delaySec' seconds
void TextWindow::windowDelay( const int delaySec )
{
    if ( delaySec > 0 )
    {
        QThread* thread = new QThread( this ); //  assigning a parent to the QThread
        thread->start();
        thread->sleep( delaySec );
        thread->quit();
    }
}

void TextWindow::setText( const QByteArray& text, const int style )
{
    // set text
    ui->text_pTE->clear(); // unless you know the editor is empty
    ui->text_pTE->appendPlainText ( text ); // add text

    // set text style
    if ( style == 0 ) // displayTextWindow
    {
        ui->text_pTE->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded ); // allow scroll
        ui->text_pTE->moveCursor( QTextCursor::Start ); // scroll up
    }
    if ( style == 1 ) // displayOutput
    {
        ui->text_pTE->moveCursor( QTextCursor::End ); // scroll down
        ui->text_pTE->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff ); // prevent scrolling
        ui->text_pTE->setEnabled( false ); // prevent text cursor
    }
    ui->text_pTE->setReadOnly( true ); // prevent text edit
    ui->text_pTE->setTextInteractionFlags( Qt::TextEditable ); // prevent text selection
    setFixedSize( size() ); // dialog window size is fixed
}

void TextWindow::displayModalDialog( void )
{
    setModal( true );
    exec();
}

void TextWindow::displayModelessDialog( const int delaySec )
{
    TextWindow::setModal( false );
    ui->close_Btn->setEnabled( false );
    ui->close_Btn->setText( "Will close in " + QString::number( delaySec ) + " sec" );
    TextWindow::show();
    windowDelay( delaySec );
    TextWindow::close();
}

void TextWindow::displayOutputWindow( const QString& title,
                                      const QByteArray& text,
                                      const bool console,
                                      const bool window,
                                      const bool pause,
                                      const int  delaySec )
{
    TextWindow::setWindowTitle( title );
    TextWindow::setText( text,1 );

    if ( console ) cDebug() << text; // send text to console

    if ( window ) // send text to graphcal window
    {
        if ( pause ) displayModalDialog(); 				// pause
        else        displayModelessDialog( delaySec );  // delay
    }
}

void TextWindow::displayTextWindow( const QString& title,
                                    const QByteArray& text )
{
    TextWindow::setWindowTitle( title );
    TextWindow::setText( text,0 );
    TextWindow::displayModalDialog();
}
