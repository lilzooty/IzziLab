#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <draggablebutton.h>

using std::vector;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onAndGateClicked();
    void onOrGateClicked();
    void onInverterClicked();
    void onWireClicked();


private:
    Ui::MainWindow *ui;

    vector<DraggableButton*> andGates;
    vector<DraggableButton*> orGates;
    vector<DraggableButton*> inverters;
    vector<DraggableButton*> norGates;
    vector<DraggableButton*> nandGates;
    vector<DraggableButton*> xorGates;

};
#endif // MAINWINDOW_H
