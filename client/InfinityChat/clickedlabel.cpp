#include "clickedlabel.h"
#include <QMouseEvent>

ClickedLabel::ClickedLabel(QWidget *parent):QLabel(parent),_curState(ClickLbState::Normal)
{

}

void ClickedLabel::SetState(QString normal, QString hover, QString press, QString select, QString select_hover, QString select_press)
{
    _normal = normal;
    _normal_hover = hover;
    _normal_press = press;
    _selected = select;
    _selected_hover = select_hover;
    _selected_press = select_press;
    setProperty("State",normal);
    repolish(this);
}

ClickLbState ClickedLabel::GetCurState()
{
    return _curState;
}

void ClickedLabel::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton) {
        if(_curState == ClickLbState::Normal){
              qDebug()<<"clicked , change to selected hover: "<< _selected_hover;
            _curState = ClickLbState::Selected;
            setProperty("State",_selected_hover);
            repolish(this);
            update();
        }else{
               qDebug()<<"clicked , change to normal hover: "<< _normal_hover;
            _curState = ClickLbState::Normal;
            setProperty("State",_normal_hover);
            repolish(this);
            update();
        }
        emit clicked();
    }
    // 调用基类的mousePressEvent以保证正常的事件处理
    QLabel::mousePressEvent(ev);
}

void ClickedLabel::enterEvent(QEvent *event)
{
    // 在这里处理鼠标悬停进入的逻辑
    if(_curState == ClickLbState::Normal){
         qDebug()<<"enter , change to normal hover: "<< _normal_hover;
        setProperty("State",_normal_hover);
        repolish(this);
        update();
    }else{
         qDebug()<<"enter , change to selected hover: "<< _selected_hover;
        setProperty("State",_selected_hover);
        repolish(this);
        update();
    }
    QLabel::enterEvent(event);
}

void ClickedLabel::leaveEvent(QEvent *event)
{
    // 在这里处理鼠标悬停离开的逻辑
    if(_curState == ClickLbState::Normal){
         qDebug()<<"leave , change to normal : "<< _normal;
        setProperty("State",_normal);
        repolish(this);
        update();
    }else{
         qDebug()<<"leave , change to normal hover: "<< _selected;
        setProperty("State",_selected);
        repolish(this);
        update();
    }
    QLabel::leaveEvent(event);
}
