//////////////////////////////////////////////////////////////////////////////
// Product: PixelLabel utility class for Qt
// Last Updated for Version: 4.5.02
// Date of the Last Update:  Jul 04, 2012
//
//                    Q u a n t u m     L e a P s
//                    ---------------------------
//                    innovating embedded systems
//
// Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Alternatively, this program may be distributed and modified under the
// terms of Quantum Leaps commercial licenses, which expressly supersede
// the GNU General Public License and are specifically designed for
// licensees interested in retaining the proprietary status of their code.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// Contact information:
// Quantum Leaps Web sites: http://www.quantum-leaps.com
//                          http://www.state-machine.com
// e-mail:                  info@quantum-leaps.com
//////////////////////////////////////////////////////////////////////////////
#ifndef pixel_label_h
#define pixel_label_h

#include <QLabel>
#include <QPixmap>

// PixelLabel class is for drawing graphic displays with up to 24-bit color
class PixelLabel : public QLabel {
    Q_OBJECT

public:
    PixelLabel(QWidget *parent = 0)
      : QLabel(parent), m_bits(0)
    {}
    virtual ~PixelLabel();
    void init(int width,  int xScale,
              int height, int yScale,
              quint8 const bgColor[3]);
    void clear(void);
    void setPixel(int x, int y, quint8 const color[3]);
    void clearPixel(int x, int y) {
        setPixel(x, y, m_bgColor);
    }
    void redraw(void);

private:
    int      m_width;
    int      m_xScale;
    int      m_height;
    int      m_yScale;
    QPixmap  m_pixmap;
    quint32  m_size;
    quint8  *m_bits;
    quint8   m_bgColor[3];
};

#endif                                                        // pixel_label_h
