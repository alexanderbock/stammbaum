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

#ifndef __MAINWINDOW__
#define __MAINWINDOW__

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QMap>

class Person;
class PersonGraphicsItem;
class QAction;

class MyGraphicsView : public QGraphicsView {
    void wheelEvent(QWheelEvent* event);
};

class MyGraphicsScene : public QGraphicsScene {
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
};

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

private slots:
    void showAddPersonDialog();
    void deleteCurrentPerson();
    void openFile();
    void saveFile();
    void clearFamilyTree();
    void editPerson(Person* person);
    void sceneSelectionChanged();

private:
    QAction* _deleteAction;
    QGraphicsView* _graphicsView;
    QList<Person*> _familyTree;
};

#endif
