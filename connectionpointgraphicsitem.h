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

#ifndef __CONNECTIONPOINTGRAPHICSITEM__
#define __CONNECTIONPOINTGRAPHICSITEM__

#include <QGraphicsItem>

class PersonGraphicsItemConnection;

class ConnectionPointGraphicsItem : public QGraphicsItem {
public:
    ConnectionPointGraphicsItem(QGraphicsItem* parent = 0);
    ~ConnectionPointGraphicsItem();

    QRectF boundingRect() const;

    void removeConnection();
    void createConnectionTo(ConnectionPointGraphicsItem* p2);

    QPointF scenePos() const;

private:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    PersonGraphicsItemConnection* _connection;
};

#endif
