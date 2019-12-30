#include <fteditor.h>
#include <QVBoxLayout>
#include <QGridLayout>

FTEditor::FTEditor(QWidget *parent):
    QMainWindow(parent)
{
    this->setWindowFlag(Qt::Tool);
    this->setFixedSize(50, 500);
}

FTEditor::~FTEditor()
{

}


