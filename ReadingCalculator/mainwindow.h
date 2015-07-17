#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>

#define GRID_WIDTH 5

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void submit();

private:
    int get_days_difference();
    void clear_window();
    void remove_window_resize();

    Ui::MainWindow *ui;
    QWidget *widget;
    QGridLayout *layout;
    QGridLayout *line_layout;
    QCheckBox *include_selected_date_check_box;
    QCheckBox *include_current_date_check_box;
    QComboBox *day_combo_box;
    QComboBox *month_combo_box;
    QComboBox *year_combo_box;
    QLineEdit *read_pages_text_edit;
    QLineEdit *all_pages_text_edit;
    QPushButton *submit_push_button;
    QLabel *result_label;
};

#endif // MAINWINDOW_H
