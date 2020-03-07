#include "crossword.h"
#include "dialogvved.h"
#include "ui_crossword.h"
#include "help_browser.h"
#include "dictionary.h"
#include <QFile>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QWebEngineView>
#include <QByteArray>
#include <cstdlib>
#include <QTableWidgetItem>
#include <QString>
#include <QPrinter>
#include <QTextDocument>
#include <QTextCodec>
#include <QResource>
#include <QItemDelegate>
#include <ctime>

static int amountWords = 0,amountWords2=0,counterWordWrite=2;
static bool first=true,isStep=false, method = false;

Crossword::Crossword(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Crossword)
{
    ui->setupUi(this);
    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(cellDelegate(int,int)));
}

Crossword::~Crossword()
{
    delete ui;
}

void Crossword::on_pushButton_clicked()
{
    DialogVved *sl = new DialogVved(this);
    sl->setWindowFlags(sl->windowFlags() & ~Qt::WindowContextHelpButtonHint|Qt::MSWindowsFixedSizeDialogHint);
    sl->show();
    connect(sl,SIGNAL(word(QString, QString,bool)),this,SLOT(addWord(QString, QString,bool)));
    if(!method){
        amountWords=0;
        amountWords2=0;
        strList.clear();
        strList2.clear();
        writeStrText.clear();
        writeStrText2.clear();
        questionList.clear();
        ui->textEdit->clear();
        ui->textEdit_2->clear();
        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(0);
        ui->tableWidget->setColumnCount(0);
    }
    method = true;
}

void Crossword::addWord(QString str, QString str1, bool checked)
{
    writeStrText.append(str1);
    writeStrText2.append(str1);
    amountWords++;
    amountWords2++;
    strList.append(str);
    strList2.append(str);
    if(checked){
        QFile *file = new QFile;
        file->setFileName(QApplication::applicationDirPath() + "/list.txt");
        if(!file->open(QIODevice::ReadOnly|QIODevice::Text))
        {
            qCritical("Error %d: %s",file->error(),qPrintable(file->errorString()));
        }
        else{
            bool okWrite = true;
            while(!file->atEnd()){
                QString writedStr;
                writedStr = file->readLine();
                writedStr.remove(str.length(),writedStr.length()-str.length());
                if(str == writedStr.toLower()){
                    okWrite = false;
                }
            }
            file->close();
            if(!file->open(QIODevice::WriteOnly|QIODevice::Append))
            {
                qCritical("Error %d: %s",file->error(),qPrintable(file->errorString()));
            }
            else{
                if(okWrite){
                    QString strWr = str + "-" + str1 + "\n";
                    QByteArray ArrayResul1251;
                    ArrayResul1251.insert(0,strWr);
                    QTextCodec *Codec1251=QTextCodec::codecForName("Windows-1251"); //ob]9vlenie ob]ekta QTextCodec, konvertiruushego from chcp1251 to unicode
                    QString ResultUnicode=Codec1251->toUnicode(ArrayResul1251); //convert to unicode
                    QTextCodec *CodecUtf8=QTextCodec::codecForName("UTF-8"); //ob]9vlenie ob]ekta QTextCodec, konvertiruushego from unicode to utf-8
                    QString ResultUtf=CodecUtf8->fromUnicode(ResultUnicode);
                    file->write(qPrintable(ResultUtf));
                    file->close();
                }
            }

        }
    }
}

void Crossword::on_pushButton_5_clicked()
{
    amountWords=0;
    amountWords2=0;
    strList.clear();
    strList2.clear();
    writeStrText.clear();
    writeStrText2.clear();
    questionList.clear();
    ui->textEdit->clear();
    ui->textEdit_2->clear();
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
}

void Crossword::on_pushButton_2_clicked()
{
    strList = strList2;
    writeStrText = writeStrText2;
    questionList.clear();
    ui->textEdit->clear();
    ui->textEdit_2->clear();
    for(int i = 0;i<strList.length();i++){
        ui->textEdit->append(strList[i] + "\n");
    }
    amountWords=amountWords2;
    counterWordWrite=2;
    first = true;
    if(ui->radioButton_2->isChecked()){
        isStep = false;
    }
    else if (ui->radioButton->isChecked()) {
        isStep = true;
    }
    writeStr.clear();
    if(amountWords!=0){
        int selectIndex=0;
        bool write = false, checkAmount = false;
        QStringList writeIndex[2];
        QStringList writePos;
        QStringList notWrite;
        QStringList notWriteText;
        ui->tableWidget->clear();
        QString startArray[300][300];
        for (int i = 0;i<300;i++) {
            for (int j = 0;j<300;j++) {
                startArray[i][j] = "";
            }
        }

        while(amountWords>0){
                if(first){
                    selectIndex = rand()%amountWords;
                    if((strList[selectIndex].length()/2)!=150){
                        for (int i = 0;i<strList[selectIndex].length();i++) {
                            startArray[300/2-1][150-(strList[selectIndex].length()/2)+i] = strList[selectIndex].at(i);
                        }
                        startArray[300/2-1][149-(strList[selectIndex].length()/2)] = "1)";
                        writeIndex[0].append(QString::number(300/2-1));
                        writeIndex[1].append( QString::number(150-(strList[selectIndex].length()/2)));
                        writeStr.append(strList[selectIndex]);
                        questionList.append(writeStrText[selectIndex]);
                        writeStrText.removeAt(selectIndex);
                        writePos.append("h");
                        strList.removeAt(selectIndex);
                        amountWords--;
                        first = false;
                        selectIndex=-1;
                    }
                }
                else {
                    int amountNotFit=0,countList = strList.length();
                    int selIndWrite = rand()%writeStr.length();
                    if(selIndWrite==1 && writeStr.length() == 1){
                        selIndWrite=0;
                    }
                    if(amountWords>1){
                        selectIndex = rand()%amountWords;
                    }
                    else if(amountWords==1){
                        if(writeStr.length()==0){
                            amountWords=0;
                            break;
                        }
                        selectIndex = 0;
                    }
                    for (;selIndWrite<writeStr.length();selIndWrite++) {
                        for (int ia = 0;ia<strList[selectIndex].length();ia++) {
                            for (int ja = 0;ja<writeStr[selIndWrite].length();ja++) {
                                if(strList[selectIndex].at(ia) == writeStr[selIndWrite].at(ja)){
                                    int row = 300 - writeIndex[0][selIndWrite].toInt() - 1;
                                    int indt = strList[selectIndex].length() - ia - 1;
                                    int rowY = 300 - writeIndex[1][selIndWrite].toInt() - 1;
                                    int indtY = strList[selectIndex].length() - ia - 1;
                                        if(writePos[selIndWrite] == "h" && strList[selectIndex].length()<300 && ia<writeIndex[0][selIndWrite].toInt() && indt<=row ){
                                        int indexY = writeIndex[1][selIndWrite].toInt()+ja;
                                        int indexX = writeIndex[0][selIndWrite].toInt();
                                        if(indexX>=ia){
                                            bool okWrite = true;
                                            int localeIndexInWord=0;
                                            for (int firstIndex = indexX - ia;firstIndex<indexX+(strList[selectIndex].length()-ia);firstIndex++) {
                                                if(firstIndex!=indexX){
                                                    if(firstIndex == indexX - ia){
                                                        if(!startArray[firstIndex-1][indexY].isEmpty()){
                                                            okWrite = false;
                                                        }
                                                        if(!startArray[firstIndex][indexY-1].isEmpty() || !startArray[firstIndex][indexY+1].isEmpty()){
                                                            if(startArray[firstIndex][indexY] != strList[selectIndex].at(localeIndexInWord)){
                                                                okWrite = false;
                                                            }
                                                        }
                                                    }
                                                    else if (firstIndex == indexX+(strList[selectIndex].length()-ia)-1) {
                                                        if(!startArray[firstIndex+1][indexY].isEmpty()){
                                                            okWrite = false;
                                                        }
                                                        if(!startArray[firstIndex][indexY-1].isEmpty() || !startArray[firstIndex][indexY+1].isEmpty()){
                                                            if(startArray[firstIndex][indexY] != strList[selectIndex].at(localeIndexInWord)){
                                                                okWrite = false;
                                                            }
                                                        }
                                                    }
                                                    else {
                                                        if(!startArray[firstIndex][indexY-1].isEmpty() || !startArray[firstIndex][indexY+1].isEmpty()){
                                                            if(startArray[firstIndex][indexY] != strList[selectIndex].at(localeIndexInWord)){
                                                                okWrite = false;
                                                            }
                                                        }
                                                    }
                                                }
                                                else{
                                                    if(firstIndex == indexX - ia && ia==0){
                                                        if(!startArray[firstIndex-1][indexY].isEmpty()){
                                                            okWrite = false;
                                                        }
                                                    }
                                                }
                                                if(ia == 0 && ja == 0){
                                                    if(!startArray[indexX-1][indexY].isEmpty())
                                                        okWrite = false;
                                                }
                                                if(ia == strList[selectIndex].length()-1 && ja == 0){
                                                    if(!startArray[indexX+1][indexY].isEmpty())
                                                        okWrite = false;
                                                }
                                                if(ia == 0 && ja == writeStr[selIndWrite].length()-1){
                                                    if(!startArray[indexX-1][indexY].isEmpty())
                                                        okWrite = false;
                                                }
                                                if(ia == strList[selectIndex].length()-1 && ja == writeStr[selIndWrite].length()-1){
                                                    if(!startArray[indexX+1][indexY].isEmpty())
                                                        okWrite = false;
                                                }
                                                if(!startArray[firstIndex][indexY].isEmpty()){
                                                    if( startArray[firstIndex][indexY] != strList[selectIndex].at(localeIndexInWord)){
                                                        okWrite = false;
                                                    }
                                                }
                                                localeIndexInWord++;
                                            }
                                            if(okWrite){
                                                int localeIndexInWord=0;
                                                for (int firstIndex = indexX - ia;firstIndex<indexX+(strList[selectIndex].length()-ia);firstIndex++) {
                                                    startArray[firstIndex][indexY] = strList[selectIndex].at(localeIndexInWord++);
                                                }
                                                startArray[indexX - ia - 1][indexY] =QString::number(counterWordWrite++) + ")";
                                                writeIndex[0].append(QString::number(indexX-ia));
                                                writeIndex[1].append(QString::number(indexY));
                                                writeStr.append(strList[selectIndex]);
                                                questionList.append(writeStrText[selectIndex]);
                                                writeStrText.removeAt(selectIndex);
                                                writePos.append("v");
                                                strList.removeAt(selectIndex);
                                                amountWords--;
                                                write = true;

                                            }
                                            else{
                                                if(selIndWrite+1<writeStr.length()){
                                                    selIndWrite++;
                                                }
                                                else {
                                                    if(writeStr.length()!=0){selIndWrite=0;}
                                                }
                                                amountNotFit++;
                                                ja=0;
                                                ia=0;
                                                if(amountNotFit==writeStr.length()){
                                                    notWrite.append(strList[selectIndex]);
                                                    notWriteText.append(writeStrText[selectIndex]);
                                                    strList.removeAt(selectIndex);
                                                    writeStrText.removeAt(selectIndex);
                                                    amountWords--;
                                                    write = true;
                                                    amountNotFit=0;
                                                }
                                            }
                                        }
                                    }
                                    else if(writePos[selIndWrite] == "v" && strList[selectIndex].length()<300 && ia<writeIndex[1][selIndWrite].toInt() && indtY<=rowY){
                                        int indexY = writeIndex[1][selIndWrite].toInt();
                                        int indexX = writeIndex[0][selIndWrite].toInt() + ja;
                                        if(indexY>=ia){
                                            bool okWrite = true;
                                            int localeIndexInWord=0;
                                            for (int firstIndex = indexY - ia;firstIndex<indexY+(strList[selectIndex].length()-ia);firstIndex++) {
                                                if(firstIndex!=indexY){
                                                    if(firstIndex == indexY - ia){
                                                        if(!startArray[indexX][firstIndex-1].isEmpty()){
                                                            okWrite = false;
                                                        }
                                                        if(!startArray[indexX-1][firstIndex].isEmpty() || !startArray[indexX+1][firstIndex].isEmpty()){
                                                            if(startArray[indexX][firstIndex] != strList[selectIndex].at(localeIndexInWord)){
                                                                okWrite = false;
                                                            }
                                                        }
                                                   }
                                                    else if (firstIndex == indexY+(strList[selectIndex].length()-ia)-1) {
                                                        if(!startArray[indexX][firstIndex+1].isEmpty()){
                                                            okWrite = false;
                                                        }
                                                        if(!startArray[indexX-1][firstIndex].isEmpty() || !startArray[indexX+1][firstIndex].isEmpty()){
                                                            if(startArray[indexX][firstIndex] != strList[selectIndex].at(localeIndexInWord)){
                                                                okWrite = false;
                                                            }
                                                        }
                                                    }
                                                    else {
                                                        if(!startArray[indexX-1][firstIndex].isEmpty() || !startArray[indexX+1][firstIndex].isEmpty()){
                                                            if(startArray[indexX][firstIndex] != strList[selectIndex].at(localeIndexInWord)){
                                                                okWrite = false;
                                                            }
                                                        }
                                                    }
                                                }
                                                else{
                                                    if(firstIndex == indexY - ia && ia==0){
                                                        if(!startArray[indexX][firstIndex-1].isEmpty()){
                                                            okWrite = false;
                                                        }
                                                   }
                                                }
                                                if(ia == 0 && ja == 0){
                                                    if(!startArray[indexX][indexY-1].isEmpty())
                                                        okWrite = false;
                                                }
                                                if(ia == strList[selectIndex].length()-1 && ja == 0){
                                                    if(!startArray[indexX][indexY+1].isEmpty())
                                                        okWrite = false;
                                                }
                                                if(ia == 0 && ja == writeStr[selIndWrite].length()-1){
                                                    if(!startArray[indexX][indexY-1].isEmpty())
                                                        okWrite = false;
                                                }
                                                if(ia == strList[selectIndex].length()-1 && ja == writeStr[selIndWrite].length()-1){
                                                    if(!startArray[indexX][indexY+1].isEmpty())
                                                        okWrite = false;
                                                }
                                                if(!startArray[indexX][firstIndex].isEmpty()){
                                                    if( startArray[indexX][firstIndex] != strList[selectIndex].at(localeIndexInWord)){
                                                        okWrite = false;
                                                    }
                                                }
                                                localeIndexInWord++;
                                            }
                                            if(okWrite){
                                                int localeIndexInWord=0;
                                                for (int firstIndex = indexY - ia;firstIndex<indexY+(strList[selectIndex].length()-ia);firstIndex++) {
                                                    startArray[indexX][firstIndex] = strList[selectIndex].at(localeIndexInWord++);
                                                }
                                                startArray[indexX][indexY - ia - 1] = QString::number(counterWordWrite++) + ")";
                                                writeIndex[0].append(QString::number(indexX));
                                                writeIndex[1].append(QString::number(indexY-ia));
                                                writeStr.append(strList[selectIndex]);
                                                questionList.append(writeStrText[selectIndex]);
                                                writeStrText.removeAt(selectIndex);
                                                writePos.append("h");
                                                strList.removeAt(selectIndex);
                                                amountWords--;
                                                write = true;

                                            }
                                            else{
                                                if(selIndWrite+1<writeStr.length()){
                                                    selIndWrite++;
                                                }
                                                else {
                                                    if(writeStr.length()!=0){selIndWrite=0;}
                                                }
                                                amountNotFit++;
                                                ja=0;
                                                ia=0;
                                                if(amountNotFit==writeStr.length()){
                                                    notWriteText.append(writeStrText[selectIndex]);
                                                    notWrite.append(strList[selectIndex]);
                                                    strList.removeAt(selectIndex);
                                                    writeStrText.removeAt(selectIndex);
                                                    amountWords--;
                                                    write = true;
                                                    amountNotFit=0;
                                                }
                                               }
                                            }
                                        }
                                    }
                                    if(write){
                                        break;
                                    }
                                }
                                if(write){
                                    break;
                                }
                            }
                            if(write){
                                write = false;
                                break;
                            }
                        }
                    if(amountWords>2){
                        if(selIndWrite == writeStr.length() && strList.length() == countList){
                            notWrite.append(strList[selectIndex]);
                            strList.removeAt(selectIndex);
                            notWriteText.append(writeStrText[selectIndex]);
                            writeStrText.removeAt(selectIndex);
                            amountWords--;
                        }
                    }
                    else if(amountWords==2){
                        if(selIndWrite==writeStr.length() && !strList[1].isNull()){
                            notWrite.append(strList[selectIndex]);
                            strList.removeAt(selectIndex);
                            notWriteText.append(writeStrText[selectIndex]);
                            writeStrText.removeAt(selectIndex);
                            amountWords--;
                        }
                    }
                    else if(amountWords==1){
                        if(selIndWrite==writeStr.length() && !strList.isEmpty()){
                            notWrite.append(strList[0]);
                            strList.removeAt(0);
                            notWriteText.append(writeStrText[0]);
                            writeStrText.removeAt(0);
                            amountWords=0;
                        }
                    }

                }
                if(amountWords==0 && !checkAmount){
                    amountWords=notWrite.length();
                    strList = notWrite;
                    writeStrText = notWriteText;
                    notWriteText.clear();
                    notWrite.clear();
                    checkAmount = true;
                }
        }
            if(writeStr.length()>1){
                int minY = 302,maxY = -1,minX = 302,maxX = -1;
                for(int q = 0;q<300;q++){
                    for(int q1 = 0;q1<300;q1++){
                        if(startArray[q][q1] != "" && q<minY){
                            minY = q;
                        }
                        if(startArray[q][q1] != "" && q1<minX){
                            minX = q1;
                        }
                        if(startArray[q][q1] != "" && q>maxY){
                            maxY = q;
                        }
                        if(startArray[q][q1] != "" && q1>maxX){
                            maxX = q1;
                        }
                    }
                }
                ui->tableWidget->setRowCount(maxY - minY + 1);
                ui->tableWidget->setColumnCount(maxX - minX + 1);
                QDesktopWidget *desktop = QApplication::desktop();
                int pix_x = desktop->width();
                int pix_y = desktop->height();
                if((ui->tableWidget->columnCount()*30+150)>pix_x ||  (ui->tableWidget->rowCount()*30 + 180) > pix_y || this->isMaximized()){
                    this->showMaximized();
                }
                else{
                    this->setGeometry(x(),y(),ui->tableWidget->columnCount()*30+350,ui->tableWidget->rowCount()*30 + 180);
                }
                for(int t = 0 ; t<writeStr.length();t++){
                    int indx = writeIndex[0][t].toInt(),indy = writeIndex[1][t].toInt();
                    for(int y = -1; y<writeStr[t].length();y++){
                        QTableWidgetItem *item = new QTableWidgetItem;
                        item->setTextAlignment(Qt::AlignCenter);
                        if(writePos[t] == "h"){
                            item->setText(startArray[indx][indy + y]);
                            ui->tableWidget->setItem(indx - minY,indy - minX + y,item);
                        }
                        else{
                            item->setText(startArray[indx + y][indy]);
                            ui->tableWidget->setItem(indx - minY + y,indy - minX,item);
                        }
                    }
                    ui->tableWidget->setItemDelegate(new MyDelegate(this));
                    if(isStep && t!=writeStr.length()-1){
                        QMessageBox::information(this,"Пауза","Натисніть щоб продовжити");
                    }
                }
                if(!notWrite.isEmpty()){
                    QString str;
                    for(int r = 0;r<notWrite.length();r++) {
                        ui->textEdit_2->append(notWrite[r] + "\n");
                    }
                }
            }
            else {
                ui->tableWidget->clear();
                ui->tableWidget->setRowCount(0);
                ui->tableWidget->setColumnCount(0);
                QMessageBox::information(this,"Error","Побудувати кросворд не вдалося!!!");
            }
    }
}


void Crossword::on_pushButton_4_clicked()
{
    if(ui->tableWidget->columnCount()!=0 && ui->tableWidget->rowCount()!=0){
        QString text("<html><head></head><body><table style='border-collapse: collapse;font-size:20px;'><tbody>");
        for (int i = 0; i <  ui->tableWidget->rowCount(); i++) {
            text.append("<tr>");
            for (int j = 0; j <  ui->tableWidget->columnCount(); j++) {
                if(ui->tableWidget->item(i,j)){
                    QString str;
                    str = ui->tableWidget->item(i,j)->text();
                    if(str.length() == 1){
                        text.append("<td style='border-width:1px; border-color: black; border-style: solid; width:30px; height :30px'>").append("   ").append("</td>");
                    }
                    else{
                        text.append("<td style='width:30px; height :30px'>").append(str).append("</td>");
                    }
                }
                else{
                    text.append("<td style='width:30px; height :30px'>").append("</td>");
                }
            }
            text.append("</tr>");
        }
        text.append("</tbody></table></body></html>");
        text.append("<p>Запитання:</p><p>");
        for(int i = 0;i<questionList.length();i++){
            text.append(QString::number(i+1) + ") " + questionList[i] + "; ");
        }
        text.append("</p>");
        text.append("<p style=\"transform: rotate(180deg);\">Відповіді: ");
        for(int i = 0;i<writeStr.length();i++){
            text.append(QString::number(i+1) + ") " + writeStr[i] + "; ");
        }
        text.append("</p>");
        QFile *file = new QFile;
        file->setFileName(QApplication::applicationDirPath() + "/webPage.html");
        if(!file->open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Truncate))
        {
            qCritical("Error %d: %s",file->error(),qPrintable(file->errorString()));
        }
        else{
            file->write(qPrintable(text));
            file->close();
            page = new QWebEngineView(this);
            connect(page,SIGNAL(loadFinished(bool)),this,SLOT(fromPdf(bool)));
            page->load(QUrl::fromLocalFile(file->fileName()));
        }
    }
}


void MyDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QItemDelegate::paint( painter, option, index);
    QString value = index.model()->data(index).toString();
    if(value.length() == 1){
       painter->setPen( Qt::black);
       painter->drawRect( option.rect );
    }
}

void Crossword::on_pushButton_6_clicked()
{
    Dictionary *dict = new Dictionary;
    dict->setWindowFlags(dict->windowFlags() & ~Qt::WindowContextHelpButtonHint|Qt::MSWindowsFixedSizeDialogHint);
    dict->setWindowModality(Qt::WindowModality());
    connect(dict,SIGNAL(wordCount(int)),this,SLOT(setWordCount(int)));
    method = false;
    dict->exec();
}

void Crossword::setWordCount(int count)
{
    amountWords=0;
    amountWords2=0;
    strList.clear();
    strList2.clear();
    ui->textEdit->clear();
    ui->textEdit_2->clear();
    writeStrText.clear();
    questionList.clear();
    writeStrText2.clear();
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    QFile *file = new QFile;
    file->setFileName(QApplication::applicationDirPath() + "/list.txt");
    if(!file->open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qCritical("Error %d: %s",file->error(),qPrintable(file->errorString()));
    }
    else {
        int count1=0;
        QString str,strwrite;
        QStringList countList;
        do{
            str=file->readLine();
            if(!str.isNull()){
                count1++;
            }
        }while(!str.isNull());
        file->close();
        for (int i = 0;i<count;i++) {
            int random = 0;
            while(random==0){
                random = rand()%count1;
            }
            bool ok = true;
            for (int amountWords = 0;amountWords<countList.length();amountWords++) {
                if(random == countList[amountWords].toInt()){
                    ok = false;
                    i-=1;
                }
            }
            if(ok){
                countList.append(QString::number(random));
                file->open(QIODevice::ReadOnly|QIODevice::Text);
                while(random!=0){
                    str = file->readLine();
                    random--;
                }
                for(int j = 0;j<str.length();j++){
                    if(str.at(j)!="-"){
                        strwrite+=str.at(j).toLower();

                    }
                    else{
                        str.remove(0,j+1);
                        break;
                    }
                }
                file->close();
                writeStrText.append(str);
                writeStrText2.append(str);
                amountWords++;
                amountWords2++;
                strList.append(strwrite);
                strList2.append(strwrite);
                strwrite = "";
            }
        }
        file->close();
    }
}

void Crossword::fromPdf(bool selectIndex)
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Landscape);
    printer.setOutputFileName(QApplication::applicationDirPath() + "/NewCrosword.pdf");
    page->page()->printToPdf(printer.outputFileName(),printer.pageLayout());
}

void Crossword::cellDelegate(int row, int column)
{
    ui->tableWidget->setItemDelegate(new MyDelegate(this));
}

void Crossword::on_pushButton_3_clicked()
{
    help_browser *help = new help_browser(this);
    help->setWindowFlags(help->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    help->exec();
}
