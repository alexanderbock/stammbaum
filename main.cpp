/***********************************************************************
 *                                                                     *
 * Stammbaum                                                           *
 *                                                                     *
 * Copyright (c) 2012, Alexander Bock                                  *
 *                                                                     *
 * This work is licensed under the                                     *
 * Creative Commons Attribution-ShareAlike 3.0 Unported License.       *
 *                                                                     *
 * To view a copy of this license, visit                               *
 * http://creativecommons.org/licenses/by-sa/3.0/                      *
 * or send a letter to                                                 *
 * Creative Commons, 444 Castro Street, Suite 900,                     *
 * Mountain View, California, 94041, USA.                              *
 *                                                                     *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS *
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT   *
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS   *
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE     *
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, *
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES            *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR  *
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)  *
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, *
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)       *
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED *
 * OF THE POSSIBILITY OF SUCH DAMAGE.                                  *
 *                                                                     *
 **********************************************************************/

#include "person.h"
#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QTranslator translator;
    QString locale = QLocale::system().name();
    translator.load(QString("stammbaum_") + locale);
    app.installTranslator(&translator);

    MainWindow mainWindow;
    mainWindow.show();

    app.exec();

    exit(EXIT_SUCCESS);
}