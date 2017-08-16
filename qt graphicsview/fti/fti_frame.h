#ifndef FTI_FRAME_H
#define FTI_FRAME_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPropertyAnimation>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>
#include <QTime>

#include "powerlogo_widget.h"
#include "welcome_widget.h"
#include "connect_wifi_widget.h"

namespace Ui {
class FtiFrame;
}

class FtiFrame : public QWidget
{
    Q_OBJECT

public:
    explicit FtiFrame(QWidget *parent = 0);
    ~FtiFrame();
    void InitUiWidget();

protected:
    void keyReleaseEvent(QKeyEvent *keyevent);

private:
    void SwitchPage(QWidget *currentWidget, QWidget *newWidget);


    Ui::FtiFrame *ui;
    QVBoxLayout *verticalLayout_;
//    PowerLogoWidget *powerlog_widget_;
//    WelcomeUseWidget *welcom_use_widaget_;
    QGraphicsScene *sence_ ;
    QGraphicsView *view_;

    QWidget *current_widget_;
    unsigned short int current_page_num_;
};

void sleepms(unsigned int msec);

#endif // FTI_FRAME_H
