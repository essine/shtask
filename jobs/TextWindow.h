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

#ifndef TEXTWINDOW_H
#define TEXTWINDOW_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>

namespace Ui
{
class TextWindow;
}

class TextWindow : public QDialog
{
    Q_OBJECT
public:
    explicit TextWindow( QWidget* parent = 0 );
    virtual ~TextWindow();

    void displayTextWindow( const QString& title,
                            const QByteArray& text );

    void displayOutputWindow( const QString& title,
                              const QByteArray& text,
                              const bool console,
                              const bool window,
                              const bool pause,
                              const int  delaySec );

//signals:
//public slots:

private:
    Ui::TextWindow* ui;

    void setText( const QByteArray& text,
                  const int style );
    void windowDelay( const int delaySec );
    void displayModelessDialog( const int delaySec );
    void displayModalDialog( void );
};

#endif // TEXTWINDOW_H


