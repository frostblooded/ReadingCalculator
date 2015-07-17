#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QGridLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <string>
#include <ctime>
#include <QStatusBar>
#include <math.h>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //this code is dark and full of terrors
    //BEWARE
    //I don't like it either
    //*shrug*

    ui->setupUi(this);

    int current_line = 0;

    clear_window();
    remove_window_resize();

    widget = new QWidget(this);
    layout = new QGridLayout(widget);
    line_layout = new QGridLayout(widget); //the sub layout is used for every line so that it looks better

    line_layout->addWidget(new QLabel("Include selected date?", widget), 0, 0);

    include_selected_date_check_box = new QCheckBox(widget);
    include_selected_date_check_box->setChecked(false);
    line_layout->addWidget(include_selected_date_check_box, 0, 1);

    layout->addLayout(line_layout, current_line, 0, 1, GRID_WIDTH);
    current_line++;
    line_layout = new QGridLayout(widget); //clear layout

    line_layout->addWidget(new QLabel("Include current date?", widget), 0, 0);

    include_current_date_check_box = new QCheckBox(widget);
    include_current_date_check_box->setChecked(false);
    line_layout->addWidget(include_current_date_check_box, 0, 1);

    layout->addLayout(line_layout, current_line, 0, 1, GRID_WIDTH);
    current_line++;
    line_layout = new QGridLayout(widget); //clear layout

    layout->addWidget(new QLabel("Select deadline:", widget), current_line, 0, 1, GRID_WIDTH);
    current_line++;

    //date combo box
    day_combo_box = new QComboBox(widget);

    for(int i = 0; i < 31; i++){
        day_combo_box->addItem(QString::number(i + 1));
    }

    line_layout->addWidget(day_combo_box, 0, 0);

    //month combo box
    month_combo_box = new QComboBox(widget);
    month_combo_box->addItem("January");
    month_combo_box->addItem("February");
    month_combo_box->addItem("March");
    month_combo_box->addItem("April");
    month_combo_box->addItem("May");
    month_combo_box->addItem("June");
    month_combo_box->addItem("July");
    month_combo_box->addItem("August");
    month_combo_box->addItem("September");
    month_combo_box->addItem("October");
    month_combo_box->addItem("November");
    month_combo_box->addItem("December");
    line_layout->addWidget(month_combo_box, 0, 1);

    //year text box
    year_combo_box = new QComboBox(widget);

    for(int i = 2015; i <= 2050; i++){
        year_combo_box->addItem(QString::number(i));
    }

    line_layout->addWidget(year_combo_box, 0, 2);
    layout->addLayout(line_layout, current_line, 0, 1, GRID_WIDTH);
    current_line++;
    line_layout = new QGridLayout(widget); //clear layout

    line_layout->addWidget(new QLabel("I have read", widget), 3, 0);

    read_pages_text_edit = new QLineEdit(widget);
    line_layout->addWidget(read_pages_text_edit, 3, 1);

    line_layout->addWidget(new QLabel("from", widget), 3, 2);

    all_pages_text_edit = new QLineEdit(widget);
    line_layout->addWidget(all_pages_text_edit, 3, 3);

    line_layout->addWidget(new QLabel("pages", widget), 3, 4);
    layout->addLayout(line_layout, current_line, 0, 1, GRID_WIDTH);
    current_line++;
    line_layout = new QGridLayout(widget);

    submit_push_button = new QPushButton("Calculate", widget);
    layout->addWidget(submit_push_button, current_line, 0, 1, GRID_WIDTH);
    current_line++;
    connect(submit_push_button, SIGNAL(clicked()), this, SLOT(submit()));

    result_label = new QLabel("You should read ... pages per day", widget);
    layout->addWidget(result_label, current_line, 0, 1, GRID_WIDTH);
    current_line++;

    widget->setLayout(layout);
    setCentralWidget(widget);
}

void MainWindow::clear_window()
{
    this->removeToolBar(ui->mainToolBar);
    statusBar()->hide();
}

void MainWindow::remove_window_resize()
{
    this->setFixedSize(this->minimumWidth(), this->minimumHeight());
}

void MainWindow::submit()
{
    if(check_pages_input()){
        int read_pages = read_pages_text_edit->text().toInt();
        int all_pages = all_pages_text_edit->text().toInt();

        int pages_to_read = all_pages - read_pages;
        int days_to_read = get_days_difference();

        int pages_per_day;

        if(days_to_read != 0){
            pages_per_day = ceil(pages_to_read / days_to_read); //round upwards so that it is accurate
            result_label->setText("You should read " + QString::number(pages_per_day) + " pages per day");
        }
        else{
            display_error("Please select a number of days other than 0!");
        }
    }
    else{
        display_error("Please enter valid page count!");
    }
}

int MainWindow::get_days_difference()
{
    int day = day_combo_box->currentIndex() + 1; //index starts at 0
    int month = month_combo_box->currentIndex() + 1; //index starts 0
    int year = year_combo_box->currentText().toInt();

    //slight modifications because of the way Zthe data is received
    tm time_mark = {0, 0, 0, day, month - 1, year - 1900};
    time_t selected_date = mktime(&time_mark);
    time_t current_time = time(NULL);
    tm *curr_time = localtime(&current_time);

    //set clock to 12 hours at midnight, so that the days inbetween is a whole number
    curr_time->tm_sec = 0;
    curr_time->tm_min = 0;
    curr_time->tm_hour = 0;
    current_time = mktime(curr_time);

    double date_difference = difftime(selected_date, current_time) / (60 * 60 * 24);

    date_difference = floor(date_difference);
    date_difference--; //the result is the difference without the current and the selected dates

    if(include_current_date_check_box->isChecked()){
        date_difference++;
    }

    if(include_selected_date_check_box->isChecked()){
        date_difference++;
    }

    result_label->setText(QString::number(date_difference));

    return date_difference;
}

bool MainWindow::check_pages_input()
{
    QString text = read_pages_text_edit->text();
    for(int i = 0; i < text.length(); i++){
        if(text[i] < '0' || text[i] > '9')
            return false;
    }

    text = all_pages_text_edit->text();
    for(int i = 0; i < text.length(); i++){
        if(text[i] < '0' || text[i] > '9')
            return false;
    }

    return true;
}

void MainWindow::display_error(QString message)
{
    QMessageBox message_box;
    message_box.critical(0, "Error!", message);
    message_box.show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
