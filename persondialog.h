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

#ifndef __NEWPERSONDIALOG__
#define __NEWPERSONDIALOG__

#include <QDialog>
#include <QList>

class Person;
class QCheckBox;
class QComboBox;
class QDateEdit;
class QGraphicsScene;
class QLineEdit;
class QListWidget;
class QRadioButton;

class PersonDialog : public QDialog {
Q_OBJECT
public:
    PersonDialog(Person* person, const QList<Person*>& persons);

private slots:
    void createPerson();
    void addChild();

private:
    QLineEdit* _familyNameEdit;
    QLineEdit* _givenNameEdit;
    QLineEdit* _maidenNameEdit;
    QRadioButton* _sexMaleRadioButton;
    QRadioButton* _sexFemaleRadioButton;
    QCheckBox* _hasBirthday;
    QDateEdit* _birthdayEdit;
    QCheckBox* _hasObit;
    QDateEdit* _obitEdit;
    QComboBox* _spouseComboBox;
    QComboBox* _fatherComboBox;
    QComboBox* _motherComboBox;
    QListWidget* _childrenList;
    QComboBox* _childAddComboBox;

    Person* _person;
    QGraphicsScene* _scene;
    const QList<Person*>& _persons;
};

#endif
