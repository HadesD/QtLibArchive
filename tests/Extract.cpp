#include "../include/QArchive"

#include <QDir>

#include "../src/Log.hpp"

int main()
{
  // Create multi-thread hard disk extractor
  QString fileName = "test.zip";
  auto extractor = new QArchive::DiskExtractor(this);
  extractor->setArchiveFileName(fileName);
  extractor->setOutputPath(QDir::currentPath());

  // Connect event
  connect(extractor, &QArchive::DiskExtractor::started, [=](){
    LOG_DEBUG("[+] Started extract:" << fileName);
  });

  static int speedByte;
  connect(extractor, &QArchive::DiskExtractor::progress, [=](const QString& fileName, int val, int total){
    LOG_DEBUG("[+] Extracting:" << fileName << val << total);
  });

  connect(extractor, &QArchive::DiskExtractor::error, [=](auto code, const QString& name){
    const short codeNum = static_cast<short>(code);
    LOG_DEBUG("[-] Extract Error:" << codeNum << QArchive::statusCodeToString(code) << name);

    // Remove file after extracted
    QFile::remove(fileName);
  });

  connect(extractor, &QArchive::DiskExtractor::finished, [=](){
    LOG_DEBUG("[+] Extracted!");

    // Destructor
    extractor->deleteLater();
  });

  // Start extract test.zip
  extractor->start();

  return 0;
}

