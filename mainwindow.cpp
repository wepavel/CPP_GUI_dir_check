#include "mainwindow.h"
#include "ui_mainwindow.h"
std::atomic<unsigned int> nthreads{1} ;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->statusbar->showMessage("Выберите папку");
    connect(ui->pushButton_2, SIGNAL(released()), this, SLOT(m_folder_check()));
    connect(ui->pushButton_3, SIGNAL(released()), this, SLOT(save_dirs_as_file()));
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_released()
{
    QString str;
    str = QFileDialog::getExistingDirectory(this, tr("Выбрать папку"), "/home/wepal", QFileDialog::ShowDirsOnly
                                            | QFileDialog::DontResolveSymlinks);

    ui->label->setText(str);
    qInfo() <<  str;
    QString check_path = "";
    if (str.toStdString().c_str()!=check_path)
    {
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(false);
        ui->statusbar->showMessage("Папка выбрана");
    }
    else
    {
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->statusbar->showMessage("Выберите папку");
    }

}

void MainWindow::get_files(std::vector<std::string>& paths, const std::string& current_path)
{
    for(const auto& file : std::filesystem::directory_iterator(current_path))
    {
        if(std::filesystem::is_directory(file))
        {
            get_files(paths, file.path().string());
            paths.push_back(file.path().string());
        }
        else
        {
            paths.push_back(file.path().string());
        }
    }
}

void MainWindow::folder_check()
{
   qInfo() <<  "I HATE C++";
    std::cout << "GOVNO" << std::endl;
   const auto start = std::chrono::steady_clock::now() ;
   std::vector<std::string>paths;

   get_files(paths, ui->label->text().toStdString());
//   get_files(paths, "/home/wepal");

   for(int i = 0; i< paths.size(); i++) std::cout << paths[i] << std::endl;

   const auto dur = std::chrono::steady_clock::now() - start ;
   qInfo()  << "elapsed wall-clock time " << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count() << " milliseconds.\n";

}




std::vector<std::string> m_find_files( std::filesystem::path p, std::string substr )
{
    std::cout << nthreads << ' ' ;
    ++nthreads ;
    std::vector<std::string> file_list ;

    auto begin = std::filesystem::recursive_directory_iterator(p);
    const auto end = std::filesystem::recursive_directory_iterator() ;

    for( ; begin != end ; ++begin )
    {
        const std::string path = begin->path().string() ;
        bool path1 = path.find( substr ) ;
        if( path.find( substr ) != std::string::npos ) file_list.push_back(path) ;
    }

    return file_list ;
}


void MainWindow::m_folder_check()
{

    nthreads = 1;
    using namespace std::chrono ;
    const auto start = steady_clock::now() ;

    std::vector<std::string> file_list ;

    auto begin = std::filesystem::directory_iterator(ui->label->text().toStdString());
    const auto end = std::filesystem::directory_iterator() ;

    std::vector< std::future< std::vector<std::string> > > futures ;
    for( ; begin != end ; ++begin )
    {
        const std::filesystem::path p = begin->path() ;
        if( std::filesystem::is_directory(p) )
            futures.emplace_back( std::async( std::launch::async, m_find_files, p, ".a" ) ) ;
        else
        {
            const std::string path_str = p.string() ;
            if( path_str.find( ".a" ) != std::string::npos ) MainWindow::file_list.push_back(path_str) ;
        }
    }

    for( auto& f : futures )
    {
        auto v = f.get() ;
        MainWindow::file_list.insert( MainWindow::file_list.end(), v.begin(), v.end() ) ;
    }

    const auto dur = steady_clock::now() - start ;

    for(int i = 0; i< MainWindow::file_list.size(); i++) std::cout << MainWindow::file_list[i] << std::endl;

//    std::cout << file_list.size() << " entries with '.a' in the path were added to the vector\n"
//              << "elapsed wall-clock time " << duration_cast<milliseconds>(dur).count() << " milliseconds.\n"
//              << "#threads: " << nthreads << '\n' ;
    qInfo() <<  file_list.size() << " entries with '.a' in the path were added to the vector\n"
             << "elapsed wall-clock time " << duration_cast<milliseconds>(dur).count() << " milliseconds.\n"
             << "#threads: " << nthreads << '\n' ;
    ui->statusbar->showMessage("Структура файлов определена");
    ui->pushButton_3->setEnabled(true);
    qInfo() <<  "I HATE C++";

}


//std::string get_full_sum(const std::string& file_path, Glib::Checksum::ChecksumType sum_type, size_t block_size)
//{
//    Glib::Checksum sum(sum_type);
//    std::ifstream file(file_path, std::ios::binary);
//    if( file.is_open() ){
//        std::string buffer;
//        buffer.resize(block_size);
//        do{
//            file.read(&(buffer[0]), block_size);
//            buffer.resize(file.gcount());
//            sum.update(buffer);
//        }while(buffer.size()>0);//!buffer.empty()
//    }
//    return sum.get_string();
//}



void MainWindow::save_dirs_as_file()
{
    std::cout << "GOVNO" << std::endl;
    QString save_path;
    QString save_path_new;
    QString filter = "Text files (*.txt);;(*.pdf)";
    QString exten_fin;
    save_path = QFileDialog::getSaveFileName(this, tr("Сохранить файл"), "/home/wepal", filter, &filter);
    if(!save_path.isNull())
    {
        save_path_new =  save_path.section( ".", 0, 0);
        QString exten =  filter.section( ".", -1, -1 );
        exten_fin =  exten.section( ")", -2, -2 );
        std::cout << "Я покакал" << std::endl;
    }

    QString output_file_path = save_path_new + "." + exten_fin;
    std::ofstream output_file(output_file_path.toStdString().c_str());

    if (save_path.section( ".", -1, -1).toStdString().c_str())
    {
        output_file <<"";
    }

    for(std::vector<std::string>::const_iterator i = MainWindow::file_list.begin(); i != MainWindow::file_list.end(); ++i) {
        output_file << *i << '\n';
    }
    QByteArray text_file;
    QFile file(output_file_path.toStdString().c_str());

    while (!file.atEnd()) {
        text_file.append(file.readLine());
    }
    ui->statusbar->showMessage("Файл сохранён");
//    QString s = "My string";
//    QByteArray ba = s.toUtf8();
    qDebug() <<  QCryptographicHash::hash(text_file, QCryptographicHash::Md5).toHex();
}
