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

#include "connectionpointgraphicsitem.h"
#include "persongraphicsitemconnection.h"

#include <QGraphicsScene>
#include <QPainter>

ConnectionPointGraphicsItem::ConnectionPointGraphicsItem(QGraphicsItem* parent)
    : QGraphicsItem(parent)
    , _connection(0)
{}

ConnectionPointGraphicsItem::~ConnectionPointGraphicsItem() {
    delete _connection;
}

QRectF ConnectionPointGraphicsItem::boundingRect() const {
    return QRectF(-5.0, -5.0, 5.0, 5.0);
}

void ConnectionPointGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    const QRectF& rect = boundingRect();
    painter->setBrush(Qt::black);
    painter->setPen(Qt::black);
    painter->drawRect(rect);
}    

void ConnectionPointGraphicsItem::removeConnection() {
    scene()->removeItem(_connection);
    delete _connection;
    _connection = 0;
}

void ConnectionPointGraphicsItem::createConnectionTo(ConnectionPointGraphicsItem* p2) {
    _connection = new PersonGraphicsItemConnection(this, p2);
    scene()->addItem(_connection);
}

QPointF ConnectionPointGraphicsItem::scenePos() const {
    return mapToScene(boundingRect().center());
}
