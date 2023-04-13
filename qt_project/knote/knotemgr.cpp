#include <QSettings>
#include <QDebug>
#include <QIntValidator>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFileInfoList>
#include <QPalette>
#include <QRegExp>
#include <QGraphicsEffect>
#include <QListWidget>
#include <QMessageBox>

#include "config/appsettings.h"

#include "knoteitem.h"
#include "knotemgr.h"
#include "ui_knotemgr.h"

KNoteMgr::KNoteMgr(QWidget *parent) :
    QDialog(parent), ui(new Ui::KNoteMgr),
    hasNoteChanged(false),
    mKBookIndex(-1), mKNoteIndex(-1),
    mKNoteTotal(0), mKNoteCountSelected(0)
{
    ui->setupUi(this);

    Qt::WindowFlags windowFlag = Qt::Dialog;
    windowFlag |= Qt::WindowMinimizeButtonHint;
    windowFlag |= Qt::WindowMaximizeButtonHint;
    windowFlag |= Qt::WindowCloseButtonHint;

    setWindowFlags(windowFlag);
    initUI();
    initData();
}

KNoteMgr::~KNoteMgr()
{
    delete ui;
}

void KNoteMgr::initUI()
{
    QString listViewStyleSheet("QListView::item { border-bottom: 1px solid gray; }");
    //QString listViewStyleSheet("QListView::item:selected { border: 1px solid #6a6ea9; }");
    mKBookModel = new QStandardItemModel(this);
    ui->listViewKBook->setModel(mKBookModel);
    ui->listViewKBook->setStyleSheet(listViewStyleSheet);

    mKNoteModel = new QStandardItemModel(this);
    ui->listViewKNote->setModel(mKNoteModel);
    ui->listViewKNote->setStyleSheet(listViewStyleSheet);

    ui->listViewKBook->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->listViewKBook, SIGNAL(clicked(QModelIndex)), this, SLOT(onKBookSelected(QModelIndex)));

    ui->listViewKNote->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->listViewKNote, SIGNAL(clicked(QModelIndex)), this, SLOT(onKNoteSelected(QModelIndex)));

    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
    effect->setOpacity(0);
    ui->btnHide->setGraphicsEffect(effect);
    connect(ui->btnImport, SIGNAL(clicked()), this, SLOT(onBtnImportClicked()));
    connect(ui->btnExport, SIGNAL(clicked()), this, SLOT(onBtnExportClicked()));
    connect(ui->btnDelBook, SIGNAL(clicked()), this, SLOT(onBtnDelBookClicked()));
    connect(ui->btnDelNote, SIGNAL(clicked()), this, SLOT(onBtnDelNoteClicked()));
    ui->labelStatus->setText(tr("就绪"));
}

void KNoteMgr::initData()
{
    QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
    mKNoteFileDir = s.value(AppSettings::KNOTE_SDIR).toString();
    mKNoteSaveDir = s.value(AppSettings::KNOTE_DDIR).toString();

#if 0
    QStandardItemModel *model = new QStandardItemModel(this);
    ui->listViewKBook->setModel(model);
    QStandardItem *item = new QStandardItem("无法悲伤");
    model->appendRow(item);
    item = new QStandardItem("红楼梦");
    model->appendRow(item);
    item = new QStandardItem("恰同学少年");
    model->appendRow(item);
    item = new QStandardItem("爱你就像爱生命");
    model->appendRow(item);
    item = new QStandardItem("郁达夫小说全集");
    model->appendRow(item);
#endif
}

/** note format:
line 1: 淮南子 (（西汉）刘安)
line 2: - 您在位置 #92-93的标注 | 添加于 2022年9月29日星期四 上午9:00:10

line 4: 所谓无为者，不先物为也；所谓无不为者，因物之所为。
line 5: ==========
*/
bool KNoteMgr::isKNoteFile(QString filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->labelStatus->setText("笔记打开失败~");
        return false;
    }

    if (file.size() == 0) {
        file.close();
        ui->labelStatus->setText("笔记空~");
        return false;
    }

    QTextStream in(&file);
    int lineCnt = 0;
    while (!in.atEnd()) {
        lineCnt += 1;
        QString line = in.readLine();  // line 1

        lineCnt += 1;
        line = in.readLine();  // line 2
        if (!line.startsWith("-") || !line.contains("|")) {
            goto err_exit;
        }

        lineCnt += 1;
        line = in.readLine();  // line 3
        if (!line.isEmpty()) {
            goto err_exit;
        }

        lineCnt += 1;
        line = in.readLine();  // line 4

        lineCnt += 1;
        line = in.readLine();  // line 5
        if (!line.startsWith("==========")) {
            goto err_exit;
        }

        mKNoteTotal += 1;
    }

    file.close();
    return true;

err_exit:
    QString errMsg("笔记格式错误，行号：");
    errMsg.append(QString::number(lineCnt));
    ui->labelStatus->setText(errMsg);
    file.close();
    return false;
}

int KNoteMgr::parseKNote(QString filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return -1;
    }

    if (file.size() == 0) {
        file.close();
        return 0;
    }

    QTextStream in(&file);
    int itemId = 0;
    while (!in.atEnd()) {
        KNoteItem item;
        itemId += 1;
        item.setId(itemId);

        QString line = in.readLine();  // line 1
        item.setBookName(line);

        line = in.readLine();  // line 2
        item.setDateTime(line);

        line = in.readLine();  // line 3
        line = in.readLine();  // line 4
        item.setContent(line);

        line = in.readLine();  // line 5
        if (!line.startsWith("==========")) {
            file.close();
            return -1;
        }

        mKNoteItems.append(item);
    }
    qDebug() << mKNoteItems.size();

    file.close();
    return 0;
}

int KNoteMgr::showKBook()
{
    for (int i = 0; i < mKNoteItems.size(); i ++) {
        KNoteItem item = mKNoteItems.at(i);
        //qDebug() << item.getContent();
        showKBookName(item.getBookName());
    }

    return 0;
}

/* format: 史记 (司马迁) */
int KNoteMgr::showKBookName(QString s)
{
    int pos = s.lastIndexOf("(");
    pos -= 1;
    QString bookName = s.left(pos);
    if (!mKBooks.contains(bookName)) {
        //qDebug() << "bookName: " << bookName;
        mKBooks.append(bookName);

        QStandardItem *item = new QStandardItem(bookName);
        mKBookModel->appendRow(item);
    }
    return 0;
}

int KNoteMgr::saveKNote(SaveNoteModeFlag flag)
{
    QString saveFile = QFileDialog::getSaveFileName(this, tr("Open file"), mKNoteSaveDir);
    qDebug() << saveFile;

    if (saveFile.isEmpty()) {
        ui->labelStatus->setText(tr("未设置导出目录！"));
        return -1;
    }

    if ((0 == mKNoteItems.size() && MODE_ALL == flag) ||
        (0 == mKNoteSelected.size() && MODE_SELECTED == flag)) {
        ui->labelStatus->setText(tr("笔记为空！"));
        return -1;
    }

    QFile file(saveFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        return -1;
    }

    QTextStream out(&file);
    /* format: utf-8 with BOM */
    out.setGenerateByteOrderMark(true);
    out.setCodec("utf-8");

    QString newLine("\n");
    int size = 0;
    if (MODE_ALL == flag) {
        size = mKNoteItems.size();
    } else if (MODE_SELECTED == flag) {
        size = mKNoteSelected.size();
    } else {
        file.close();
        return -1;
    }

    for (int i = 0; i < size; i ++) {
        if (MODE_ALL == flag) {
            KNoteItem item = mKNoteItems.at(i);
            out << item.getBookName();
            out << newLine;

            out << item.getDateTime();
            out << newLine;

            out << "";
            out << newLine;

            out << item.getContent();
            out << newLine;

            out << "==========";
            out << newLine;
        } else if (MODE_SELECTED == flag) {
            KNoteItem item = mKNoteSelected.at(i);
            out << item.getContent();
            out << newLine;
            out << "\n";
        } else {
            file.close();
            return -1;
        }
    }

    QFileInfo fileInfo(file);
    mKNoteSaveDir = QDir::toNativeSeparators(fileInfo.absolutePath());
    QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
    s.setValue(AppSettings::KNOTE_SDIR, mKNoteSaveDir);

    file.flush();
    file.close();
    return 0;
}

void KNoteMgr::saveKNoteAll()
{
    saveKNote(MODE_ALL);
}

void KNoteMgr::saveKNoteSelect()
{
    saveKNote(MODE_SELECTED);
}

int KNoteMgr::AddKBookNote(QString dateTime, QString content)
{
    /* dateTime format: - 您在位置 #3445-3446的标注 | 添加于 2023年4月12日星期三 上午10:06:36 */
    int pos = dateTime.indexOf("|");
    pos += 2;
    QString tmp = dateTime.right(dateTime.size() - pos);
    //qDebug() << tmp;

    QString note(content);
    note.append("\r\n");
    note.append(tmp);

    QStandardItem *item = new QStandardItem(note);
    mKNoteModel->appendRow(item);
    return 0;
}

void KNoteMgr::updateLabelNote()
{
    QString msg("总条数：");
    msg.append(QString::number(mKNoteTotal));
    if (mKNoteCountSelected > 0) {
        msg.append("，选中笔记条数：");
        msg.append(QString::number(mKNoteCountSelected));
    }
    ui->labelNote->setText(msg);
}

void KNoteMgr::delNoteOne(int index)
{
    for (int i = 0; i < mKNoteItems.size(); i ++) {
        KNoteItem tmp = mKNoteItems.at(i);
        if (tmp.getId() == index) {
            mKNoteItems.removeAt(i);
            hasNoteChanged = true;
            break;
        }
    }
}

void KNoteMgr::delNoteBook()
{
    for (int i = 0; i < mKNoteSelected.size(); i ++) {
        KNoteItem item = mKNoteSelected.at(i);
        //qDebug() << "del item: " << item.getId();
        delNoteOne(item.getId());
    }

    mKNoteCountSelected = 0;
    mKNoteSelected.clear();
    mKNoteModel->clear();
    mKNoteTotal = mKNoteItems.size();
    updateLabelNote();
}

void KNoteMgr::statusSetKBookCurrent()
{
    ui->labelStatus->setText(mKBookSelected);
}

void KNoteMgr::onKBookSelected(QModelIndex index)
{
    mKBookIndex = index.row();
    qDebug() << "mKBookIndex: " << mKBookIndex;
    mKBookSelected.clear();
    mKBookSelected.append("当前书籍：《");
    QString bookName = index.data().toString();
    mKBookSelected.append(bookName);
    mKBookSelected.append("》");
    statusSetKBookCurrent();

    /* show note */
    mKNoteModel->clear();
    mKNoteSelected.clear();
    for (int i = 0; i < mKNoteItems.size(); i ++) {
        KNoteItem item = mKNoteItems.at(i);
        QString tmp = item.getBookName();

        if (tmp.contains(bookName)) {
            AddKBookNote(item.getDateTime(), item.getContent());
            mKNoteSelected.append(item);
        }
    }
    mKNoteCountSelected = mKNoteSelected.size();
    updateLabelNote();
}

void KNoteMgr::onKNoteSelected(QModelIndex index)
{
    mKNoteIndex = index.row();
}

void KNoteMgr::closeEvent(QCloseEvent *event)
{
    if (hasNoteChanged) {
        QMessageBox msgBox;
        msgBox.setInformativeText(tr("笔记有改动，保存？"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Discard);
        int res = msgBox.exec();
        switch (res) {
        case QMessageBox::Yes:
            saveKNoteAll();
            break;
        default:
            break;
        }
    }
    event->accept();
}

void KNoteMgr::onBtnImportClicked()
{
    /* load file from mKNoteFileDir */
    QString noteFile = QFileDialog::getOpenFileName(this, tr("Open file"), mKNoteFileDir);

    qDebug() << noteFile;
    if (noteFile.isEmpty()) {
        ui->labelStatus->setText(tr("未导入笔记！"));
    } else {
        if (isKNoteFile(noteFile)) {
            QFileInfo fileInfo(noteFile);
            mKNoteFileDir = QDir::toNativeSeparators(fileInfo.absolutePath());
            QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
            s.setValue(AppSettings::KNOTE_SDIR, mKNoteFileDir);
            ui->labelStatus->setText(tr("笔记解析中..."));
            int result = parseKNote(noteFile);
            if (!result) {
                QString msg("笔记解析成功，总条数：");
                msg.append(QString::number(mKNoteTotal));
                ui->labelStatus->setText(msg);
                showKBook();
                updateLabelNote();
            } else {
                ui->labelStatus->setText(tr("笔记解析失败！"));
            }
        } else {
            mKNoteFileDir.clear();
            QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
            s.setValue(AppSettings::KNOTE_SDIR, mKNoteFileDir);
        }
    }
}

void KNoteMgr::onBtnExportClicked()
{
    saveKNoteSelect();
}

void KNoteMgr::onBtnDelBookClicked()
{
    if (mKBookIndex != -1) {
        mKBooks.removeAt(mKBookIndex);
        mKBookIndex = -1;
        //QModelIndex index = mKBookModel->index(mKBookIndex, 0);
        //QString bookName = index.data().toString();
        //qDebug() << "del book: " << bookName;
        mKBookModel->clear();
        for (int i = 0; i < mKBooks.size(); i ++) {
            QStandardItem *item = new QStandardItem(mKBooks.at(i));
            mKBookModel->appendRow(item);
        }
        delNoteBook();

    }
}

void KNoteMgr::onBtnDelNoteClicked()
{
    if (mKNoteIndex != -1) {
        KNoteItem item = mKNoteSelected.at(mKNoteIndex);
        mKNoteSelected.removeAt(mKNoteIndex);
        mKNoteIndex = -1;

        mKNoteModel->clear();
        for (int i = 0; i < mKNoteSelected.size(); i ++) {
            KNoteItem tmp = mKNoteSelected.at(i);
            AddKBookNote(tmp.getDateTime(), tmp.getContent());
        }

        delNoteOne(item.getId());
        mKNoteTotal = mKNoteItems.size();
        mKNoteCountSelected = mKNoteSelected.size();
        updateLabelNote();
    }
}
