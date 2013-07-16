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

#include "mainwindow.h"
#include "persondialog.h"
#include "person.h"

#include <QAction>
#include <QDate>
#include <QDesktopServices>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>
#include <QTextStream>
#include <QToolBar>
#include <QWheelEvent>

#include <math.h>

namespace {
    const QString elementNamePersons = "Persons";
    const QString elementNamePerson = "Person";
    const QString attributeNameID = "id";
    const QString elementNamePosition = "Position";
    const QString attributeNamePositionX = "x";
    const QString attributeNamePositionY = "y";
    const QString elementNameName = "Name";
    const QString attributeNameFamilyName = "familyName";
    const QString attributeNameGivenName = "givenName";
    const QString attributeNameMaidenName = "maidenName";
    const QString elementNameSex = "Sex";
    const QString attributeNameSex = "isMale";
    const QString elementNameBirthday = "Birthday";
    const QString elementNameObit = "Obit";
    const QString attributeNameDate = "date";
    const QString elementNameFather = "Father";
    const QString elementNameMother = "Mother";
    const QString elementNameSpouse = "Spouse";
    const QString elementNameChildren = "Children";
    const QString elementNameChild = "Child";
}

void MyGraphicsView::wheelEvent(QWheelEvent* event) {
    QPointF p = mapToScene(event->pos());
    translate(-p.x(), -p.y());
    float factor = pow(2.0, event->delta() / 360.0);
    scale(factor, factor);
    translate(p.x(), p.y());
}

void MyGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    invalidate();
    QGraphicsScene::mouseMoveEvent(event);
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , _deleteAction(0)
    , _graphicsView(0)
{
    QToolBar* toolBar = addToolBar(""); 
    toolBar->addAction(tr("New"), this, SLOT(clearFamilyTree()));
    toolBar->addAction(tr("Open"), this, SLOT(openFile()));
    toolBar->addAction(tr("Save"), this, SLOT(saveFile()));
    toolBar->addSeparator();
    toolBar->addAction(tr("Add"), this, SLOT(showAddPersonDialog()));
    _deleteAction = toolBar->addAction(tr("Delete"), this, SLOT(deleteCurrentPerson()));
    
    _deleteAction->setEnabled(false);

    _graphicsView = new MyGraphicsView;
    _graphicsView->setMinimumSize(640, 480);
    QGraphicsScene* scene = new MyGraphicsScene;
    connect(scene, SIGNAL(selectionChanged()), this, SLOT(sceneSelectionChanged())), 
    _graphicsView->setScene(scene);
    scene->setBackgroundBrush(Qt::lightGray);
    setCentralWidget(_graphicsView);
}

MainWindow::~MainWindow() {
    clearFamilyTree();
}

void MainWindow::sceneSelectionChanged() {
    QList<QGraphicsItem*> selected = _graphicsView->scene()->selectedItems();
    _deleteAction->setEnabled(selected.size() == 1);
}

void MainWindow::clearFamilyTree() {
    for (int i = 0; i < _familyTree.size(); ++i) {
        _graphicsView->scene()->removeItem(_familyTree[i]);
        delete _familyTree[i];
    }
    _familyTree.clear();
}

void MainWindow::openFile() {
    clearFamilyTree();

    QString documentsFolder = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Stammbaum"), documentsFolder, tr("Stammbaum (*.xml)"));
    if (fileName == "")
        return;

    QFile file(fileName);
    bool openSuccess = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!openSuccess) {
        QMessageBox::critical(this, tr("Stammbaum"), tr("Failed to open file '%1'").arg(fileName));
        return;
    }

    QDomDocument doc("Stammbaum");
    QString errorString;
    int errorLine;
    int errorColumn;
    bool setContentSuccess = doc.setContent(&file, &errorString, &errorLine, &errorColumn);
    if (!setContentSuccess) {
        QMessageBox::critical(this, tr("Stammbaum"), tr("Failed to parse file.\nError message: '%1' at line: %2 and column: %3").arg(errorString).arg(errorLine).arg(errorColumn));
        return;
    }

    QMap<int, Person*> idMap;
    QDomElement persons = doc.firstChild().toElement();
    QDomElement personElement = persons.firstChildElement(elementNamePerson);

    while (!personElement.isNull()) {
        Person* person = new Person;
        connect(person, SIGNAL(editPerson(Person*)), this, SLOT(editPerson(Person*)));
        _familyTree.append(person);
        _graphicsView->scene()->addItem(person);

        int id = personElement.attribute(attributeNameID).toInt();
        idMap.insert(id, person);

        QDomElement positionElement = personElement.firstChildElement(elementNamePosition);
        qreal posX = positionElement.attribute(attributeNamePositionX).toDouble();
        qreal posY = positionElement.attribute(attributeNamePositionY).toDouble();
        person->setPos(posX, posY);

        QDomElement nameElement = personElement.firstChildElement(elementNameName);
        const QString& familyName = nameElement.attribute(attributeNameFamilyName);
        person->setFamilyName(familyName);
        const QString& givenName = nameElement.attribute(attributeNameGivenName);
        person->setGivenName(givenName);
        if (nameElement.hasAttribute(attributeNameMaidenName)) {
            const QString& maidenName = nameElement.attribute(attributeNameMaidenName);
            person->setMaidenName(maidenName);
        }

        QDomElement sexElement = personElement.firstChildElement(elementNameSex);
        int isMale = sexElement.attribute(attributeNameSex).toInt();
        if (isMale == 1)
            person->setMale();
        else
            person->setFemale();

        QDomElement birthdayElement = personElement.firstChildElement(elementNameBirthday);
        if (!birthdayElement.isNull()) {
            const QString& birthday = birthdayElement.attribute(attributeNameDate);
            const QDate& birthdayDate = QDate::fromString(birthday);
            person->setBirthday(birthdayDate);
        }

        QDomElement obitElement = personElement.firstChildElement(elementNameObit);
        if (!obitElement.isNull()) {
            const QString& obit = obitElement.attribute(attributeNameDate);
            const QDate& obitDate = QDate::fromString(obit);
            person->setObit(obitDate);
        }
        personElement = personElement.nextSiblingElement();
    }

    personElement = persons.firstChildElement(elementNamePerson);
    while (!personElement.isNull()) {
        int id = personElement.attribute(attributeNameID).toInt();
        Person* person = idMap[id];

        QDomElement fatherElement = personElement.firstChildElement(elementNameFather);
        if (!fatherElement.isNull()) {
            int fatherId = fatherElement.attribute(attributeNameID).toInt();
            Person* fatherPerson = idMap[fatherId];
            person->setFather(fatherPerson);
        }

        QDomElement motherElement = personElement.firstChildElement(elementNameMother);
        if (!motherElement.isNull()) {
            int motherId = motherElement.attribute(attributeNameID).toInt();
            Person* motherPerson = idMap[motherId];
            person->setMother(motherPerson);
        }

        QDomElement spouseElement = personElement.firstChildElement(elementNameSpouse);
        if (!spouseElement.isNull()) {
            int spouseId = spouseElement.attribute(attributeNameID).toInt();
            Person* spousePerson = idMap[spouseId];
            person->setSpouse(spousePerson);
        }

        QDomElement childrenElement = personElement.firstChildElement(elementNameChildren);
        if (!childrenElement.isNull()) {
            QDomElement childElement = personElement.firstChildElement(elementNameChild);
            while (!childElement.isNull()) {
                int childID = childElement.attribute(attributeNameID).toInt();
                Person* childPerson = idMap[childID];
                person->addChild(childPerson);
                childElement = childElement.nextSiblingElement();
            }
        }

        personElement = personElement.nextSiblingElement();
    }
}

void MainWindow::saveFile() {
    QDomDocument doc("Stammbaum");
    QDomElement persons = doc.createElement(elementNamePersons);
    doc.appendChild(persons);

    QMap<Person*, int> idMap;
    for (int i = 0; i < _familyTree.size(); ++i) {
        Person* currentPerson = _familyTree[i];
        idMap.insert(currentPerson, i);
    }
    for (int i = 0; i < _familyTree.size(); ++i) {
        Person* currentPerson = _familyTree[i];
        QDomElement personElement = doc.createElement(elementNamePerson);
        personElement.setAttribute(attributeNameID, idMap[currentPerson]);

        QDomElement positionElement = doc.createElement(elementNamePosition);
        positionElement.setAttribute(attributeNamePositionX, currentPerson->pos().x());
        positionElement.setAttribute(attributeNamePositionY, currentPerson->pos().y());
        personElement.appendChild(positionElement);

        QDomElement nameElement = doc.createElement(elementNameName);
        nameElement.setAttribute(attributeNameFamilyName, currentPerson->familyName());
        nameElement.setAttribute(attributeNameGivenName, currentPerson->givenName());
        if (currentPerson->hasMaidenName())
            nameElement.setAttribute(attributeNameMaidenName, currentPerson->maidenName());
        personElement.appendChild(nameElement);

        QDomElement sexElement = doc.createElement(elementNameSex);
        sexElement.setAttribute(attributeNameSex, currentPerson->isMale());
        personElement.appendChild(sexElement);

        if (currentPerson->hasBirthday()) {
            QDomElement birthdayElement = doc.createElement(elementNameBirthday);
            birthdayElement.setAttribute(attributeNameDate, currentPerson->birthday().toString());
            personElement.appendChild(birthdayElement);
        }
        if (currentPerson->hasObit()) {
            QDomElement obitElement = doc.createElement(elementNameObit);
            obitElement.setAttribute(attributeNameDate, currentPerson->obit().toString());
            personElement.appendChild(obitElement);
        }
        if (currentPerson->father()) {
            QDomElement fatherElement = doc.createElement(elementNameFather);
            fatherElement.setAttribute(attributeNameID, idMap[currentPerson->father()]);
            personElement.appendChild(fatherElement);
        }
        if (currentPerson->mother()) {
            QDomElement motherElement = doc.createElement(elementNameMother);
            motherElement.setAttribute(attributeNameID, idMap[currentPerson->mother()]);
            personElement.appendChild(motherElement);
        }
        if (currentPerson->spouse()) {
            QDomElement spouseElement = doc.createElement(elementNameSpouse);
            spouseElement.setAttribute(attributeNameID, idMap[currentPerson->spouse()]);
            personElement.appendChild(spouseElement);
        }
        if (currentPerson->children().size() > 0) {
            QDomElement childrenElement = doc.createElement(elementNameChildren);
            for (int j = 0; j < currentPerson->children().size(); ++j) {
                Person* currentChild = currentPerson->children()[j];
                QDomElement childElement = doc.createElement(elementNameChild);
                childElement.setAttribute(attributeNameID, idMap[currentChild]);
                childrenElement.appendChild(childElement);
            }
            personElement.appendChild(childrenElement);
        }

        persons.appendChild(personElement);
    }

    QString documentsFolder = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    QString fileName = QFileDialog::getSaveFileName(this, tr("Stammbaum"), documentsFolder, tr("Stammbaum (*.xml)"));
    if (fileName == "")
        return;

    QFile file(fileName);
    bool openSuccess = file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (openSuccess) {
        QTextStream stream(&file);
        stream << doc.toString();
        file.close();
    }
    else {
        QMessageBox::critical(this, tr("Stammbaum"), tr("Failed to open file '%1'").arg(fileName));
    }
}

void MainWindow::showAddPersonDialog() {
    Person* person = new Person;
    connect(person, SIGNAL(editPerson(Person*)), this, SLOT(editPerson(Person*)));
    person->hide();
    _graphicsView->scene()->addItem(person);
    PersonDialog dialog(person, _familyTree);
    int result = dialog.exec();
    if (result == QDialog::Accepted) {
        person->show();
        _familyTree.append(person);
    } else {
        _graphicsView->scene()->removeItem(person);
        delete person;
    }
}

void MainWindow::deleteCurrentPerson() {
    QList<QGraphicsItem*> selected = _graphicsView->scene()->selectedItems();
    Q_ASSERT(selected.size() == 1);
    
    Person* person = static_cast<Person*>(selected.at(0));
    if (person->father())
        person->father()->removeChild(person);
    if (person->mother())
        person->mother()->removeChild(person);
    if (person->spouse())
        person->spouse()->setSpouse(0);
    foreach (Person* child, person->children()) {
        if (person->isMale())
            child->setFather(0);
        else
            child->setMother(0);
    }
    
    _familyTree.removeOne(person);
    _graphicsView->scene()->removeItem(person);
    delete person;
}

void MainWindow::editPerson(Person* person) {
    PersonDialog dialog(person, _familyTree);
    dialog.exec();
}
