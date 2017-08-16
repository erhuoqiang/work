#include "fti_frame.h"
#include "ui_fti_frame.h"
#include <QDebug>

FtiFrame::FtiFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FtiFrame),current_page_num_(0),current_widget_(nullptr)
{
    ui->setupUi(this);
   // this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    InitUiWidget();
}

FtiFrame::~FtiFrame()
{
    delete ui;
}

void FtiFrame::InitUiWidget()
{
    verticalLayout_ = new QVBoxLayout(ui->widget);
    verticalLayout_->setSpacing(0);
    verticalLayout_->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalLayout_->setContentsMargins(0, 0, 0, 0);

    current_widget_ = new PowerLogoWidget(ui->widget);
    current_widget_->show();
    verticalLayout_->addWidget(current_widget_);

    sence_ = new QGraphicsScene(ui->widget);
    view_ = new  QGraphicsView(sence_, ui->widget);
    view_->setFocusPolicy(Qt::NoFocus);
    view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_->hide();
    view_->setStyleSheet("QGraphicsView{background:transparent;}");
}

void FtiFrame::keyReleaseEvent(QKeyEvent *keyevent)
{
    QWidget *tempwidget = nullptr;
    if(keyevent->key() == Qt::Key_Right)
     {
        switch(current_page_num_)
        {
        case 0:
            tempwidget = new WelcomeWidget(nullptr);
            tempwidget->resize(current_widget_->size());
            SwitchPage(current_widget_, tempwidget);
            current_page_num_++;
            current_widget_ = tempwidget;
            break;
         case 1:
            tempwidget = new ConnectWifiWidget(nullptr);
            tempwidget->resize(current_widget_->size());
            SwitchPage(current_widget_, tempwidget);
            current_page_num_++;
            current_widget_ = tempwidget;
            break;
        default:
            break;
        }
     }
    else if(keyevent->key() == Qt::Key_Left)
    {
        switch(current_page_num_)
        {
        case 2:
            tempwidget = new WelcomeWidget(nullptr);
            tempwidget->resize(current_widget_->size());
            SwitchPage(current_widget_, tempwidget);
            current_page_num_--;
            current_widget_ = tempwidget;
            break;
        case 1:
            tempwidget = new PowerLogoWidget(nullptr);
            tempwidget->resize(current_widget_->size());
            SwitchPage(current_widget_, tempwidget);
            current_page_num_--;
            current_widget_ = tempwidget;
            break;
        default:
            break;
        }

    }
}

void FtiFrame::SwitchPage(QWidget *currentWidget, QWidget  *nextWidget)
{
    if(currentWidget == nullptr || nextWidget == nullptr)
    {
        return;
    }

   verticalLayout_->removeWidget(currentWidget);
   verticalLayout_->addWidget(view_);
   currentWidget->setParent(nullptr);

   QGraphicsWidget *next_graphics_widget = sence_->addWidget(nextWidget);
   QGraphicsWidget *current_graphics_widget = sence_->addWidget(currentWidget);
   nextWidget->setAttribute(Qt::WA_TranslucentBackground, true);
   currentWidget->setAttribute(Qt::WA_TranslucentBackground, true);

   view_->show();

   QRectF r = current_graphics_widget->boundingRect();

   for(int i=1; i<=90; ++i)
   {
       current_graphics_widget->setTransform(QTransform()
                            .rotate(i , Qt::YAxis));

                          //.translate(-r.width() / 2, -r.height() / 2));

       next_graphics_widget->setTransform(QTransform()
                            .rotate(270 + i, Qt::YAxis));
                        //  .translate(-r.width() / 2, -r.height() / 2));

//       next_graphics_widget->setY(0);
//       current_graphics_widget->setY(0);
//       current_graphics_widget->setPos(0,i*r.height() /90);
//       next_graphics_widget->setPos(0,-r.height()+i*r.height() /90);
       sleepms(5);
       view_->update();
   }


   verticalLayout_->removeWidget(view_);
   nextWidget->setParent(ui->widget);
   verticalLayout_->addWidget(nextWidget);
   sence_->removeItem(currentWidget->graphicsProxyWidget());
   sence_->removeItem(nextWidget->graphicsProxyWidget());
   currentWidget->graphicsProxyWidget()->setWidget(nullptr);
   nextWidget->graphicsProxyWidget()->setWidget(nullptr);

   nextWidget->show();
   delete currentWidget;
}


void sleepms(unsigned int msec)
{
        QTime dieTime = QTime::currentTime().addMSecs(msec);
        while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
