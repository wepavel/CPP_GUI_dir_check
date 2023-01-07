#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <iostream>
#include <filesystem>
#include <vector>
#include <chrono>
#include <atomic>
#include <future>
#include <string>
#include <fstream>
#include <QCryptographicHash>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_released();
    void get_files(std::vector<std::string>& paths, const std::string& current_path);
    void folder_check();

//    std::vector<std::string> m_find_files( std::filesystem::path p, std::string substr );
    void m_folder_check();
    void save_dirs_as_file();


private:
    Ui::MainWindow *ui;
    std::vector<std::string> file_list ;
};
#endif // MAINWINDOW_H
