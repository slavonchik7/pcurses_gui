#ifndef __PDCRESIZER_HPP__
#define __PDCRESIZER_HPP__

#ifndef _WIN32
#warning "PDCResizeChecker not work only for WINDOWS"
#endif

#include <QObject>
#include <QMutex>
#include <QWaitCondition>

class PDCResizeChecker 
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

    void notifyErrorSlot(void *);
    void notifyResizeSlot(void *, int, int);

private:
    void            *m_handle; /* windows HANDLE type */
    QThread         *m_thread;
    Qt::HANDLE      m_thHandle;
    
    QMutex          m_mut;
    QWaitCondition  m_mutCond;
    bool            m_thIdReady;
};  


#endif /* __PDCRESIZER_HPP__ */
