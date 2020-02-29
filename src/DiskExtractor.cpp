#include "DiskExtractor.hpp"

#include <QCoreApplication>
#include <QFile>
#include <QDir>

#include <libarchive/archive.h>
#include <libarchive/archive_entry.h>

#include "Log.hpp"

namespace QArchive
{
  DiskExtractor::DiskExtractor(QObject *parent) : QObject(parent)
  {
    LOG_DEBUG(__FUNCTION__ << "called");

    m_archiveRead = archive_read_new();
    archive_read_support_format_all(m_archiveRead);
    archive_read_support_compression_all(m_archiveRead);

    connect(&m_extractThread, &QThread::started, [=](){
      this->startExtract();
    });

    connect(&m_extractThread, &QThread::finished, this, &DiskExtractor::finished);
  }

  DiskExtractor::~DiskExtractor()
  {
    LOG_DEBUG(__FUNCTION__ << "called");

    archive_read_free(m_archiveRead);
  }

  void DiskExtractor::setArchiveFileName(const QString& fileName)
  {
    m_archiveFileName = fileName;
  }

  void DiskExtractor::setOutputPath(const QString& outputPath)
  {
    m_outputPath = outputPath;
  }

  void DiskExtractor::setPassword(const QString &password)
  {
    archive_read_add_passphrase(m_archiveRead, password.toLatin1().constData());
  }

  bool DiskExtractor::hasError() const
  {
    return m_hasError;
  }

  void DiskExtractor::emitError(const StatusCode code, const QString& errorString)
  {
    LOG_DEBUG(__FUNCTION__ << static_cast<short>(code) << errorString);

    m_hasError = true;
    Q_EMIT this->error(code, errorString);
  }

  void DiskExtractor::start()
  {
    LOG_DEBUG(__FUNCTION__ << "called");
    m_extractThread.start();
  }

  void DiskExtractor::startExtract()
  {
    LOG_DEBUG(__FUNCTION__ << QThread::currentThread());

    Q_EMIT this->started();

    if (!QFile::exists(m_archiveFileName))
    {
      this->emitError(StatusCode::FILE_NOT_EXISTS, m_archiveFileName);
      m_extractThread.quit();
      return;
    }

    m_outputDir.setPath(
          m_outputPath.isEmpty() ? QDir::currentPath() : m_outputPath
                                   );
    if (!m_outputDir.exists())
    {
      if (!m_outputDir.mkpath(".."))
      {
        this->emitError(StatusCode::DIR_NOT_EXISTS, m_outputDir.path());
        m_extractThread.quit();
        return;
      }
    }

    int ret = ARCHIVE_OK;
#if _WIN32
#define archive_read_open_filename archive_read_open_filename_w
#endif
    ret = archive_read_open_filename(
          m_archiveRead, m_archiveFileName.toStdWString().c_str(), m_blockSize
          );
    if (ret != ARCHIVE_OK)
    {
      this->emitError(StatusCode::LIBARCHIVE_READ_ERROR, archive_error_string(m_archiveRead));
      m_extractThread.quit();
      return;
    }

    archive_entry* entry;
    while ((ret = archive_read_next_header(m_archiveRead, &entry)) != ARCHIVE_EOF)
    {
      if ((ret < ARCHIVE_OK) || (ret < ARCHIVE_WARN))
      {
        this->emitError(
              StatusCode::LIBARCHIVE_READ_ERROR,
              archive_error_string(m_archiveRead)
              );
        break;
      }

      if (!this->writeEntry(entry))
      {
        continue;
      }
    }

    // Extract completed
    archive_read_close(m_archiveRead);
    m_extractThread.quit();
  }

  bool DiskExtractor::writeEntry(archive_entry *entry)
  {
    const char* entryPath = archive_entry_pathname(entry);
    auto outputFilePath = m_outputDir.filePath(entryPath);
    QDir outputDir(outputFilePath);
    if (!outputDir.mkpath(".."))
    {
      this->emitError(StatusCode::DIR_NOT_EXISTS, "Mkpath: " + outputFilePath);
      return false;
    }

    // Is Dir
    if (archive_entry_filetype(entry) == AE_IFDIR)
    {
      Q_EMIT this->progress(entryPath, 100, 100);
      return true;
    }
    QFile hOutputFile(outputFilePath);
    if (!hOutputFile.open(QIODevice::WriteOnly))
    {
      this->emitError(StatusCode::LIBARCHIVE_WRITE_ERROR, "Open: " + outputFilePath);
      return false;
    }

    auto entrySize = archive_entry_size(entry);
    LOG_DEBUG(__FUNCTION__ << entryPath << entrySize);
    int totalReadSize = 0;
    constexpr int bufferSize = 1024000 * 20; // 20MB
    auto bSize = bufferSize > entrySize ? entrySize : bufferSize;
    uchar* buff = new uchar[bSize];
    bool writeSuccessed = true;
    while (totalReadSize < entrySize)
    {
      auto readSize = static_cast<int>(
            archive_read_data(m_archiveRead, buff, bSize)
            );

      if (readSize < 0)
      {
        writeSuccessed = false;
        this->emitError(StatusCode::LIBARCHIVE_WRITE_ERROR, archive_error_string(m_archiveRead));
        break;
      }

      totalReadSize += readSize;
      if (readSize != hOutputFile.write(reinterpret_cast<char*>(buff), readSize))
      {
        writeSuccessed = false;
        this->emitError(StatusCode::LIBARCHIVE_WRITE_ERROR, outputFilePath);
        break;
      }

      Q_EMIT this->progress(entryPath, totalReadSize, entrySize);

      LOG_DEBUG(__FUNCTION__ << "read" << totalReadSize);
    }
    delete[] buff;
    hOutputFile.close();

    return writeSuccessed;
  }

  QString statusCodeToString(const DiskExtractor::StatusCode code)
  {
    switch (code)
    {
      case DiskExtractor::StatusCode::SUCCESSED:
        return "Extractor::StatusCode::SUCCESSED";
      case DiskExtractor::StatusCode::DIR_NOT_EXISTS:
        return "Extractor::StatusCode::DIR_NOT_EXISTS";
      case DiskExtractor::StatusCode::FILE_NOT_EXISTS:
        return "Extractor::StatusCode::FILE_NOT_EXISTS";
      case DiskExtractor::StatusCode::LIBARCHIVE_READ_ERROR:
        return "Extractor::StatusCode::LIBARCHIVE_READ_ERROR";
      case DiskExtractor::StatusCode::LIBARCHIVE_WRITE_ERROR:
        return "DiskExtractor::StatusCode::LIBARCHIVE_WRITE_ERROR";
      case DiskExtractor::StatusCode::LIBARCHIVE_FILE_CORRUPTED:
        return "DiskExtractor::StatusCode::LIBARCHIVE_FILE_CORRUPTED";
      default:
        return "Code not handled";
    }
  }
}
