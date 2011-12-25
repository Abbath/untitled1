#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("cat.jpg"));
    Status = new QLabel("Hello");
    this->Operation =1 ;
    this->engine = new Engine();
    ui->statusBar->addWidget(this->Status);
    ui->label->setScaledContents(true);
    ui->pushButton->setToolTip("Run");
    ui->pushButton_2->setToolTip("Open data file");
    ui->pushButton_3->setToolTip("Open BMP | WAV file");
    ui->pushButton_4->setToolTip("Open Output file");
    ui->plainTextEdit->hide();
    this->adjustSize();

    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(mainslot()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(OpenFile1()));
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(OpenFile2()));
    connect(ui->pushButton_4,SIGNAL(clicked()),this,SLOT(OpenFile3()));
    connect(ui->actionHide,SIGNAL(triggered()),this,SLOT(Hide()));
    connect(ui->actionUnhide,SIGNAL(triggered()),this,SLOT(Unhide()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(About()));
    connect(ui->actionHelp,SIGNAL(triggered()),this,SLOT(Help()));
    connect(ui->actionFrom_Keyboard,SIGNAL(triggered()),this,SLOT(Hide()));
    connect(ui->actionQuit,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionFrom_Keyboard,SIGNAL(triggered()),this,SLOT(ShowDialog()));
    connect(ui->checkBox,SIGNAL(clicked()),this,SLOT(Noise()));
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::ShowDialog() {
    if(ui->plainTextEdit->isHidden()){
        ui->plainTextEdit->show();
        this->Hide();
    }
    else {
        ui->plainTextEdit->hide();
    }
    this->centralWidget()->adjustSize();
    this->adjustSize();
}
void MainWindow::OpenFile1(){
    QString s = QFileDialog::getOpenFileName(0,"Choose input file","./",0,0,0);
    ui->lineEdit->setText(s);
    Status->setText(" ");
}
void MainWindow::OpenFile2(){
    QString s = QFileDialog::getOpenFileName(0,"Choose check file","./",0,0,0);
    ui->lineEdit_2->setText(s);
    Status->setText(" ");
}

void MainWindow::OpenFile3(){
    QString s = QFileDialog::getOpenFileName(0,"Choose output file","./",0,0,0);
    ui->lineEdit_3->setText(s);
    Status->setText(" ");
}
void MainWindow::About(){
    QMessageBox::about(this, tr("About Program"),
                       tr("<h2>Steganography 0.2</h2>"
                          "<p>Copyright &copy; 2011 Lizanets Danylo.</p>",0
                          ));
}
void MainWindow::Help(){
    QMessageBox::information(this,
                             tr("Help"),
                             tr("Choose option in menu Operation, \nChoose Input files,\
                                and output file if you want (sets \"result\" for default) \nPush the \"Start\" button.")
                                    ,0,0,0);
}
void MainWindow::Hide(){
    this->Operation =1;
    ui->label->setText("Input Data");
    ui->label_2->setText("BMP | WAV");
    ui->label_4->setText("Hide");
    ui->pushButton_2->setToolTip("Open data file");
    ui->pushButton_3->setToolTip("Open BMP | WAV file");
    Status->setText(" ");
    ui->checkBox->setEnabled(true);
}
void MainWindow::Unhide(){
    this->Operation =2;
    ui->plainTextEdit->hide();
    ui->centralWidget->adjustSize();
    this->adjustSize();
    ui->label->setText("BMP | WAV with data");
    ui->label_2->setText("Control BMP | WAV");
    ui->label_4->setText("Unhide");
    ui->pushButton_2->setToolTip("Open BMP | WAV file with data");
    ui->pushButton_3->setToolTip("Open control BMP | WAV file");
    Status->setText(" ");
    ui->checkBox->setDisabled(true);
}
void MainWindow::Noise(){
    if(ui->checkBox->isChecked())
        this->engine->setNoise(true);
    else
        this->engine->setNoise(false);
}
int MainWindow::mainslot()
{
    bool fk = ui->plainTextEdit->isHidden();
    char *file;
    if(!fk){
        char name[11]= {0,0,0,0,0,0,0,0,0,0};
        name[10] = '\0';
        for(int i = 0; i< 10; i++)
            name[i]=(rand()%10)+48;
        QString *b = new QString(name);
        FILE* tmp = fopen(name,"w+");
        QString text = ui->plainTextEdit->toPlainText();
        fwrite(text.toLocal8Bit().data(),1,strlen(text.toLocal8Bit().data()),tmp);
        ui->lineEdit->setText(*b);
        fclose(tmp);
        file = name;
    }
    int IOerror;
    QString a = ui->lineEdit->text();
    if(!fk)
        this->ui->lineEdit->setText(" ");
    QString b = ui->lineEdit_2->text();
    QString c = ui->lineEdit_3->text();
    if(c.capacity() == 0) c = "result";
    if(a.capacity() == 0 || b.capacity() == 0){
        Status->setText("Empty argument(s)");
        return 1;
    }
    if(this->Operation == 1)
        IOerror = engine->hide(a.toLocal8Bit().data(),b.toLocal8Bit().data(),c.toLocal8Bit().data());
    else
        IOerror = engine->unhide(a.toLocal8Bit().data(),b.toLocal8Bit().data(),c.toLocal8Bit().data());
    if(!fk)
        remove(file);
    switch(IOerror){
    case 1 :{
            Status->setText("I/O Error");
            break;
        }
    case 2:{
            Status->setText("Invalid fyletype");
            break;
        }
    case 3:{
            Status->setText("File is too big");
            break;
        }
    case 4:{
            Status->setText("File is compressed");
            break;
        }
    case 5:{
            Status->setText("Unsupported BMP");
            break;
        }
    case 6:{
            Status->setText("Files don't match");
            break;
        }
    case 0: {
            Status->setText("Completed");
            break;
        }
    default:{
            Status->setText("Immposible situation");
            break;
        }
    }
    return 0;
}





