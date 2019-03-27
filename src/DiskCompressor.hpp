#ifndef QARCHIVE_DISKCOMPRESSOR_HPP
#define QARCHIVE_DISKCOMPRESSOR_HPP

#include <QObject>

namespace QArchive {
  class DiskCompressor : public QObject
  {
      Q_OBJECT
    public:
      explicit DiskCompressor(QObject *parent = nullptr);

    signals:

    public slots:
  };
}

#endif // DISKCOMPRESSOR_HPP
