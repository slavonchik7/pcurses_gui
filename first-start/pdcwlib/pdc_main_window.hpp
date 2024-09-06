#ifndef __PDC_MAIN_WINDOW_HPP__
#define __PDC_MAIN_WINDOW_HPP__

#ifndef _WIN32
#warning "PDCResizeChecker not work only for WINDOWS"
#endif

#include <QObject>
#include <QMutex>
#include <QWaitCondition>

class PDCMainWindow
    :   public QObject
{
    Q_OBJECT

public:
    explicit PDCResizeChecker();
    ~PDCResizeChecker();

    void start(void *handle);

signals:
    void resized(int width /* new */, int height /* new */);
    void errorOccurred(void);

private slots:
    void process();

private:
    QThread         *m_thread;
    
    QMutex          m_mut;
    QWaitCondition  m_mutCond;
    bool            m_thIdReady;
};  


#endif /* __PDC_MAIN_WINDOW_HPP__ */