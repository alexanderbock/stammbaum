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

#include "persongraphicsitemconnection.h"
#include "connectionpointgraphicsitem.h"

#include <QPainter>

PersonGraphicsItemConnection::PersonGraphicsItemConnection(ConnectionPointGraphicsItem* p1, ConnectionPointGraphicsItem* p2)
    : _p1(p1)
    , _p2(p2)
{
    Q_ASSERT(_p1 != 0);
    Q_ASSERT(_p2 != 0);
}

QRectF PersonGraphicsItemConnection::boundingRect() const {
    QRectF rect = shape().boundingRect();
    return rect;
}

void PersonGraphicsItemConnection::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QPainterPath path = shape();
    painter->drawPath(path);
}

QPainterPath PersonGraphicsItemConnection::shape() const {
    QPainterPath path;
    path.moveTo(_p1->scenePos());
    path.lineTo(_p2->scenePos());
    return path;
}
