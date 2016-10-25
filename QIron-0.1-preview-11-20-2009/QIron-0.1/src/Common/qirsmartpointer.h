#ifndef QIRSMARTPOINTER_H
#define QIRSMARTPOINTER_H

#include <QtGlobal>

template < class T >
class QIrScopedPointer
{	
public:
	inline explicit QIrScopedPointer( T * p = 0 ) : m_d(p) { }	
	inline ~QIrScopedPointer()  { reset(); }
	
	inline T * data() const { return m_d; }
	inline bool isNull() const { return (m_d == 0); }
	inline operator T * () const { return m_d; }
	inline T & operator * () const { return *m_d; }
	inline T * operator -> () const { return m_d; }
	inline QIrScopedPointer< T > & operator = ( T * p ) { reset(); m_d = p; return *this; }
	inline void reset() { if (m_d) {delete m_d; m_d = 0;} }

private:
	Q_DISABLE_COPY(QIrScopedPointer)
	T * m_d;
};

#endif // QIRSMARTPOINTER_H