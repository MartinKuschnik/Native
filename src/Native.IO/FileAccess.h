#pragma once

namespace Native
{
	namespace IO
	{
        enum class FileAccess
        {
            // Specifies read access to the file. Data can be read from the file and
            // the file pointer can be moved. Combine with WRITE for read-write access.
            Read = 1,

            // Specifies write access to the file. Data can be written to the file and
            // the file pointer can be moved. Combine with READ for read-write access.
            Write = 2,

            // Specifies read and write access to the file. Data can be written to the
            // file and the file pointer can be moved. Data can also be read from the 
            // file.
            ReadWrite = 3
        };
	}
}