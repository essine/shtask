/* === This file is part of Calamares - <https://github.com/calamares> ===
 *
 *   Copyright 2015, Teo Mrnjavac <teo@kde.org>
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

import QtQuick 2.0;
import calamares.slideshow 1.0;

Presentation
{
    id: presentation

    Timer {
        id: advanceTimer
        interval: 5000
        running: false
        repeat: true
        onTriggered: presentation.goToNextSlide()
    }
    
    Slide {

        Image {
            id: background
            source: "squid1.png"
            width: 200; height: 200
            fillMode: Image.PreserveAspectFit
            anchors.centerIn: parent
        }
        Text {
            anchors.horizontalCenter: background.horizontalCenter
            anchors.top: background.bottom
            text: "Here the actual install of the OS is going ...<br/>"
            wrapMode: Text.WordWrap
            width: presentation.width
            horizontalAlignment: Text.Center
        }
    }

    Slide {

		Image { 
		    id: background2
			source: "squid2.png"
		    width: 200; height: 200
            fillMode: Image.PreserveAspectFit
            anchors.centerIn: parent	 
		}
		Text {

            anchors.horizontalCenter: background2.horizontalCenter
            anchors.top: background2.bottom
            text: "Here the actual install of the OS is going ...<br/>"
            wrapMode: Text.WordWrap
            width: presentation.width
            horizontalAlignment: Text.Center
        }
    }

    Slide {
    	Image { 
    	    id: background3
			source: "squid3.png"
		    width: 200; height: 200
            fillMode: Image.PreserveAspectFit
            anchors.centerIn: parent	 
		}
		Text {

            anchors.horizontalCenter: background3.horizontalCenter
            anchors.top: background3.bottom
            text: "Here the actual install of the OS is going ...<br/>"
            wrapMode: Text.WordWrap
            width: presentation.width
            horizontalAlignment: Text.Center
        }
    }

    Component.onCompleted: advanceTimer.running = true
}
