# Welcome to QArchive library

The QtLibArchive project is port of [libarchive](https://github.com/libarchive/libarchive) for Qt Applications.

It can help you extract/compress almost of archive file type.

## Features

- Portable
- Backward-compatible
- Lightweight (~300KB static build)
- Single/Multi - Thread support
- Hard Disk Extract/Compress support

## Requirements

- Qt >= 5.6.2
- LibArchive binary (static/shared build)

## Examples

### Fast extract

```cpp
// Create multi-thread hard disk extractor
auto extractor = new QArchive::DiskExtractor(this);
extractor->setArchiveFileName(fileName);
extractor->setOutputPath(QDir::currentPath());
extractor->setPassword("ThisIsExtractPassword");
extractor->start();
```

### Signals

```cpp
void QArchive::DisExtractor::started();
void QArchive::DisExtractor::progress(const QString& fileName, const std::size_t current, const std::size_t total);
void QArchive::DisExtractor::error(const StatusCode code, const QString& fileName);
void QArchive::DisExtractor::finished();
```

## LICENSE

[MIT License](https://github.com/HadesD/QtLibArchive/blob/master/LICENSE)

