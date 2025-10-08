#ifndef QUESTIONWIDGET_H
#define QUESTIONWIDGET_H

#include <QWidget>
#include <QCloseEvent>
class QuestionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QuestionWidget(QWidget *parent = nullptr);

signals:
protected:
    void closeEvent(QCloseEvent *event) override {
        // Fenster darf nicht geschlossen werden
        event->ignore();
    }

};

#endif // QUESTIONWIDGET_H
