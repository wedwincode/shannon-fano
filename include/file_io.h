#ifndef FILE_IO_H
#define FILE_IO_H

#include <fstream>

#include "String.h"
#include "types.h"
#include "Vector.h"
#include "FanoExceptions.h"


namespace file_io
{
  void checkFiles(const String& inputFilePath, const String& outputFilePath);

  Buffer readFileToBuffer(const String& inputFile);

  void writeToFile(const String& outputFile, const Packed& packed);

  size_t getFileSize(const String& filePath);
}


#endif //FILE_IO_H
