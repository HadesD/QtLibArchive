#ifndef QARCHIVE_DISKEXTRACTOR_HPP
#define QARCHIVE_DISKEXTRACTOR_HPP

#include <QDir>
#include <QObject>
#include <QThread>

struct archive;
struct archive_entry;

namespace QArchive
{
  class DiskExtractor : public QObject
  {
      Q_OBJECT

    public:
      enum class StatusCode : short
      {
        UNKNOW_CODE,
        SUCCESSED,
        FILE_NOT_EXISTS,
        DIR_NOT_EXISTS,
        LIBARCHIVE_READ_ERROR,
        LIBARCHIVE_WRITE_ERROR,
        LIBARCHIVE_FILE_CORRUPTED,
        LIBARCHIVE_WRONG_PASSWORD,
      };

    public:
      explicit DiskExtractor(QObject *parent = nullptr);
      ~DiskExtractor();

    public:
      bool hasError() const;
      void start();
      void setArchiveFileName(const QString& fileName);
      void setOutputPath(const QString& outputPath);
      void setPassword(const QString& password);

    private:
      void emitError(const StatusCode code, const QString& errorString);
      void startExtract();
      bool writeEntry(archive_entry *entry);

    Q_SIGNALS:
      void started();
      void progress(
          const QString& fileName,
          const std::size_t current, const std::size_t total
          );
      void error(const StatusCode code, const QString& fileName);
      void finished();

    public Q_SLOTS:

    private:
      QThread m_extractThread;
      QString m_archiveFileName;
      QString m_outputPath;
      QDir m_outputDir;
      bool m_hasError = false;

      // Read the archive handle
      archive* m_archiveRead = nullptr;

      std::size_t m_blockSize = 10240;
      QString m_password;
  };

  QString statusCodeToString(const DiskExtractor::StatusCode code);
}

#endif
