#ifndef FORM_SCARA_DISPLAY_H
#define FORM_SCARA_DISPLAY_H
#include "Gui/ui_Form_Scara.h"
#include "Form_ScaraArm.h"
#include <QtWidgets/QDialog>
#include <QtCore/QThread>

class Form_Scara : public QDialog
{
    Q_OBJECT

public:
	explicit Form_Scara(QWidget *parent = nullptr);
	virtual ~Form_Scara();
    

protected:
	void showEvent(QShowEvent *event);
	void closeEvent(QCloseEvent *event);

private slots:
    void on_Screw_btn_Stop_clicked();
    void on_ScaraArm_btn_Torque_On_clicked();
    void on_ScaraArm_btn_Torque_Off_clicked();

private:
	Ui::Form_Scara *ui;
	Form_ScaraArm *form_scara;
    QThread *thread_Scara;

    //Display
    std::thread *thread_display;
	bool _is_deleted_thread_display;
	void Display();
};


#endif  //FORM_SCARA_DISPLAY_H