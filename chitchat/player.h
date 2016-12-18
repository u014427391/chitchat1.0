#ifndef PLAYER_H
#define PLAYER_H



#include <QAction>
#include <QToolBar>
#include <Phonon>
#include <QTime>
#include <QMessageBox>
#include <QCloseEvent>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QDesktopServices>
#include <QTextCodec>
#include <QMainWindow>

namespace Ui {
class Player;
}
/**
 *播发器功能的头文件
 * @brief The Player class
 */
class Player : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Player(QWidget *parent = 0);
    void playerInit();
    ~Player();

protected:
     void contextMenuEvent(QContextMenuEvent *event);//内容菜单事件

 private:
        Ui::Player *ui;
        QAction *play;
        QAction *noPlay;
        QAction *skipBackWard;//后一首
        QAction *skipForWard;//前一首
        QAction *open;
        QAction *lrc;

        QToolBar *toolBar;

        Phonon::MediaObject *music;//媒体对象
        Phonon::AudioOutput *audioOutput;
        Phonon::VolumeSlider *volumeSlider;//音量调节条
        Phonon::SeekSlider *seekSlider;//

        QList<Phonon::MediaSource> sources;//列表用于存储缓存音乐文件
        Phonon::MediaObject *listMusic;
         void changeActionState();
signals:
       void playlistClean();

private slots:
    void updateTime(qint64 time);//更新时间
    void clearPlaylist();

    void stateChanged(Phonon::State s1,Phonon::State s2);

    void sourceChanged(const Phonon::MediaSource &sou);
    void aboutToFinsh();
    void metaStateChanged(Phonon::State s1,Phonon::State s2);
    void tableChilked(int row);//单击tableWidget
    void clearSources();

    void Open();
    void Play();
    void Noplay();
    void SkipBackWard();
    void SkipForWard();
    void showLRC();
    void on_exitButton_clicked();

};

#endif // PLAYER_H
