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

#ifndef __PERSON__
#define __PERSON__

#include <QDate>
#include <QGraphicsObject>
#include <QList>
#include <QObject>
#include <QString>

class ConnectionPointGraphicsItem;

class Person : public QGraphicsObject {
Q_OBJECT
public:
    Person();
    
    QRectF boundingRect() const;

    void setFamilyName(const QString& familyName);
    const QString& familyName() const;
    void setGivenName(const QString& givenName);
    const QString& givenName() const;
    void setMaidenName(const QString& maidenName);
    const QString& maidenName() const;
    bool hasMaidenName() const;

    QString fullName() const;

    void setMale();
    bool isMale() const;
    void setFemale();
    bool isFemale() const;

    void setBirthday(const QDate& birthdayOrNull);
    const QDate& birthday() const;
    bool hasBirthday() const;
    void setObit(const QDate& obitOrNull);
    const QDate& obit() const;
    bool hasObit() const;

    void setFather(Person* father);
    Person* father() const;
    ConnectionPointGraphicsItem* fatherConnectionPoint() const;

    void setMother(Person* mother);
    Person* mother() const;
    ConnectionPointGraphicsItem* motherConnectionPoint() const;

    void setSpouse(Person* spouse);
    Person* spouse() const;
    ConnectionPointGraphicsItem* spouseConnectionPoint() const;

    void addChild(Person* child);
    void removeChild(Person* child);
    const QList<Person*>& children() const;
    ConnectionPointGraphicsItem* childrenConnectionPoint() const;
    
    static bool isSpouse(Person* p1, Person* p2);
    
signals:
    void editPerson(Person* obj);

private:
    void updateChildren();

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    enum Sex {
        SexMale = 0,
        SexFemale,
    };

    QString _familyName;
    QString _givenName;
    QString _maidenName;
    QGraphicsTextItem* _nameText;
    Sex _sex;
    QDate* _birthday;
    QDate* _obit;
    
    Person* _father;
    ConnectionPointGraphicsItem* _fatherConnection;
    Person* _mother;
    ConnectionPointGraphicsItem* _motherConnection;
    Person* _spouse;
    ConnectionPointGraphicsItem* _spouseConnection;
    QList<Person*> _children;
    ConnectionPointGraphicsItem* _childrenConnection;
};

#endif
