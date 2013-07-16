/***********************************************************************
 *                                                                     *
 * Stammbaum                                                           *
 *                                                                     *
 * Copyright (c) 2012, Alexander Bock                                  *
 *                                                                     *
 * This work is licensed under the                                     *
 * GNU General Public License version 3.                               *
 *                                                                     *
 * Stammbaum is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        *
 * GNU General Public License for more details.                        *
 *                                                                     *
 * You should have received a copy of the GNU General Public License   *
 * in the file "LICENSE.txt" along with this program.                  *
 * If not, see <http://www.gnu.org/licenses/>.                         *
 *                                                                     *
 **********************************************************************/

#ifndef __PERSONGRAPHICSITEMCONNECTION__
#define __PERSONGRAPHICSITEMCONNECTION__

#include <QGraphicsItem>

class ConnectionPointGraphicsItem;

class PersonGraphicsItemConnection : public QGraphicsItem {
public:
    PersonGraphicsItemConnection(ConnectionPointGraphicsItem* p1, ConnectionPointGraphicsItem* p2);
    QRectF boundingRect() const;

private:
    QPainterPath shape() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    ConnectionPointGraphicsItem* _p1;
    ConnectionPointGraphicsItem* _p2;
};

#endif
