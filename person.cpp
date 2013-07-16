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

#include "person.h"

#include "connectionpointgraphicsitem.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>

Person::Person()
    : _familyName("")
    , _givenName("")
    , _maidenName("")
    , _sex(SexMale)
    , _birthday(0)
    , _obit(0)
    , _father(0)
    , _fatherConnection(0)
    , _mother(0)
    , _motherConnection(0)
    , _spouse(0)
    , _spouseConnection(0)
    , _childrenConnection(0)
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);

    _nameText = new QGraphicsTextItem(fullName(), this);
    _fatherConnection = new ConnectionPointGraphicsItem(this);
    _motherConnection = new ConnectionPointGraphicsItem(this);
    _spouseConnection = new ConnectionPointGraphicsItem(this);
    _childrenConnection = new ConnectionPointGraphicsItem(this);

    updateChildren();
}

QRectF Person::boundingRect() const {
    return _nameText->boundingRect().adjusted(-10.0, -5.0, 10.0, 5.0);
}

void Person::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* ) {
    const QRectF& rect = boundingRect();

    if (option->state & QStyle::State_Selected) {
        painter->setBrush(Qt::lightGray);
    }
    else {
        painter->setBrush(Qt::white);        
    }
    painter->drawRect(rect);
}

void Person::mouseDoubleClickEvent(QGraphicsSceneMouseEvent*) {
    emit editPerson(this);
}

void Person::updateChildren() {
    _nameText->setPlainText(fullName());

    const QRectF& rect = boundingRect();
    const QRectF& fatherRect = _fatherConnection->boundingRect();
    _fatherConnection->setPos(rect.center().x() - fatherRect.center().x() - (rect.center().x() - rect.topLeft().x()) / 3.0, rect.top() - fatherRect.y());

    const QRectF& motherRect = _motherConnection->boundingRect();
    _motherConnection->setPos(rect.center().x() - motherRect.center().x() + (rect.center().x() - rect.topLeft().x()) / 3.0, rect.top() - motherRect.y());

    const QRectF& spouseRect = _spouseConnection->boundingRect();
    if (isMale())
        _spouseConnection->setPos(rect.right(), rect.center().y() - spouseRect.center().y());
    else if (isFemale())
        _spouseConnection->setPos(rect.left() - spouseRect.x(), rect.center().y() - spouseRect.center().y());

    const QRectF& childRect = _childrenConnection->boundingRect();
    _childrenConnection->setPos(rect.center().x() - childRect.center().x(), rect.bottom());

}

void Person::setFamilyName(const QString& familyName) {
    _familyName = familyName;
    updateChildren();
}

const QString& Person::familyName() const {
    return _familyName;
}

void Person::setGivenName(const QString& givenName) {
    _givenName = givenName;
    updateChildren();
}

const QString& Person::givenName() const {
    return _givenName;
}

void Person::setMaidenName(const QString& maidenName) {
    _maidenName = maidenName;
    updateChildren();
}

const QString& Person::maidenName() const {
    return _maidenName;
}

bool Person::hasMaidenName() const {
    return _maidenName != "";
}

QString Person::fullName() const {
    if (hasMaidenName())
        return familyName() + ", " + givenName() + " (" + maidenName() + ")";
    else
        return familyName() + ", " + givenName();
}

void Person::setMale() {
    _sex = SexMale;
    updateChildren();
}

bool Person::isMale() const {
    return _sex == SexMale;
}

void Person::setFemale() {
    _sex = SexFemale;
    updateChildren();
}

bool Person::isFemale() const {
    return _sex == SexFemale;
}

void Person::setBirthday(const QDate& birthdayOrNull) {
    delete _birthday;
    _birthday = new QDate(birthdayOrNull);
    updateChildren();
}

const QDate& Person::birthday() const {
    return *_birthday;
}

bool Person::hasBirthday() const {
    return _birthday != 0;
}

void Person::setObit(const QDate& obitOrNull) {
    delete _obit;
    _obit = new QDate(obitOrNull);
    updateChildren();
}

const QDate& Person::obit() const {
    return *_obit;
}

bool Person::hasObit() const {
    return _obit != 0;
}

void Person::setFather(Person* father) {
    if (_father == father)
        return;

    if (_father) {
        _fatherConnection->removeConnection();
    }

    _father = father;
    if (_father) {
        _father->addChild(this);
        _fatherConnection->createConnectionTo(_father->childrenConnectionPoint());
    }
}

Person* Person::father() const {
    return _father;
}

ConnectionPointGraphicsItem* Person::fatherConnectionPoint() const {
    return _fatherConnection;
}

void Person::setMother(Person* mother) {
    if (_mother == mother)
        return;

    if (_mother) {
        _motherConnection->removeConnection();
    }

    _mother = mother;
    if (_mother) {
        _mother->addChild(this);
        _motherConnection->createConnectionTo(_mother->childrenConnectionPoint());
    }
}

Person* Person::mother() const {
    return _mother;
}

ConnectionPointGraphicsItem* Person::motherConnectionPoint() const {
    return _motherConnection;
}

void Person::setSpouse(Person* spouse) {
    if (_spouse == spouse)
        return;

    if (_spouse) {
        _spouseConnection->removeConnection();
    }

    _spouse = spouse;
    if (_spouse) {
        _spouse->setSpouse(this);
        _spouseConnection->createConnectionTo(_spouse->spouseConnectionPoint());
    }
}

Person* Person::spouse() const {
    return _spouse;
}

ConnectionPointGraphicsItem* Person::spouseConnectionPoint() const {
    return _spouseConnection;
}

void Person::addChild(Person* child) {
    Q_ASSERT(child);
    if (!_children.contains(child)) {
        _children.append(child);
        if (isMale())
            child->setFather(this);
        else if (isFemale())
            child->setMother(this);
    }
}

void Person::removeChild(Person* child) {
    Q_ASSERT(child);
    _children.removeOne(child);
}

const QList<Person*>& Person::children() const {
    return _children;
}

ConnectionPointGraphicsItem* Person::childrenConnectionPoint() const {
    return _childrenConnection;
}

bool Person::isSpouse(Person* p1, Person* p2) {
    return (p1->spouse() == p2) || (p2->spouse() == p1);
}
