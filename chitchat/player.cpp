#include "player.h"
#include "ui_player.h"
/**
 * @brief Player::Player
 * @param parent
 */
Player::Player(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Player)
{
    ui->setupUi(this);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    setWindowTitle(tr("音乐播发器"));
    setWindowIcon(QIcon(":/images/Music.png"));

    /**
     *设置背景图片
     * @brief pal
     */
    QPalette pal;
    QPixmap pixmap(":/images/background.jpg");
    pal.setBrush(QPalette::Window,QBrush(pixmap));
    setPalette(pal);

    // 设置行列数目
   ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(3);
    // 设置表头标签
    QStringList list;
    list << tr("音乐") << tr("艺术家") << tr("长度");
    ui->tableWidget->setHorizontalHeaderLabels(list);
    // 设置只能选择单行
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 设置不显示网格
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->setPalette(Qt::blue);

    playerInit();

    setFixedSize(700,430);
}

void Player::playerInit(){

    /**
     *创建媒体
     */
    music = new Phonon::MediaObject(this);
    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::createPath(music, audioOutput);
    connect(music,SIGNAL(tick(qint64)),this,SLOT(updateTime(qint64)));//更新进度

    volumeSlider=new Phonon::VolumeSlider(audioOutput,this);//音量调节
    seekSlider=new Phonon::SeekSlider(music,this);
    seekSlider->hide();


    ui->timtLabel->setText(tr("00:00/00:00"));

    play=new QAction(QIcon(":/images/playerIcon/play.png"),tr("播放"),this);
    noPlay=new QAction(QIcon(":/images/playerIcon/noplay.png"),tr("不播放"),this);
    skipForWard=new QAction(QIcon(":/images/playerIcon/skipForward.png"),tr("前一首"),this);
    skipBackWard=new QAction(QIcon(":/images/playerIcon/skipBackward.png"),tr("后一首"),this);
    open=new QAction(QIcon(":/images/Music.png"),tr("打开音乐"),this);
    lrc=new QAction(QIcon(":/images/playerIcon/lrc.png"),tr("显示歌词"),this);

    connect(open,SIGNAL(triggered()),this,SLOT(Open()));
    connect(play,SIGNAL(triggered()),this,SLOT(Play()));
    connect(noPlay,SIGNAL(triggered()),this,SLOT(Noplay()));
    connect(skipForWard,SIGNAL(triggered()),this,SLOT(SkipForWard()));
    connect(skipBackWard,SIGNAL(triggered()),this,SLOT(SkipBackWard()));
    connect(lrc,SIGNAL(triggered()),this,SLOT(showLRC()));

    toolBar=addToolBar(tr("工具栏"));
    toolBar->addAction(skipBackWard);
    toolBar->addAction(play);
    toolBar->addAction(noPlay);
    toolBar->addAction(skipForWard);
    toolBar->addAction(open);
    toolBar->addAction(lrc);
    toolBar->addWidget(volumeSlider);
    //music->setCurrentSource(Phonon::MediaSource("../myPlayer/music.mp3"));
    connect(music,SIGNAL(stateChanged(Phonon::State,Phonon::State)),this,SLOT(stateChanged(Phonon::State,Phonon::State)));

    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(tableChilked(int)));

    listMusic=new Phonon::MediaObject(this);
    Phonon::AudioOutput *output=new Phonon::AudioOutput(Phonon::MusicCategory,this);
    Phonon::createPath(listMusic,output);
    connect(listMusic,SIGNAL(stateChanged(Phonon::State,Phonon::State)),this,SLOT(metaStateChanged(Phonon::State,Phonon::State)));

    connect(music,SIGNAL(currentSourceChanged(Phonon::MediaSource)),this,SLOT(sourceChanged(Phonon::MediaSource)));

    connect(music,SIGNAL(aboutToFinish()),this,SLOT(aboutToFinsh()));

}

Player::~Player()
{
    delete ui;
}
/**
 *退出音乐播放器
 * @brief Player::on_exitButton_clicked
 */
void Player::on_exitButton_clicked()
{
    this->close();
}
/**
 *更新进度
 * @brief Player::updateTime
 * @param time
 */
void Player::updateTime(qint64 time){

    qint64 totalTimeValue = music->totalTime();//获取时间
    QTime totalTime(0, (totalTimeValue / 60000) % 60, (totalTimeValue / 1000) % 60);
    QTime currentTime(0, (time / 60000) % 60, (time / 1000) % 60);
    QString str = currentTime.toString("mm:ss") + " / " + totalTime.toString("mm:ss");
    ui->timtLabel->setText(str);
}
/**
 *音乐播放状态改变
 * @brief Player::stateChanged
 * @param s1
 * @param s2
 */
void Player::stateChanged(Phonon::State s1, Phonon::State s2){

    switch(s1){
    case Phonon::ErrorState:
        if(music->errorType()==Phonon::FatalError){
            QMessageBox::warning(this,tr("大错误"),music->errorString());

        }else{
            QMessageBox::warning(this,tr("错误"),music->errorString());

        }
        break;
    case Phonon::PlayingState:
        noPlay->setEnabled(true);
    case Phonon::StoppedState:
        noPlay->setEnabled(false);
    case Phonon::PausedState:
        noPlay->setEnabled(true);
    case Phonon::BufferingState:
        break;
    default:
        ;
    }
}
/**
 *音乐资源改变
 * @brief Player::sourceChanged
 * @param sou
 */
void Player::sourceChanged(const Phonon::MediaSource &sou){

    int index=sources.indexOf(sou);
    ui->tableWidget->selectRow(index);
    changeActionState();

}
/**
 * @brief Player::aboutToFinsh
 */
void Player::aboutToFinsh(){
    int index=sources.indexOf(music->currentSource())+1;
    if(sources.size()>index){
        music->enqueue(sources.at(index));//入队
        music->seek(music->totalTime());
    }else{
        music->stop();
    }

}
/**
 * @brief Player::metaStateChanged
 * @param s1
 * @param s2
 */
void Player::metaStateChanged(Phonon::State s1, Phonon::State s2){

    if(s1==Phonon::ErrorState){
        QMessageBox::warning(this,tr("打开文件时出错!"),listMusic->errorString());
        while(!sources.isEmpty()&&!(sources.takeLast()==listMusic->currentSource())){

        }
        return;
    }

    if(s1!=Phonon::StoppedState&&s1!=Phonon::PausedState)
        return;
    if(listMusic->currentSource().type()==Phonon::MediaSource::Invalid)
        return;

    QMap<QString,QString> metaData=listMusic->metaData();

    QString title=metaData.value("TITLE");

    if(title==""){
        QString str=listMusic->currentSource().fileName();
        title=QFileInfo(str).baseName();
    }
    /**
     *标题
     * @brief titleItem
     */
    QTableWidgetItem *titleItem=new QTableWidgetItem(title);
    titleItem->setFlags(titleItem->flags() ^ Qt::ItemIsEditable);

    titleItem->setFlags(titleItem->flags()^Qt::ItemIsEditable);
    /**
     *艺术家
     * @brief artistItem
     */
    QTableWidgetItem *artistItem=new QTableWidgetItem(metaData.value("ARTIST"));

    artistItem->setFlags(titleItem->flags()^Qt::ItemIsEditable);

    qint64 totalTime=listMusic->totalTime();

    QTime time(0,(totalTime/60000)%60,(totalTime/1000)%60);
    /**
     *播放时间
     * @brief timeItem
     */
    QTableWidgetItem *timeItem=new QTableWidgetItem(time.toString("mm::ss"));

    int row=ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
    ui->tableWidget->setItem(row,0,titleItem);
    ui->tableWidget->setItem(row,1,artistItem);
    ui->tableWidget->setItem(row,2,timeItem);

    int index=sources.indexOf(listMusic->currentSource())+1;

    if(sources.size()>index){
        listMusic->setCurrentSource(sources.at(index));//设置当前播放的音乐
    }else{
        if(ui->tableWidget->selectedItems().isEmpty()){

            if(music->state()!=Phonon::PlayingState&&music->state()!=Phonon::PausedState){
                music->setCurrentSource(sources.at(0));
            }else{
                ui->tableWidget->selectRow(0);
                changeActionState();
            }

        }else{
            changeActionState();
        }
    }
}
/**
 *
 * @brief Player::changeActionState
 */
void Player::changeActionState(){
    if(sources.count()==0){
        if(music->state()!=Phonon::PlayingState&&music->state()!=Phonon::PausedState){
            play->setEnabled(false);
            noPlay->setEnabled(false);
        }
        skipBackWard->setEnabled(false);
        skipForWard->setEnabled(false);
    }else{
        skipBackWard->setEnabled(true);
        skipForWard->setEnabled(true);
        int index=ui->tableWidget->currentRow();
        if(index==0)
            skipBackWard->setEnabled(false);
        if(index+1==sources.count())
           skipForWard->setEnabled(false);
    }
}
/**
 *单击tableWidget时播放选中的音乐
 * @brief Player::tableChilked
 * @param row
 */
void Player::tableChilked(int row){

    bool wasPlaying=music->state()==Phonon::PlayingState;
    music->stop();
    music->clearQueue();

    if(row>=sources.size())
        return;
    music->setCurrentSource(sources.at(row));
    if(wasPlaying)
        music->play();
}
/**
 *
 * @brief Player::clearSources
 */
void Player::clearSources(){
    sources.clear();
    changeActionState();
}
/**
 *打开音乐文件
 * @brief Player::Open
 */
void Player::Open(){
    // 从系统音乐目录打开多个音乐文件
    QStringList list = QFileDialog::getOpenFileNames(this, tr("打开音乐文件"),
                         QDesktopServices::storageLocation(QDesktopServices::MusicLocation));

    if(list.isEmpty())
        return;
    int index=sources.size();
    /**
     *遍历音乐资源列表，将音乐加上
     */
    foreach(QString string,list){
        Phonon::MediaSource source(string);
        sources.append(source);
    }
    if(!sources.isEmpty()){
         listMusic->setCurrentSource(sources.at(index));
    }
}
/**
 *播放
 * @brief Player::Play
 */
void Player::Play(){
    music->play();
}
/**
 *不播放
 * @brief Player::Noplay
 */
void Player::Noplay(){
    music->pause();
}
/**
 *后一首
 * @brief Player::SkipBackWard
 */
void Player::SkipBackWard(){

    int index=sources.indexOf(music->currentSource());
    music->setCurrentSource(sources.at(index-1));
    music->play();
}

/**
 *前一首
 * @brief Player::SkipForWard
 */
void Player::SkipForWard(){

    int index=sources.indexOf(music->currentSource());
    music->setCurrentSource(sources.at(index+1));
    music->play();
}
/**
 *显示歌词，还没实现
 * @brief Player::showLRC
 */
void Player::showLRC(){



}

// 清空播放列表
void Player::clearPlaylist()
{
    while (ui->tableWidget->rowCount())
        ui->tableWidget->removeRow(0);
    // 发射播放列表已清空信号
    //emit playlistClean();
}
/**
 *
 * @brief Player::contextMenuEvent
 * @param event
 */
void Player::contextMenuEvent(QContextMenuEvent *event){
    QMenu menu;
    menu.addAction(tr("清空列表"),this,SLOT(clearPlaylist()));
    menu.exec(event->globalPos());
}

