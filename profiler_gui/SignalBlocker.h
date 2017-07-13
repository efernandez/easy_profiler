#ifndef SIGNALBLOCKER_H
#define SIGNALBLOCKER_H

#include <QtGlobal>
#if QT_VERSION >= 0x050300
    #include <QSignalBlocker>
#else

QT_BEGIN_NAMESPACE

class QSignalBlocker
{
public:
    inline explicit QSignalBlocker(QObject *o) Q_DECL_NOTHROW;
    inline explicit QSignalBlocker(QObject &o) Q_DECL_NOTHROW;
    inline ~QSignalBlocker();

#ifdef Q_COMPILER_RVALUE_REFS
    inline QSignalBlocker(QSignalBlocker &&other) Q_DECL_NOTHROW;
    inline QSignalBlocker &operator=(QSignalBlocker &&other) Q_DECL_NOTHROW;
#endif

    inline void reblock() Q_DECL_NOTHROW;
    inline void unblock() Q_DECL_NOTHROW;
private:
    Q_DISABLE_COPY(QSignalBlocker)
    QObject * m_o;
    bool m_blocked;
    bool m_inhibited;
};

QSignalBlocker::QSignalBlocker(QObject *o) Q_DECL_NOTHROW
    : m_o(o),
      m_blocked(o && o->blockSignals(true)),
      m_inhibited(false)
{}

QSignalBlocker::QSignalBlocker(QObject &o) Q_DECL_NOTHROW
    : m_o(&o),
      m_blocked(o.blockSignals(true)),
      m_inhibited(false)
{}

#ifdef Q_COMPILER_RVALUE_REFS
QSignalBlocker::QSignalBlocker(QSignalBlocker &&other) Q_DECL_NOTHROW
    : m_o(other.m_o),
      m_blocked(other.m_blocked),
      m_inhibited(other.m_inhibited)
{
    other.m_o = Q_NULLPTR;
}

QSignalBlocker &QSignalBlocker::operator=(QSignalBlocker &&other) Q_DECL_NOTHROW
{
    if (this != &other) {
        // if both *this and other block the same object's signals:
        // unblock *this iff our dtor would unblock, but other's wouldn't
        if (m_o != other.m_o || (!m_inhibited && other.m_inhibited))
            unblock();
        m_o = other.m_o;
        m_blocked = other.m_blocked;
        m_inhibited = other.m_inhibited;
        // disable other:
        other.m_o = Q_NULLPTR;
    }
    return *this;
}
#endif

QSignalBlocker::~QSignalBlocker()
{
    if (m_o && !m_inhibited)
        m_o->blockSignals(m_blocked);
}

void QSignalBlocker::reblock() Q_DECL_NOTHROW
{
    if (m_o) m_o->blockSignals(true);
    m_inhibited = false;
}

void QSignalBlocker::unblock() Q_DECL_NOTHROW
{
    if (m_o) m_o->blockSignals(m_blocked);
    m_inhibited = true;
}

QT_END_NAMESPACE

#endif

#endif // SIGNALBLOCKER_H
