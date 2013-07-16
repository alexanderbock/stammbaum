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

#include "persondialog.h"
#include "person.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QDialogButtonBox>
#include <QGraphicsScene>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>

PersonDialog::PersonDialog(Person* person, const QList<Person*>& persons)
    : QDialog()
    , _person(person)
    , _persons(persons)
{
    QGridLayout* layout = new QGridLayout;

    QLabel* familyNameLabel = new QLabel(tr("Family Name"));
    layout->addWidget(familyNameLabel, 0, 0);
    _familyNameEdit = new QLineEdit;
    _familyNameEdit->setText(person->familyName());
    layout->addWidget(_familyNameEdit, 0, 1, 1, 2);

    QLabel* givenNameLabel = new QLabel(tr("Given Name"));
    layout->addWidget(givenNameLabel, 1, 0);
    _givenNameEdit = new QLineEdit;
    _givenNameEdit->setText(person->givenName());
    layout->addWidget(_givenNameEdit, 1, 1, 1, 2);

    QLabel* maidenNameLabel = new QLabel(tr("Maiden Name"));
    layout->addWidget(maidenNameLabel, 2, 0);
    _maidenNameEdit = new QLineEdit;
    _maidenNameEdit->setText(person->maidenName());
    layout->addWidget(_maidenNameEdit, 2, 1, 1, 2);

    QLabel* sexLabel = new QLabel(tr("Sex"));
    layout->addWidget(sexLabel, 3, 0);
    _sexMaleRadioButton = new QRadioButton(tr("Male"));
    _sexMaleRadioButton->setChecked(person->isMale());
    _sexFemaleRadioButton = new QRadioButton(tr("Female"));
    _sexFemaleRadioButton->setChecked(person->isFemale());
    layout->addWidget(_sexMaleRadioButton, 3, 1);
    layout->addWidget(_sexFemaleRadioButton, 3, 2);

    QLabel* birthdayLabel = new QLabel(tr("Birthday"));
    layout->addWidget(birthdayLabel, 4, 0);
    _hasBirthday = new QCheckBox;
    _hasBirthday->setChecked(person->hasBirthday());
    layout->addWidget(_hasBirthday, 4, 1);
    _birthdayEdit = new QDateEdit;
    _birthdayEdit->setDisplayFormat("dd.MM.yyyy");
    if (person->hasBirthday())
        _birthdayEdit->setDate(person->birthday());
    _birthdayEdit->setEnabled(person->hasBirthday());
    layout->addWidget(_birthdayEdit, 4, 2);
    connect(_hasBirthday, SIGNAL(toggled(bool)), _birthdayEdit, SLOT(setEnabled(bool)));

    QLabel* obitLabel = new QLabel(tr("Obit"));
    layout->addWidget(obitLabel, 5, 0);
    _hasObit = new QCheckBox;
    _hasObit->setChecked(person->hasObit());
    layout->addWidget(_hasObit, 5, 1);
    _obitEdit = new QDateEdit;
    _obitEdit->setDisplayFormat("dd.MM.yyyy");
    if (person->hasObit())
        _obitEdit->setDate(person->obit());
    _obitEdit->setEnabled(person->hasObit());
    layout->addWidget(_obitEdit, 5, 2);
    connect(_hasObit, SIGNAL(toggled(bool)), _obitEdit, SLOT(setEnabled(bool)));

    QLabel* fatherLabel = new QLabel(tr("Father"));
    layout->addWidget(fatherLabel, 7, 0);
    _fatherComboBox = new QComboBox;
    _fatherComboBox->addItem("", QVariant(-1));
    int fatherIndex = -1;
    for (int i = 0; i < persons.size(); ++i) {
        Person* currentPerson = persons.at(i);
        if (currentPerson == person->father())
            fatherIndex = i;
        if (currentPerson->isMale())
            _fatherComboBox->addItem(currentPerson->fullName(), QVariant(i));
    }
    _fatherComboBox->setCurrentIndex(_fatherComboBox->findData(QVariant(fatherIndex)));
    layout->addWidget(_fatherComboBox, 7, 1, 1, 2);

    QLabel* motherLabel = new QLabel(tr("Mother"));
    layout->addWidget(motherLabel, 8, 0);
    _motherComboBox = new QComboBox;
    _motherComboBox->addItem("", QVariant(-1));
    int motherIndex = -1;
    for (int i = 0; i < persons.size(); ++i) {
        Person* currentPerson = persons.at(i);
        if (currentPerson == person->mother())
            motherIndex = i;

        if (currentPerson->isFemale())
            _motherComboBox->addItem(currentPerson->fullName(), QVariant(i));
    }
    _motherComboBox->setCurrentIndex(_motherComboBox->findData(QVariant(motherIndex)));
    layout->addWidget(_motherComboBox, 8, 1, 1, 2);

    QLabel* spouseLabel = new QLabel(tr("Spouse"));
    layout->addWidget(spouseLabel, 6, 0);
    _spouseComboBox = new QComboBox;
    _spouseComboBox->addItem("", QVariant(-1));
    int spouseIndex = -1;
    for (int i = 0; i < persons.size(); ++i) {
        Person* currentPerson = persons.at(i);
        if (currentPerson == person->spouse())
            spouseIndex = i;

        _spouseComboBox->addItem(currentPerson->fullName(), QVariant(i));
    }
    _spouseComboBox->setCurrentIndex(_spouseComboBox->findData(QVariant(spouseIndex)));
    layout->addWidget(_spouseComboBox, 6, 1, 1, 2);

    QLabel* childrenLabel = new QLabel(tr("Children"));
    layout->addWidget(childrenLabel, 9, 0);
    _childrenList = new QListWidget;
    layout->addWidget(_childrenList, 9, 1, 1, 2);
    _childAddComboBox = new QComboBox;
    _childAddComboBox->addItem("", QVariant(-1));
    for (int i = 0; i < persons.size(); ++i) {
        Person* currentPerson = persons.at(i);
        if (person->children().contains(currentPerson)) {
            QListWidgetItem* childItem = new QListWidgetItem(currentPerson->fullName(), _childrenList);
            childItem->setData(Qt::UserRole, QVariant(i));
            _childrenList->addItem(childItem);
        }

        else {
            if (currentPerson != person)
                _childAddComboBox->addItem(currentPerson->fullName(), QVariant(i));
        }
    }
    layout->addWidget(_childAddComboBox, 10, 1);
    QPushButton* childAddButton = new QPushButton("+");
    layout->addWidget(childAddButton, 10, 2);
    connect(childAddButton, SIGNAL(clicked(bool)), this, SLOT(addChild()));


    QDialogButtonBox* dialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(dialogButtonBox, 11, 2, 1, 1, Qt::AlignRight);
    connect(dialogButtonBox, SIGNAL(accepted()), this, SLOT(createPerson()));
    connect(dialogButtonBox, SIGNAL(rejected()), this, SLOT(reject()));
    
    setLayout(layout);
}

void PersonDialog::addChild() {
    int childIndex = _childAddComboBox->currentIndex();
    if (childIndex > 0) {
        int childID = _childAddComboBox->itemData(childIndex).toInt();
        Person* childPerson = _persons.at(childID);
        QListWidgetItem* childItem = new QListWidgetItem(childPerson->fullName(), _childrenList);
        childItem->setData(Qt::UserRole, QVariant(childID));
        _childrenList->addItem(childItem);
        _childAddComboBox->removeItem(childIndex);
    }
}

void PersonDialog::createPerson() {
    // sanity checks
    if (_familyNameEdit->text().isEmpty()) {
        QMessageBox::critical(this, tr("New Person Dialog"), tr("Person must have a family name"), QMessageBox::Ok);
        return;
    }
    if (_givenNameEdit->text().isEmpty()) {
        QMessageBox::critical(this, tr("New Person Dialog"), tr("Person must have a given name"), QMessageBox::Ok);
        return;
    }

    if (_hasBirthday->isChecked() && _hasObit->isChecked() && _birthdayEdit->date() > _obitEdit->date()) {
        QMessageBox::critical(this, tr("New Person Dialog"), tr("Obit is after the birthday"), QMessageBox::Ok);
        return;
    }

    _person->setFamilyName(_familyNameEdit->text());
    _person->setGivenName(_givenNameEdit->text());
    _person->setMaidenName(_maidenNameEdit->text());
    if (_sexMaleRadioButton->isChecked())
        _person->setMale();
    else
        _person->setFemale();
    if (_hasBirthday->isChecked())
        _person->setBirthday(_birthdayEdit->date());
    if (_hasObit->isChecked())
        _person->setObit(_obitEdit->date());
    
    int spouseIndex = _spouseComboBox->currentIndex();
    int spouseID = _spouseComboBox->itemData(spouseIndex).toInt();
    if (spouseID != -1) {
        Person* spouse = _persons.at(spouseID);
        _person->setSpouse(spouse);
    }

    int fatherIndex = _fatherComboBox->currentIndex();
    int fatherID = _fatherComboBox->itemData(fatherIndex).toInt();
    if (fatherID != -1) {
        Person* father = _persons.at(fatherID);
        _person->setFather(father);
    }
    else {
        if (_person->father())
            _person->father()->removeChild(_person);
        _person->setFather(0);
    }

    int motherIndex = _motherComboBox->currentIndex();
    int motherID = _motherComboBox->itemData(motherIndex).toInt();
    if (motherID != -1) {
        Person* mother = _persons.at(motherID);
        _person->setMother(mother);
    }
    else {
        if (_person->mother())
            _person->mother()->removeChild(_person);
        _person->setMother(0);
    }

    for (int i = 0; i < _childrenList->count(); ++i) {
        QListWidgetItem* item = _childrenList->item(i);
        int childID = item->data(Qt::UserRole).toInt();
        Person* childPerson = _persons.at(childID);
        _person->addChild(childPerson);
    }

    accept();
}
